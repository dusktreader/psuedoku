#pragma once

#include "local.h"
#include "cell.h"

class Board{
private:
    int q;
    int n;
    int N;
    std::vector<Cell*> cells;
    Cell* getCell( int i, int j );
    Cell* popCell( std::vector<Cell*> &cells );
    void setZones();
public:
    Board( int q );
    ~Board();
    void clear();
    void reset();
    void setCellValue( int i, int j, int value );
    int getCellValue( int i, int j );
    std::vector<int> getCandidates( int i, int j );
    int getn();
    int getN();
    int getq();
    bool solve();
    void print( std::ostream &out );
    bool verify();
    bool read( std::string boardFile );
    bool write( std::string boardFile );
};
