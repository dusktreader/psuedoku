#pragma once

#include "local.h"

class Cell{
private:
    std::vector<Cell*> neighbors;
    std::set<int> attempted;
    std::set<int> candidates;
    std::vector<int> reservations;
    int value;
    int n;
    bool constant;
    bool isUnique( int value );
    void claimCandidate( int value );
    void releaseCandidate( int value );
public:
    Cell( int n );
    ~Cell();
    void clear();
    void addNeighbor( Cell* neighbor );
    bool setValue( int value );
    bool resetValue();
    int getValue();
    std::vector<int> getCandidates();
    bool nextValue();
    bool isUnique();
    bool isConstant();
    int candidateCount();
    static bool cmp( const Cell* A, const Cell* B );

};
