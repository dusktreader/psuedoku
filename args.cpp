#include "args.h"

using namespace std;

void parseArgs( int argc, char** argv, ArgStruct* args ){
    const struct option longopts[] = {
        { "q",             required_argument, NULL, 'q' },
        { "load_puzzle",   required_argument, NULL, 'l' },
        { "print",         optional_argument, NULL, 'p' },
        { "time_it",             no_argument, NULL, 't' },
        { "verify",              no_argument, NULL, 'v' },
        { "help",                no_argument, NULL, 'h' },
        { 0,                               0,    0,  0  }
    };

    int idx = 0;
    int arg = getopt_long( argc, argv, "q:l:p::tvh", longopts, &idx );
    while( arg != -1 ){
        switch( arg ){
            case 'q': str2num( optarg, args->q );                        break;
            case 'l': args->l = optarg;                                  break;
            case 'p': args->p = optarg == NULL ? "stdout" : optarg;      break;
            case 't': args->t = true;                                    break;
            case 'v': args->v = true;                                    break;
            case 'h': args->h = true;                                    break;
        }
        arg = getopt_long( argc, argv, "q:l:p::tvh", longopts, &idx );
    }
}

string usage( string progName ){
    string descr =  "Usage Description: \n";
           descr += "================== \n";
           descr += progName + " [opt] [arg] [opt] [arg] ... \n";
           descr += "  Option, Argument summary: \n";
           descr += "  ------------------------- \n";
           descr += "  -q [int], --q=[int]: \n";
           descr += "      The length of one side of one block \n";
           descr += "      Defaults to 3 ( a 9x9 sudoku ) \n\n";
           descr += "  -l [string], --load_puzzle=[string] \n";
           descr += "      Load puzzle values from file. \n\n";
           descr += "  -p [string], --print, --print=[string] \n";
           descr += "      Print the puzzle.  If an arument is specified, \n";
           descr += "      The puzzle will be printed to the file. \n";
           descr += "      Otherwise, the puzzle is printed to stdout. \n\n";
           descr += "  -t, --time_it \n";
           descr += "      Time the solution.  Prints time used to stdout \n\n";
           descr += "  -v, --verify \n";
           descr += "      Verify the solution.  Prints result to stdout \n\n";
           descr += "  -u, --help \n";
           descr += "      View this usage description \n\n";
    return descr;
}
