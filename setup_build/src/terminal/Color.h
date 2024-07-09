#pragma once

#include <string>

namespace color {
    inline std::string RESET = "\033[00m";

    // FOREGROUND COLOR
    inline std::string GREY = "\033[30m";
    inline std::string RED = "\033[31m";
    inline std::string GREEN = "\033[32m";
    inline std::string YELLOW = "\033[33m";
    inline std::string BLUE = "\033[34m";
    inline std::string MAGENTA = "\033[35m";
    inline std::string CYAN = "\033[36m";
    inline std::string WHITE = "\033[37m";

    inline std::string BRIGHT_GREY = "\033[90m";
    inline std::string BRIGHT_RED = "\033[91m";
    inline std::string BRIGHT_GREEN = "\033[92m";
    inline std::string BRIGHT_YELLOW = "\033[93m";
    inline std::string BRIGHT_BLUE = "\033[94m";
    inline std::string BRIGHT_MAGENTA = "\033[95m";
    inline std::string BRIGHT_CYAN = "\033[96m";
    inline std::string BRIGHT_WHITE = "\033[97m";

    // RGB COLOR \033[38;2;<r>;<g>;<b>m
    std::string RGB( uint8_t r, uint8_t g, uint8_t b );

    // BACKGROUND COLOR
    inline std::string BG_GREY = "\033[40m";
    inline std::string BG_RED = "\033[41m";
    inline std::string BG_GREEN = "\033[42m";
    inline std::string BG_YELLOW = "\033[43m";
    inline std::string BG_BLUE = "\033[44m";
    inline std::string BG_MAGENTA = "\033[45m";
    inline std::string BG_CYAN = "\033[46m";
    inline std::string BG_WHITE = "\033[47m";

    inline std::string BG_BRIGHT_GREY = "\033[100m";
    inline std::string BG_BRIGHT_RED = "\033[101m";
    inline std::string BG_BRIGHT_GREEN = "\033[102m";
    inline std::string BG_BRIGHT_YELLOW = "\033[103m";
    inline std::string BG_BRIGHT_BLUE = "\033[104m";
    inline std::string BG_BRIGHT_MAGENTA = "\033[105m";
    inline std::string BG_BRIGHT_CYAN = "\033[106m";
    inline std::string BG_BRIGHT_WHITE = "\033[107m";

    // RGB COLOR \033[48;2;<r>;<g>;<b>m
    std::string BG_RGB( uint8_t r, uint8_t g, uint8_t b );

    std::string ColoroziedString( std::string text, std::string color );
}

