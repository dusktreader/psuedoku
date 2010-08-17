#include "cell.h"

using namespace std;

Cell::Cell( int n ){
    this->n = n;
    clear();
}

Cell::~Cell(){}

void Cell::clear(){
    attempted.clear();
    candidates.clear();
    for( int i=1; i<=n; i++ )
        candidates.insert( i );
    reservations = vector<int>( n + 1, 0 );
    value = NO_VALUE;
    constant = false;
}

void Cell::addNeighbor( Cell* neighbor ){
    neighbors.push_back( neighbor );
}

int Cell::getValue(){
    return value;
}

vector<int> Cell::getCandidates(){
    vector<int> clist;
    clist.insert( clist.begin(), candidates.begin(), candidates.end() );
    return clist;
}

bool Cell::setValue( int value ){

    // If the cell value will be unchanged, do nothing
    if( value == this->value ){
        return false;
    }

    // If the cell has a value, that value needs to be released in the neighbors
    if( this->value != NO_VALUE ){
        #pragma omp parallel for
        for( int i=0; i<(int)neighbors.size(); i++ )
            neighbors[i]->releaseCandidate( this->value );
    }

    // If the new value is a value, it needs to be claimed in the neighbors
    if( value != NO_VALUE ){
        // See if the value is a candidate.  If it is not, the value cannot be set
        if( candidates.count( value ) == 0 )
            return false;
        #pragma omp parallel for
        for( int i=0; i<(int)neighbors.size(); i++ )
            neighbors[i]->claimCandidate( value );
        // Specify that this cell value was explicitly set by the user ( Not set during the solve function ).
        constant = true;
    }
    // If the new value is not a value, return the current value to the candidates
    else{
        candidates.insert( this->value );
        // Specify that this cell is no long explicitly set
        constant = false;
    }
    this->value = value;
    return true;
}

bool Cell::resetValue(){
    if( !constant )
        return false;
    #pragma omp parallel for
    for( int i=0; i<(int)neighbors.size(); i++ )
        neighbors[i]->claimCandidate( value );
    return true;
}

bool Cell::nextValue(){
    // If the cell has been previously assigned a value, release reservations on the value in all of its neighbors
    if( value != NO_VALUE ){
        #pragma omp parallel for
        for( int i=0; i<(int)neighbors.size(); i++ )
            neighbors[i]->releaseCandidate( value );
    }
    // If there are no remaining candidates, return all attempted candiates back to the candidate pool
    if( candidates.empty() ){
        set<int>::iterator i;
        // This is the single-nowait trick which parallelizes an STL iterator.
        // See: www.compunity.org/events/pastevents/parco07/parco_cpp_openmp.pdf  page 14
        #pragma omp parallel private( i )
        { // ++omp parallel
            // Release 1 local reservation for each value in the attemtped set.
            // This removes the 1 reservation buffer, meaning neighbors can again manipulate candidates
            for( i = attempted.begin(); i != attempted.end(); i++ ){
                #pragma omp single nowait
                { // ++omp single
                    reservations[ *i ]--;
                } // --omp single
            }
        } // --omp parallel
        candidates.insert( attempted.begin(), attempted.end() );
        attempted.clear();
        value = NO_VALUE;
        return false;
    }
    value = *(candidates.begin());
    attempted.insert( value );
    candidates.erase( value );
    // Indicate that the value is reserved.  This will prevent it from being added back in by another cell.
    reservations[value]++;
    #pragma omp parallel for
    for( int i=0; i<(int)neighbors.size(); i++ )
        neighbors[i]->claimCandidate( value );
    return true;
}

void Cell::claimCandidate( int value ){
    // If there were no reservations for the value yet, remove it from the candidates and increment its reservation
    #pragma omp critical
    { // ++omp critical
    if( reservations[ value ]++ == 0 )
        candidates.erase( value );
    } // --omp critical
}

void Cell::releaseCandidate( int value ){
    // If thhere are no remaining reservations for the value, return it to the candidates list
    #pragma omp critical
    { // ++omp critical
    if( --reservations[ value ] == 0 )
        candidates.insert( value );
    } // --omp critical
}

bool Cell::isUnique(){
    return isUnique( value );
}

bool Cell::isUnique( int value ){
    bool unique = true;
    // Verify uiniqueness of the value in the neighborhood.  Use the reduction operation to combine thread results
    #pragma omp parallel for reduction( &&:unique )
    for( int i=0; i<(int)neighbors.size(); i++ )
        unique = unique && neighbors[i]->value != value;
    return unique;
}

bool Cell::isConstant(){
    return constant;
}

int Cell::candidateCount(){
    return candidates.size();
}

bool Cell::cmp( const Cell* A, const Cell* B ){
    return A->candidates.size() < B->candidates.size();
}
