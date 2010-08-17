#include "args.h"
#include "board.h"
#include "tools.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <ostream>

using namespace std;

int main(int argc, char *argv[]){
    ArgStruct args;
    parseArgs( argc, argv, &args );
    if( args.h ){
        usage( argv[0] );
        return 0;
    }

    Board board( args.q );
    if( args.l != "" )
        ASSERT_MSG( board.read( args.l ), "Couldn't read input file" );
    if( args.t )
        clock();
    board.solve();
    if( args.t ){
        double elapsed = clock() / (double)CLOCKS_PER_SEC;
        cout << "Solved the puzzle in " << num2str( elapsed, 3 ) << " seconds " << endl;
    }
    if( args.p != "" ){
        if( args.p == "stdout" )
            board.print( cout );
        else{
            ofstream out( args.p.c_str() );
            board.print( out );
        }
    }
    return 0;
}
