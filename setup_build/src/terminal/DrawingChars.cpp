#include "DrawingChars.h"
using namespace std;
void boxDrwaing::AddMultipleChars( std::string & str, const std::string & chars, size_t count ) {
    for ( size_t i = 0; i < count; i++ )
        str += chars;
}

size_t boxDrwaing::RealUtf8Length( const std::string & str ) {
    size_t length = wstring_convert< codecvt_utf8<char32_t>, char32_t >().from_bytes(str).size();
    return length;
}
