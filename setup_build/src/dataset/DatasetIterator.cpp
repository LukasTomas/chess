#include "DatasetIterator.h"
#include "parsing/ConstraintGameData.h"

#include "../common/BinaryFile.h"

using namespace std;

// CONSTRUCTOR -------------------------------------------------------------
template <class T>
DatasetIterator<T>::DatasetIterator( const string & filepath ) : input(filepath, ios::binary) {
    // check if file exists
    if( !binfile::FileExists(filepath) )
        throw invalid_argument("File " + filepath + " does not exist");

    _areConstraints = false;
    if ( filepath.find(".constraints") != string::npos ) {
        _areConstraints = true;

        // Load header
        int constrainsCount;
        input.read(reinterpret_cast<char*>(&constrainsCount), sizeof(constrainsCount));

        for ( int i = 0; i < constrainsCount; i++ ) {
            Constraint constraint;
            if ( !(constraint.load(input)) )
                throw invalid_argument("Invalid constraint file");

            if ( find(constrains.begin(), constrains.end(), constraint) == constrains.end() )
                throw invalid_argument("Constraint " + constraint.toString() + " not found");

            _constraints.push_back(constraint);
        }
    }

    // Try to load the first game in order to check if there is any, then reset the stream
    _firstGameStreamPos = input.tellg();
    if (currentAdditionalData->Load(input, _constraints.size()) && currentGame->load(input) )
        _isThereFirstGame = true;

    input.seekg(_firstGameStreamPos);
}

template <class T>
DatasetIterator<T>::~DatasetIterator() {
    delete currentGame;
    delete currentAdditionalData;
}

// PUBLIC ------------------------------------------------------------------
template <class T>
ParsedGame * DatasetIterator<T>::LoadNext() {
    if ( currentAdditionalData->Load(input, _constraints.size()) && currentGame->load(input) ) {
        gamesCount++;
        return currentGame;
    }
    return nullptr;
}

template <class T>
void DatasetIterator<T>::Reset() {
    input.clear();
    input.seekg(_firstGameStreamPos);
}

template <class T>
bool DatasetIterator<T>::IsEnd() const {
    if ( gamesCount == 0 )
        return !_isThereFirstGame;

    return currentGame == nullptr;
}

template <class T>
bool DatasetIterator<T>::AreConstraints() const {
    return _areConstraints;
}

template <class T>
const vector<Constraint> & DatasetIterator<T>::GetConstraints() const {
    return _constraints;
}


// Explicit instantiation of the template class
template class DatasetIterator<ConstraintGameData>;
