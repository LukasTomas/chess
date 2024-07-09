#include "Color.h"

using namespace std;

string color::RGB( uint8_t r, uint8_t g, uint8_t b ) {
    return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
}

string color::BG_RGB( uint8_t r, uint8_t g, uint8_t b ) {
    return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
}

string color::ColoroziedString( string text, string color ) {
    return color + text + RESET;
}
