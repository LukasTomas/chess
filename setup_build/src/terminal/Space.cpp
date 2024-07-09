#include "Space.h"

#include <sstream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <random>
#include <ctime>

#include <map>
#include "DrawingChars.h"
#include "Vector.h"
#include "UnitCircle.h"
#include "LineQuadrant.h"


Space::Space() {
    srand(time(NULL));

    struct winsize windowSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize);

    _width = windowSize.ws_col;
    _height = windowSize.ws_row;

    _space = new string*[_width];
    for ( int i = 0; i < _width; i++ )
        _space[i] = new string[_height];
}

Space::~Space() {
    for ( int i = 0; i < _width; i++ )
        delete[] _space[i];
    delete[] _space;
}

// PUBLIC ----------------------------------------------------------------------

const string Space::DEFAULT_COLOR = color::BRIGHT_WHITE;

// Empty char with background set looks like fuller block, better than utf-8 block character "\u25A0"
const string Space::BLOCK = color::ColoroziedString(" ", color::BG_RED);


void Space::Info() {
    cout << _height << "x" << _width << endl;
}

void Space::Render( bool showAxis) {
    for ( int y = 0; y < _height; y++ )
        cout << lineUp;

    const int showEvery = 10;

    for ( int y = 0; y < _height-1; y++ ) {
        if ( showAxis && y == 0 ) {
            _ShowXAxis(showEvery);
            continue;
        }

        int noSpaceTimes = 0;
        for ( int x = 0; x < _width; x++ ) {

            int biggestNumberEnd = _Digits(_height-1) - 1;
            if ( showAxis) {
                int axisNumber = y - 1;
                int digits = _Digits(axisNumber);

                if ( x == 0 && axisNumber % showEvery == 0 ) {
                    cout << axisNumber;
                    noSpaceTimes = digits - 1;
                } else if ( x <= biggestNumberEnd) {
                    if ( noSpaceTimes > 0 ) {
                        cout << "";
                        noSpaceTimes--;
                    } else
                        cout << " ";
                } else
                    cout << _space[x-2][y-1];
            } else
                cout << _space[x][y];
        }
        cout << endl;
    }
}

void Space::Clean() {
    for ( int y = 0; y < _height; y++ ) {
        for ( int x = 0; x < _width; x++ ) {

            _space[x][y] = " ";
        }
    }
}


//-------------------------------------------------------------------------------

void Space::RandomBlock( int probability ) {
    for ( int y = 0; y < _height; y++ ) {
        for ( int x = 0; x < _width; x++ ) {

            int random = rand() % 100;

            if ( random <= probability ) {
                _space[x][y] = BLOCK;
            } else
                _space[x][y] = " ";
        }
    }
}

/*class Line {
public:

    void Exist( const coors & from, const coors & to ) {
        int x = from.x;
        int y = from.y;

        int dx = to.x - from.x;
        int dy = to.y - from.y;

        int steps = max(abs(dx), abs(dy));

        float xIncrement = dx / (float) steps;
        float yIncrement = dy / (float) steps;

        // also equal because we have to add + 1 because we start from 0
        coors current = from;
        for ( int v = 1; v <= steps; v++ ) {

            int _x = x + v*xIncrement;
            int _y = y + v*yIncrement;
            coors next = {_x, _y};

            Vector vector(current, next, true);
            _space[_x][_y] = lineVectorToChar.at(vector.Get());
        }
    }


private:

    const map<coors, string> lineVectorToChar = {
        {{1, 0}, boxDrwaing::LIGHT_HORIZONTAL},
        {{1, 1}, boxDrwaing::LIGHT_DIAGONAL_UPP_RIGHT_TO_LOW_LEFT},
        {{0, 1}, boxDrwaing::LIGHT_VERTICAL},
        {{-1, 1}, boxDrwaing::LIGHT_DIAGONAL_UPP_LEFT_TO_LOW_RIGHT},
        {{-1, 0}, boxDrwaing::LIGHT_HORIZONTAL},
        {{-1, -1}, boxDrwaing::LIGHT_DIAGONAL_UPP_RIGHT_TO_LOW_LEFT},
        {{0, -1}, boxDrwaing::LIGHT_VERTICAL},
        {{1, -1}, boxDrwaing::LIGHT_DIAGONAL_UPP_LEFT_TO_LOW_RIGHT}
    };

};*/


// doprovodny vzdy v y
UnitCircle mainUnitCircle({
    {Vector(1, 0), boxDrwaing::LIGHT_HORIZONTAL},
    {Vector(1, 1), boxDrwaing::LIGHT_DOWN_AND_LEFT},
    {Vector(0, 1), boxDrwaing::LIGHT_VERTICAL},
    {Vector(-1, 1), boxDrwaing::LIGHT_DOWN_AND_RIGHT},
    {Vector(-1, 0), boxDrwaing::LIGHT_HORIZONTAL},
    {Vector(-1, -1), boxDrwaing::LIGHT_UP_AND_RIGHT},
    {Vector(0, -1), boxDrwaing::LIGHT_VERTICAL},
    {Vector(1, -1), boxDrwaing::LIGHT_UP_AND_LEFT}
});

UnitCircle minorUnitCircle({
    {Vector(1, 0), boxDrwaing::LIGHT_HORIZONTAL},
    {Vector(1, 1), boxDrwaing::LIGHT_UP_AND_RIGHT},
    {Vector(0, 1), boxDrwaing::LIGHT_VERTICAL},
    {Vector(-1, 1), boxDrwaing::LIGHT_UP_AND_LEFT},
    {Vector(-1, 0), boxDrwaing::LIGHT_HORIZONTAL},
    {Vector(-1, -1), boxDrwaing::LIGHT_DOWN_AND_LEFT},
    {Vector(0, -1), boxDrwaing::LIGHT_VERTICAL},
    {Vector(1, -1), boxDrwaing::LIGHT_DOWN_AND_RIGHT}
});

