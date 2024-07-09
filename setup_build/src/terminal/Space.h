#pragma once

#include "Coors.h"
#include "Color.h"

#include <iostream>
using namespace std;

class Space {
public:

    static const int FPS = 30;

    static const string DEFAULT_COLOR;
    static const string BLOCK;

    Space();
    ~Space();

    void Info();

    void Render( bool showAxis = false);
    void Clean();

    void RandomBlock( int probability=20 );
    void Line( const coors & from, const coors & to );
    void Rectangle( const coors & from, const coors & to );
    void TerminalSquare( const coors & from, size_t size );

    void Animate();

    int GetWidth() const;
    int GetHeight() const;

private:

    // ratio between terminal width and height
    static const int TERMINAL_WIDTH_HEIGHT_RATIO = 2;


    int _Digits( int number);
    void _ShowXAxis( int every );

    static const string lineUp;
    int _width, _height;

    string **_space;
};
