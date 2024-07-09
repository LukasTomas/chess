#pragma once

#include "Color.h"

#include <iostream>

namespace logging {

    inline std::string NOTICE_COLOR = color::RGB(0, 255, 0);         // Green
    inline std::string INFO_COLOR = color::RGB(0, 191, 255);         // Light Blue
    inline std::string WARNING_COLOR = color::RGB(255, 165, 0);      // Orange
    inline std::string ERROR_COLOR = color::RGB(255, 0, 0);          // Red

    void LogColor(const std::string text, const std::string color, bool toCerr = false);

    void LogNotice(const std::string & text);
    void LogInfo(const std::string & text);
    void LogWarning(const std::string & text);
    void LogError(const std::string & text);

    // define co i can call infoLogger <<

}


#define LOG_INFO Log(logging::INFO_COLOR, "INFO").Get()
#define LOG_NOTICE Log(logging::NOTICE_COLOR, "NOTICE").Get()
#define LOG_WARNING Log(logging::WARNING_COLOR, "WARNING").Get()
#define LOG_ERROR Log(logging::ERROR_COLOR, "ERROR", true).Get()

class Log {
public:

    Log( const std::string & color, const std::string & name, bool isCerr = false );
    ~Log();

    std::ostream & Get();

private:

    const std::string color;
    const std::string name;
    const bool isCerr;

    bool showName = false;

    std::ostream & os = isCerr ? std::cerr : std::cout;
};

