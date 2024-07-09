#include "Chessboard.h"
#include "../engineCore/ChessDefinitions.h"
#include "../engineCore/chessLogic/Rules.h"

#include <stdexcept>
#include <algorithm>

const ChessColors Chessboard::DEFAULT_COLORS = ChessColors(Default_colors::BLANCHED_ALMOND, Default_colors::BURLY_WOOD);

Chessboard::Chessboard(SDL_Surface * surface, const ChessColors & colors, const Board & board) : surface(surface), colors(colors), size(surface->w), board(board) {
    if ( surface->w != surface->h )
        throw invalid_argument("Surface with does not match height");

    this->renderer = SDL_CreateSoftwareRenderer(surface);

}

void Chessboard::render() {
    const size_t square_size = this->size / chessboardSize::LINE_SQUARES;

    for (int file = 0; file < chessboardSize::LINE_SQUARES; file++) {
        for (int rank = 0; rank < chessboardSize::LINE_SQUARES; rank++) {
                bool isLight = (file + rank + 1) % 2 != 0;
                Color squareColor = isLight ? colors.light : colors.dark;

                uint8_t square = rank * chessboardSize::LINE_SQUARES + file;
                if ( find(allowedMoves.begin(), allowedMoves.end(), square) != allowedMoves.end() )
                    squareColor = colors.allowedMove;

                SDL_Rect rect;
                rect.x = file * square_size;
                rect.y = rank * square_size;
                rect.w = square_size;
                rect.h = square_size;
                SDL_SetRenderDrawColor(renderer, squareColor.r, squareColor.g, squareColor.b, 255);
                SDL_RenderFillRect(renderer, &rect);
        }
    }

    for ( size_t i = 0; i < board.n_squares; i++ ) {

        size_t rotatedIndex = getRotatedIndex(i);

        if ( board[rotatedIndex] == PieceType::None )
            continue;

        Piece piece = board[rotatedIndex];
        int sideTmp = piece.getSide();
        const map<piece_t, SDL_Surface *> & surfaces = (sideTmp == White) ? pieces[White] : pieces[Black];

        int pieceTmp = piece.getPieceType();
        SDL_Surface * pieceSurface = surfaces.find(pieceTmp)->second;
        SDL_Rect rect;

        int hightAlignment = (square_size - pieceSurface->h)/2;
        int widthAlignment = (square_size - pieceSurface->w)/2;

        rect.x = square_size * ( i % chessboardSize::LINE_SQUARES ) + hightAlignment;
        rect.y = square_size * ( i / chessboardSize::LINE_SQUARES ) + widthAlignment;

        rect.w = pieceSurface->w;
        rect.h = pieceSurface->h;
        SDL_BlitScaled(pieceSurface, NULL, surface, &rect);
    }
}


void Chessboard::setAllowedMoves(const vector<Move> & moves) {
    for ( const Move & move : moves )
        allowedMoves.push_back(getRotatedIndex(move.to));
}

uint8_t Chessboard::getRotatedIndex(uint8_t square) const {
    int row = square / chessboardSize::LINE_SQUARES;
    int column = square % chessboardSize::LINE_SQUARES;

    const int indexedLineSquares = chessboardSize::LINE_SQUARES - 1;
    return chessboardSize::LINE_SQUARES * (indexedLineSquares - row) + column;
}
