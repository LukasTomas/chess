// 0.

#include "../../src/dataset/parsing/SanParser.h"
#include "../../src/terminal/Logging.h"

#include <iostream>
using namespace std;

void showHelp() {
    LOG_INFO << "Usage: ParsePgn" << endl;
}

int main( int argc, char* argv[]) {
    if ( argc != 1 ) {
        LOG_ERROR << "Invalid number of arguments: " << (argc-1) << endl;
        showHelp();
        return 1;
    }

    SanParser s;
    s.parseAll();
    return 0;
}