/* Tells for diagonal line whether to go:
                      ╰╮     ╮              ╰╮                          ╮

*/
const

//ahojjaksedari     pridat dalsi dva jednotkove kruhy pro 4. case

bool IsVectorDiagonal( Vector & vector ) {
    int absX = abs(vector.x);
    int absY = abs(vector.y);
    return absX == absY;
}

void Space::Line( const coors & from, const coors & to ) {
    //mainUnitCircle.Invert('y');

    if ( from == to )
        throw invalid_argument("From and to coordinates are the same");
    if ( from.x < 0 || from.x >= _width || from.y < 0 || from.y >= _height )
        throw out_of_range("'From' coordinates out of range: " /*+ from.ToString()*/);
    if ( to.x < 0 || to.x >= _width || to.y < 0 || to.y >= _height )
        throw out_of_range("'To' coordinates out of range: " /*+ to.ToString()*/);

    int x = from.x;
    int y = from.y;

    int dx = to.x - from.x;
    int dy = to.y - from.y;

    int steps = max(abs(dx), abs(dy));
    float xIncrement = dx / (float) steps;
    float yIncrement = dy / (float) steps;

    // also equal because we have to add + 1 because we start from 0
    coors current = from;
    for ( int v = 1; v <= steps; v++ ) {
        int _x = x + v*xIncrement;
        int _y = y + v*yIncrement;
        coors next = {_x, _y};

        Vector vector(current, next);
        //cout << "vector: " << vector.Get() << endl;
        string charToDraw = mainUnitCircle.GetByVector(vector);
        _space[current.x][current.y] = charToDraw;

        if ( IsVectorDiagonal(vector) ) {
            //doprovodny jsou vzdy v y
            int plusY = vector.y;
            _space[current.x][current.y + plusY] = minorUnitCircle.GetByVector(vector);
        }

        current = next;
    }


}

void Space::Rectangle( const coors & from, const coors & to ) {
    Line(from, {from.x, to.y});
    Line({from.x, to.y}, to);
    Line(to, {to.x, from.y});
    Line({to.x, from.y}, from);
}

void Space::TerminalSquare( const coors & from, size_t size ) {
    int x = from.x + TERMINAL_WIDTH_HEIGHT_RATIO*size;
    int y = from.y + size;

    Rectangle(from, {x, y});
}

void Space::Animate() {
    const int fps = 60;
    int frameTime = 1000000/fps;

    coors start = {0, 20};
    int totalSeconds = 1;
    double steps = 0.15 * fps;
    double stepSize = (_width-1) / (double)steps;

    for ( int i = 1; i <= steps; i++ ) {
        Clean();
        if ( i == steps ) // last step
            Line(start, {_width-1, start.y});
        else
            Line(start, {i*stepSize, start.y});
        Render();
        usleep(frameTime);
    }

    // Spell Impact
    start = {_width-1, 20};
    coors start2 = {_width-1, 20};
    for ( int i = 1; i <= 10; i++ ) {
        Clean();

        bool incrementX = rand() % 2;
        if ( incrementX )
            start.x--;
        else
            start.y++;

        _space[start.x][start.y] = BLOCK;

        incrementX = rand() % 2;
        if ( incrementX )
            start2.x--;
        else
            start2.y--;

        _space[start2.x][start2.y] = BLOCK;

        Render();
        usleep(frameTime*6);
    }
}

int Space::GetWidth() const {
    return _width;
}

int Space::GetHeight() const {
    return _height;
}

// PRIVATE ---------------------------------------------------------------------

const string Space::lineUp = "\033[A";

int Space::_Digits( int number) {
    int digits = 1;

    while ( number > 9 ) {
        number /= 10;
        digits++;
    }

    return digits;
}

void Space::_ShowXAxis( int every) {
    // first x axis
    int noSpaceTimes = 0;
    cout << " " << " ";

    for ( int x = 0; x < _width-2; x++ ) {

        if ( x % every == 0 ) {
            int digits = _Digits(x);

            // check if the last number fits in the space
            if ( x + (digits - 1) >= _width )
                cout << " ";
            else {
                noSpaceTimes = digits - 1;
                cout << x;
            }
        } else {
            if ( noSpaceTimes > 0 )
                noSpaceTimes--;
            else
                cout << " ";
        }
    }
}

// int main() {
//     LineQuadrant("a", "b", "c", "d", "e").Print();

//     // Space space;

//     // space.Clean();
//     // space.Line({0, 0}, {10, 0});
//     // space.Line({0, 2}, {0, 10});


//     // space.Line({20, 0}, {30, 10});
//     // space.Line({30, 12}, {20, 22});

//     // space.Line({40, 0}, {60, 10});
//     // space.Line({60, 12}, {40, 22});

//     // space.Line({70, 0}, {80, 20});
//     // space.Line({80, 22}, {70, 42});

//     // space.Render(true);

//     // int microsecondsOne = 1000000 / Space::FPS;
//     // for ( int i = 0; i < 30; i ++ ) {
//     //     if ( i % 2 == 0 )
//     //         space.Clean();
//     //     else
//     //         space.RandomBlock();
//     //     space.Render(true);
//     //     usleep(microsecondsOne);
//     // }


//     return 0;
// }
