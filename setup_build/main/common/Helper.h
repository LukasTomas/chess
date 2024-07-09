#pragma once

#include <string>
#include <vector>

class Helper {
public:

    static const int TAB_SIZE = 4;

    Helper( const std::string & name, const std::string & description );

    void AddOption( const std::string & option, const std::string & description, bool required = false );
    void ShowHelp() const;
    void ShowStart() const;
    void Check( const std::vector<std::string> & arguments ) const;

private:

    struct option {
        std::string name;
        std::string description;
        bool required;
    };

    std::string _name, _description;
    std::vector<option> options;
    int requiredOptions = 0;
    int realOptions = 0;        // without help

    int longestOption = 0;

};
