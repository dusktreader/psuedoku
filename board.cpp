#include "board.h"

using namespace std;

Board::Board( int q ){
    this->q = q;
    n = q * q;
    N = n * n;
    for( int k=0; k<N; k++ )
        cells.push_back( new Cell( n ) );
    setZones();
}

Board::~Board(){
    while( !cells.empty() ){
        delete cells.back();
        cells.pop_back();
    }
}

void Board::clear(){
    #pragma omp parallel for
    for( int k=0; k<N; k++ )
        cells[k]->clear();
}

void Board::reset(){
    #pragma omp parallel for
    for( int k=0; k<N; k++ )
        if( !cells[k]->isConstant() )
            cells[k]->clear();
    #pragma omp parallel for
    for( int k=0; k<N; k++ )
        if( cells[k]->isConstant() )
            cells[k]->resetValue();
}

void Board::setZones(){
    int row;
    int col;
    int blockRow0;
    int blockCol0;
    #pragma omp parallel for private( row, col, blockRow0, blockCol0 )
    for( int k=0; k<N; k++ ){
        row = k / n;
        col = k % n;
        blockRow0 = row / q * q;
        blockCol0 = col / q * q;
        // Add neighbors in the same row zone
        for( int i=0; i<n; i++ ){
            if( i >= blockRow0 && i < blockRow0 + q )
                continue;
            cells[k]->addNeighbor( getCell( i, col ) );
        }
        // Add neighbors in the same column zone
        for( int j=0; j<n; j++ ){
            if( j>=blockCol0 && j < blockCol0 + q )
                continue;
            cells[k]->addNeighbor( getCell( row, j ) );
        }
        // Add neibhors in the same column zone
        for( int i=blockRow0; i<blockRow0 + q; i++ ){
            for( int j=blockCol0; j<blockCol0+q; j++ ){
                if( i == row && j == col )
                    continue;
                cells[k]->addNeighbor( getCell( i, j ) );
            }
        }
    }
}

void Board::setCellValue( int i, int j, int value ){
    Cell* cell = getCell( i, j );
    cell->setValue( value );
}

int Board::getCellValue( int i, int j ){
    return getCell( i, j )->getValue();
}

vector<int> Board::getCandidates( int i, int j ){
    return getCell( i, j )->getCandidates();
}

int Board::getn(){
    return n;
}

int Board::getN(){
    return N;
}

int Board::getq(){
    return q;
}

Cell* Board::getCell( int i, int j ){
    return cells[ i * n + j ];
}

Cell* Board::popCell( vector<Cell*> &cells ){
    // These will hold ( per thread ) the cell with the fewest candidates and its index
    Cell* localMin;
    int   localIdx;
    // These will hold the minimum cell and index of all threads
    Cell* globalMin = cells[0];
    int   globalIdx = 0;
    #pragma omp parallel shared( globalMin, globalIdx ) private( localMin, localIdx )
    { // ++omp parallel
        // Set the local values inside the parallel region so that they are the same across all threads
        localMin = globalMin;
        localIdx = globalIdx;
        // Notice that we begin with element 1.  0 is already the local minimum
        #pragma omp for
        for( int i=1; i<(int)cells.size(); i++ ){
            // If the current cell is the minimum, set it
            if( Cell::cmp( cells[i], localMin ) ){
                localMin = cells[i];
                localIdx = i;
            }
        }
        // The barrier will ensure that all threads have found their local minimum cells
        #pragma omp barrier
        // Now, each thread will compare its minimum against the global minimum
        #pragma omp critical
        { // ++omp critical
            // If the local cell has fewer candidates, make it the global minimum
            if( Cell::cmp( localMin, globalMin ) ){
                globalMin = localMin;
                globalIdx = localIdx;
            }
        } // --omp critical
    } // --omp parallel
    // Now extract the global minimum from the cell vector and return it
    cells[ globalIdx ] = cells.back();
    cells.pop_back();
    return globalMin;
}

bool Board::solve(){
    vector<Cell*> newCells;
    stack<Cell*> oldCells;
    // Add all cells that contain a value to the newCells list
    for( int i=0; i<(int)cells.size(); i++ )
        if( !cells[i]->isConstant() )
            newCells.push_back( cells[i] );
    if( newCells.size() == 0 )
        return true;
    random_shuffle( newCells.begin(), newCells.end() );
    // Get the cell with the fewest candidates and put it onto the old cell stack
    oldCells.push( popCell( newCells ) );
    // The loop will continue until the function returns.  The i variable counts iterations
    int i;
    for(i=0;;i++){
        // If we cannot assign a new value to the cell on top of the oldCells stack:
        if( !oldCells.top()->nextValue() ){
            // Put the cell back into the new cell list ( note that it is already reset by nextValue() )
            newCells.push_back( oldCells.top() );
            oldCells.pop();
            // If the oldCells list is empty, no solution could be found
            if( oldCells.empty() )
                return false;
        }
        else{
            // If the newCells list is empty, than a solution has been found
            if( newCells.empty() )
                return true;
            oldCells.push( popCell( newCells ) );
        }
    }
}

bool Board::verify(){
    bool correct = true;
    #pragma omp parallel for reduction( &&: correct )
    for( int i=0; i<(int)cells.size(); i++ )
        correct = cells[i]->isUnique();
    return correct;
}

void Board::print( ostream &out ){
    vector<int> values = vector<int>( cells.size(), 0 );
    #pragma omp parallel for
    for( int i=0; i<(int)cells.size(); i++ )
        values[i] = cells[i]->getValue();
    printMatrix( values.begin(), n, n, "PseudoKu Puzzle", false, 0, 5, out );
}

bool Board::read( string boardFile ){
    clear();
    ifstream in;
    in.open( boardFile.c_str() );
    if( !in.good() )
        return false;
    int i, j, value;
    while( !in.eof() ){
        in.ignore( 500, '(' );
        in >> i;
        in.ignore( 500, ',' );
        in >> j;
        in.ignore( 500, ')' );
        in >> value;
        setCellValue( i, j, value );
        if( value > n ){
            this->clear();
            return false;
        }
        in.ignore( 500, '\n' );
    }
    in.close();
    return true;
}

bool Board::write( string boardFile ){
    ofstream out;
    out.open( boardFile.c_str() );
    if( !out.good() )
        return false;
    int value;
    for( int i=0; i<n; i++ ){
        for( int j=0; j<n; j++ ){
            value = getCellValue( i, j );
            if( value != NO_VALUE )
                out << "( " << i << ", " << j << " ) " << value << endl;
        }
    }
    out.close();
    return true;
}
