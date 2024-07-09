#include "ProgressShower.h"

// if linux os
#ifdef __linux__
    #include <sys/ioctl.h>
#endif

ProgressShower::ProgressShower() {
    #ifdef __linux__
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        _fullWidth = w.ws_col;
    #else
        _fullWidth = 100 + 2*2 + 3 + 1;
    #endif

    // subtract 4 for the brackets and 3 for two-digit number and 1 for the percent sign
    _progressWidth = _fullWidth - (2*2 + 3 + 1);
    _colsOnePercent = _progressWidth / 100.0;
}

/**
 * Displays the progress bar based on the current count and total count.
 *
 * @param currentCount The current count.
 * @param totalCount The total count.
 */
void ProgressShower::showProgress(long currentCount, long totalCount) {
    if (currentCount < 0 || totalCount < 0) {
        cerr << "Counts must be greater than 0" << endl;
        throw exception();
    }

    if (currentCount > totalCount) {
        cerr << "Current count must be less than total count" << endl;
        throw exception();
    }

    unsigned int percent = static_cast<double>(currentCount) / totalCount * 100;
    showProgress(percent);
}

/**
 * Displays the progress bar based on the given percentage.
 *
 * @param percent The progress percentage (between 0 and 100).
 */
void ProgressShower::showProgress(int percent) {
    if (percent < 0 || percent > 100) {
        cerr << "Percent must be between 0 and 100: " << percent << endl;
        throw exception();
    }

    // skip if the percent is the same as last time
    if (percent == lastPercent)
        return;


    int columnsProgress = percent * _colsOnePercent;
    int columnsEmpty = _progressWidth - columnsProgress;

    // if (percent == 100) {
    //     columnsProgress = _progressWidth;
    //     columnsEmpty = 0;
    // }


    string progress = "[" + string(columnsProgress, PROGRESS_CHAR) + string(columnsEmpty, ' ') + "]";

    string percentString = to_string(percent);
    percentString += string(3 - percentString.length(), ' ');
    string totalString = "[" + percentString + "%]" + progress;
    cout << "\r" << totalString << flush;

    lastPercent = percent;
}

/**
 * Clears the progress bar from the console.
 */
void ProgressShower::clear() {
    cout << "\r" << string(_fullWidth, ' ') << flush;
    lastPercent = 0;
}

