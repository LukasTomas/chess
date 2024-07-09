#include "GraphicPieces.h"

#include "../engineCore/ChessDefinitions.h"

#include <stdexcept>

const string GraphicPieces::SRC_DIR_NAME = "setup_build";
const string GraphicPieces::PIECES_DIR_NAME = "pieces";

const  pair<side_t, string> GraphicPieces::SIDE_NAMES[2] = {
    make_pair(Side::White, "l"),
    make_pair(Side::Black, "b")
};



GraphicPieces::GraphicPieces() {
    path path = getPiecesDirPath();
    cout << "found path: " << path << endl;

    PieceIterator pieceIterator;
    do {
        piece_t current = pieceIterator.current;
        string piecePath = path.string() + "/" + PIECE_NAMES.at(current);

        for ( const pair<side_t, string> & sideName : SIDE_NAMES ) {
            string sidePiecePath = piecePath + "_" + sideName.second + ".png";
            SDL_Surface * imgSurface = IMG_Load(sidePiecePath.c_str());

            pair<piece_t, SDL_Surface *> pieceSurfaceEl = {current, imgSurface};
            switch ( sideName.first ) {
                case Side::White:
                    whitePieces.insert(pieceSurfaceEl);
                    break;
                case Side::Black:
                    blackPieces.insert(pieceSurfaceEl);
                    break;
            }
        }
    } while( pieceIterator.next() );

}

const map<piece_t, SDL_Surface *> & GraphicPieces::operator[](side_t side) const {
    if ( side == Side::White )
        return whitePieces;
    else
        return blackPieces;
}

path GraphicPieces::getPiecesDirPath() const {
    path currentDir = current_path();
    path originalDir = currentDir;    // for exception info
    while ( currentDir.filename() != SRC_DIR_NAME ) {
        cout << currentDir.filename() << endl;
        currentDir = currentDir.parent_path();

        cout << currentDir.string() << endl;
        if ( currentDir.string() == "/" )
            throw runtime_error("Path " + originalDir.string() + " doesn't contain directory '" + SRC_DIR_NAME + "'");
    }

    path projectRootPath = currentDir.parent_path();
    return projectRootPath / PIECES_DIR_NAME;
}
