#pragma once

#include "Helper.h"

#include "../../src/terminal/Space.h"
#include "../../src/terminal/ProgressShower.h"
#include "../../src/terminal/Logging.h"
#include "../../src/terminal/DrawingChars.h"

#include <chrono>
#include <iostream>
#include <vector>

class Program {
public:

    static constexpr double FPS = 0.3;

    Helper helper;


    static std::string GetStringTime( int64_t milliseconds );

    Program( int argc, char * argv[], const string & description );
    ~Program();

    void Start();

private:

    std::string _programName;
    std::string _description;

    std::vector<std::string> _arguments;


    chrono::time_point<std::chrono::high_resolution_clock> _startTime;

    Space space;

    int64_t GetDuration();

    void Render();
};
