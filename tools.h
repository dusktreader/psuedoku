#pragma once
#include <deque>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <algorithm>

/** OS specific flags and etcetera */
#ifdef WIN32
#define OS_SEP_STR "\\"
#define OS_SEP_CHAR '\\'
#define PRINT_FUNC __FUNCSIG__
#else
#define PRINT_FUNC __PRETTY_FUNCTION__
#define OS_SEP_STR "/"
#define OS_SEP_CHAR '/'
#endif

/** The EXEC_TRACE Macros allow the user to trace execution through output
  * @example
  *   #define EXEC_TRACE
  *   int my_func(){
  *     BEGIN_TRACE;
  *     ENTER;
  *     ...
  *     RETURN my_int;
  *   }
  * @note When using the EXEC_TRACE idiom, be sure that RETURNS that are dependant on conditional clauses are
  *       encompassed within a block.  Otherwise undefined behavior will occur.  The curly brackets are required for
  *       proper functionality.  EXEC_TRACE is not thread safe, so do not use it in a function called from within an
  *       omp parallel block.
  *         if( test_val ){
  *             RETURN ret_val;
  *         }
  */
//#define EXEC_TRACE
#ifdef EXEC_TRACE
extern std::deque<std::string> exec_stack;
#define ENTER  { exec_stack.push_back( std::string( __FILE__ ) + ": " + num2str( __LINE__ ) + ": " + PRINT_FUNC ); }
#define RETURN { exec_stack.pop_back(); } return
#define TRACE( code ) try{ code }                           \
                      catch( exception e ){                 \
                          int depth = 0;                    \
                          string func;                      \
                          while( !exec_stack.empty() ){     \
                              func = exec_stack.front();    \
                              depth++;                      \
                              exec_stack.pop_front();       \
                              cout << "|" << flush;         \
                              for( int d=0; d<depth; d++ )  \
                                  cout << "--" << flush;    \
                              cout << "> " << func << endl; \
                          }                                 \
                          cout << endl << e.what() << endl; \
                          exit(1);                          \
                      }
/*
extern int callDepth;
#define BEGIN_TRACE { callDepth = 0; }
#define ENTER  if( callDepth >= 0 ){ \
               callDepth++; \
               cout << "|" << flush; \
               for( int depth=0; depth<callDepth; depth++ ) \
                   cout << "==" << flush; \
               cout << "> " << PRINT_FUNC << endl; }
#define RETURN if( callDepth >= 0 ){ \
               cout << "|<" << flush; \
               for( int depth=0; depth<callDepth; depth++ ) \
                   cout << "==" << flush; \
               cout << " " << PRINT_FUNC << endl; \
               callDepth--; } \
               return
*/
#else
#define ENTER {}
#define RETURN return
#define TRACE( code ) code
#endif

/** The DB_REP Macros convenient for debugging." */
#define DB_REP { cout << __FILE__ << ":" << __LINE__ << ": reporting" << endl; }
#define DB_REP_MSG( msg ) { cout << __FILE__ << ":" << __LINE__ << ": message: " <<  msg << endl; }

/** A custom exception that is thrown when a local ASSERT fails */
class LocalAssert: public std::exception{
private:
    /** Describes the message set during throw */
    std::string message;
public:
    /** Creates the exception with a default message */
    LocalAssert( std::string message = "Local Assertion Failed" ){ this->message = message; }
    /** Destructs the exception */
    ~LocalAssert() throw(){}
    /** Returns the message describing the cause of the exception */
    virtual const char* what() const throw(){ return message.c_str(); }
};

/** Improves the assert() idiom by allowing it to be caught in a try block */
#define ASSERT( expr ) \
    if( ( expr ) == false ) \
        throw LocalAssert( string("in function ") + PRINT_FUNC + \
                           "  of file " + __FILE__ + \
                           " at line " + num2str(__LINE__) + "\n" )

/** Allows an assertion with a failure message */
#define ASSERT_MSG( expr, msg ) \
    if( ( expr ) == false ) \
        throw LocalAssert( string("in function ") + PRINT_FUNC + \
                           "  of file " + __FILE__ + \
                           " at line " + num2str(__LINE__) + \
                           "\n\"" + msg + "\"\n" )

/** Sets a local variable to a value contingent on a logical expression */
#define ASSERT_SET( var, val, expr, msg ) \
    var = val; \
    ASSERT_MSG( expr, msg );

/** Generates a random float in the interval [ 0.0, 1.0 ] */
#define RAND_FLT ( (float)rand() / RAND_MAX )

/** Generates a random double in the interval [ 0.0, 1.0 ] */
#define RAND_DBL ( (double)rand() / RAND_MAX )

#define IN_OPEN_INTERVAL( x, lo, hi )       ( (x) >  (lo) && (x) <  (hi) )
#define IN_CLOSED_INTERVAL( x, lo, hi )     ( (x) >= (lo) && (x) <= (hi) )
#define IN_HALFCLOSED_INTERVAL( x, lo, hi ) ( (x) >= (lo) && (x) <  (hi) )
#define IN_RANGE IN_HALFCLOSED_INTERVAL

#define PI 3.1415926535897931

/** Converts a number to a string
  * @param  number - The number to convert
  * @param  prec   - Floating point precision to use
  * @param  w      - Width of the string
  * @return A string representation of the number
  */
template <class T>
std::string num2str( T number, int prec=2, int w=0 ){
    std::ostringstream ss;
    ss.setf( std::ios::showpoint | std::ios::fixed );
    ss.precision( prec );
    ss.fill( ' ' );
    ss.width( w );
    ss << number;
    return ss.str();
}

/** Converts a string to a number
  * @param  str    - The string to convert
  * @param  number - The storage for the number
  */
template <class T>
void str2num( std::string str, T &number ){
    std::istringstream ss( str );
    ss >> number;
}


template <class T>
void printVar( T var, std::string label="variable", std::ostream &out=std::cout ){
    out << label << " = " << var << std::endl;
}

template <class T>
void printList( T list, int l, std::string label="items", bool abbreviate=false, int prec=2, int width=0, std::ostream &out=std::cout ){
    out << label << ": [ " << std::flush;
    for( int i=0; i<l; i++ ){
        if( abbreviate && i == 3 && l > 10 ){
            out << "... " << std::flush;
            i = l - 4;
        }
        else
            out << std::setw( width ) <<
                   std::setfill( ' ' ) <<
                   std::fixed <<
                   std::setprecision( prec ) <<
                   list[i] << " " << std::flush;
    }
    out << " ]" << std::endl;
}

template <class T>
void printMatrix( T matrix, int w, int h, std::string label="matrix", bool abbreviate=false, int prec=2, int width=0, std::ostream &out=std::cout ){
    out << label << std::endl
        << "------------" << std::endl;
    for( int i=0; i<h; i++ ){
        if( abbreviate && i == 3 && h > 10 ){
            out << "..." << std::endl;
            i = h - 4;
        }
        printList( matrix + i * w, w, "|", abbreviate, prec, width, out );
    }
}

/** Converts a character to an integer
  * @param  a - The character to convert.  Must be in a 0-9 range.
  * @return The converted integer
  */
int char2Int( const char a );


/** Performs a division and modulo operation
  * @param  dividend  - The dividend   ( Numerator )
  * @param  divisor   - The divisor    ( Denominator )
  * @param  quotient  - The quotient   ( Result for division )
  * @param  remainder - The remeainder ( Result for modulo )
  */
void divmod( int dividend, int divisor, int &quotient, int &remainder );
