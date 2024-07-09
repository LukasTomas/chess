#include "Program.h"

using namespace std;

// STATIC METHODS -----------------------------------------------------------------------------------------------------

string Program::GetStringTime( int64_t milliseconds ) {
    int seconds = milliseconds / 1000;
    string time;

    int64_t hours = seconds / 3600;
    seconds -= hours * 3600;
    if ( hours > 0 )
        time += to_string(hours) + "h";

    int64_t minutes = seconds / 60;
    seconds -= minutes * 60;
    if ( minutes > 0 ) {
        if ( time.length() != 0 )
            time += " ";
        time += to_string(minutes) + "m";
    }

    if ( seconds > 0 ) {
        if ( time.length() != 0 )
            time += " ";
        time += to_string(seconds) + "s";
    }

    return time;
}

// CONSTRUCTORS -------------------------------------------------------------------------------------------------------
Program::Program( int argc, char * argv[], const string & description ) : helper(argv[0], description) {
    _programName = argv[0];
    _description = description;

    for ( int i = 1; i < argc; i++ ) {
        if ( argv[i][0] == '-')
            _arguments.push_back(string(argv[i]));
    }

    _startTime = chrono::high_resolution_clock::now();
}

Program::~Program() {
    int64_t duration = GetDuration();
    cout << "Program finished in " << GetStringTime(duration) << endl;
}


// PUBLIC METHODS -----------------------------------------------------------------------------------------------------
void Program::Start() {
    helper.ShowStart();
    helper.Check(_arguments);

    // while ( !stopFlag ) {
    //     Render();
    //     this_thread::sleep_for(chrono::milliseconds((int)(1000 * FPS)));
    // }
}


// PRIVATE METHODS ----------------------------------------------------------------------------------------------------

int64_t Program::GetDuration() {
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - _startTime).count();
    return duration;
}

void Program::Render() {
    const size_t nameSize = _programName.length();
    int widthRemains = space.GetWidth() - nameSize - 2; // 2 for spaces
    int leftWidth = widthRemains / 2;
    int rightWidth = widthRemains - leftWidth;

    string duration = GetStringTime(GetDuration());
    rightWidth -= duration.length() + 1; // 1 for space

    string firstLine;
    boxDrwaing::AddMultipleChars(firstLine, boxDrwaing::LIGHT_HORIZONTAL, leftWidth);
    firstLine += " " + _programName + " ";
    boxDrwaing::AddMultipleChars(firstLine, boxDrwaing::LIGHT_HORIZONTAL, rightWidth);
    firstLine += " " + duration;

    LOG_NOTICE << firstLine << endl;
}
