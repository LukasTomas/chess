#pragma once

#include <iostream>
#include <unistd.h>
using namespace std;

class ProgressShower {
public:

    static const char PROGRESS_CHAR = '#';

    ProgressShower();

    void showProgress( long currentCount, long totalCount );
    void showProgress( int percent );

    void clear();

private:
    unsigned short _fullWidth;
    unsigned short _progressWidth;
    double _colsOnePercent;

    int lastPercent = 0;
};

