#pragma once

#include <string>
#include <locale>
#include <codecvt>

namespace boxDrwaing {
    inline std::string LIGHT_HORIZONTAL = "─";
    inline std::string LIGHT_VERTICAL = "│";
    inline std::string LIGHT_DOWN_AND_RIGHT = "╭";
    inline std::string LIGHT_DOWN_AND_LEFT = "╮";
    inline std::string LIGHT_UP_AND_LEFT = "╯";
    inline std::string LIGHT_UP_AND_RIGHT = "╰";
    inline std::string LIGHT_DIAGONAL_UPP_RIGHT_TO_LOW_LEFT = "╱";
    inline std::string LIGHT_DIAGONAL_UPP_LFT_TO_LOW_RIGHT = "╲";

    void AddMultipleChars( std::string & str, const std::string & chars, size_t count );
    size_t RealUtf8Length( const std::string & str );

};
