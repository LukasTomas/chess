#pragma once

#include <fstream>
#include <sys/stat.h>

namespace binfile {

    inline void SaveString( std::ostream & os, const std::string & s ) {
        // first save the size of the string
        const size_t size = s.size();
        os.write(reinterpret_cast<const char*>(&size), sizeof(size));
        os.write(s.c_str(), size);
    }

    inline void LoadString( std::istream & is, std::string & s ) {
        // first load the size of the string
        size_t size;
        is.read(reinterpret_cast<char*>(&size), sizeof(size));
        s.resize(size);
        is.read(&s[0], size);
    }

    inline bool FileExists (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }

};
