#pragma once

#include <iostream>

class ConstraintGameData {
public:

    size_t _size = 0;
    int * _data = nullptr;

    ConstraintGameData() {}

    ConstraintGameData( size_t size ) {
        init(size);
    }

    ~ConstraintGameData() {
        delete[] _data;
    }

    std::istream & Load( std::istream & is, size_t size ) {
        delete _data;
        init(size);

        for ( int i = 0; i < size; i++ )
            is.read(reinterpret_cast<char*>(&_data[i]), sizeof(_data[i]));

        return is;
    }

private:

    void init( size_t size ) {
        _size = size;
        _data = new int[size];
    }

};
