#include "Logging.h"

using namespace std;

void logging::LogColor(const string text, const string color, bool toCerr) {
    string colorizedText = color::ColoroziedString(text, color);
    if ( toCerr )
        cerr << colorizedText << endl;
    else
        cout << colorizedText << endl;
}

void logging::LogNotice(const string & text) {
    LogColor(text, logging::NOTICE_COLOR);
}

void logging::LogInfo(const string & text) {
    LogColor(text, logging::INFO_COLOR);
}

void logging::LogWarning(const string & text) {
    LogColor(text, logging::WARNING_COLOR);
}

void logging::LogError(const string & text) {
    LogColor(text, logging::ERROR_COLOR, true);
}


// Logger ----------------------------------------------------------------------

Log::Log( const string & color, const string & name, bool isCerr ) : color(color), name(name), isCerr(isCerr) {}

Log::~Log() {
    os << color::RESET << flush;
}

ostream & Log::Get() {
    os << color;
    if ( showName )
        os << name << ": ";
    return os;
}

