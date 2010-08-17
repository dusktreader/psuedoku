#pragma once
#include "getopt.h"
#include "tools.h"
extern char* optarg;

/** A structure that holds values for command line arguments */
typedef struct argStruct{
    int q;
    std::string l;
    std::string p;
    bool t;
    bool v;
    bool h;
    argStruct():
        q( 3 ), l( "" ), p( "" ), t( false ), v(false), h( false ){}
} ArgStruct;

/** Parses command line arguments
  * @param  argc - The number of arguments to parse
  * @param  argv - A vector containig the values of the arguments
  * @param  args - An ArgStruct to hold the values of the arguments
  */
void parseArgs( int argc, char** argv, ArgStruct* args );

/** Prints out a usage description */
std::string usage( std::string progName );
