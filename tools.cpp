#include "tools.h"

using namespace std;

#ifdef EXEC_TRACE
deque<string> exec_stack;
#endif

int char2Int( char a ){
    ENTER;
    int i;
    ASSERT_SET( i, (int)(a-'0'),
                a >= '0' && a <= '9', "Character must be a numeral" );
    RETURN i;
}

void divmod( int dividend, int divisor, int &quotient, int &remainder ){
    quotient = dividend / divisor;
    remainder = dividend - divisor * quotient;
}
