#include "Rules.h"

#include <iostream>
#include <stdexcept>

const vector< pair<int, int> > Rules::KNIGHT_MOVES = {
    {North, NorthWest}, {North, NorthEast},
    {South, SouthWest}, {South, SouthEast},
    {West, NorthWest}, {West, SouthWest},
    {East, NorthEast}, {East, SouthEast}
};


bool Rules::canInThisDirection(uint8_t square, int direction) {
    int file = square / LINE_SQUARES;
    int rank = square % LINE_SQUARES;

    //cout << "rank: " << rank << ", file " << file << endl;

    if ( file == 0 ) {
        if ( direction == South || direction == SouthEast || direction == SouthWest )
            return false;
    } else if ( file == (LINE_SQUARES - 1) ) {
        if ( direction == North || direction == NorthEast || direction == NorthWest )
            return false;
    }

    if ( rank == 0 ) {
        if ( direction == West || direction == NorthWest || direction == SouthWest )
            return false;
    } else if ( rank == (LINE_SQUARES - 1) ) {
        if ( direction == East || direction == NorthEast || direction == SouthEast )
            return false;
    }

    return true;
}


Rules::Rules( const Board & board ) : board(board) {
    setPieceInfos();

    // for better reading split into two cycles
    // at first for common direction offsets
    for ( uint8_t square = 0; square < SQUARES; square++ ) {

        int rank = square % LINE_SQUARES;
        int file = square / LINE_SQUARES;

        int numNorth = 7 - file;
        int numSouth = file;
        int numWest = rank;
        int numEast = 7 - rank;

        precomputedMaxMoves[square] = {
            {North, numNorth},
            {South, numSouth},
            {West, numWest},
            {East, numEast},
            {NorthWest, min(numNorth, numWest)},
            {SouthEast, min(numSouth, numEast)},
            {NorthEast, min(numNorth, numEast)},
            {SouthWest, min(numSouth, numWest)}
        };
    }

    // second - special exception combinative direction offsets for knight

    // next cycle used for determining if knight can move in that direction ("max moves")
    // so I will check it in case some rules are change so I now about it
    if ( whitePieceInfos[PieceType::Knight].infMoves == true )
        throw logic_error("Rules for knight piece changed, it is no longer true that knight can make only one move in that directions");

    for ( uint8_t square = 0; square < SQUARES; square++) {
        for ( const pair<int, int> & knightMove : KNIGHT_MOVES ) {
            int maxMoves = 0;

            int testingMaxMoves = precomputedMaxMoves[square][knightMove.first];
            if ( testingMaxMoves != 0 ) {

                int newSquare = square + knightMove.first;
                int testingMaxMoves = precomputedMaxMoves[newSquare][knightMove.second];

                if ( testingMaxMoves != 0 )
                    maxMoves = 1;
            }

            // sum directions for key
            int directionKey = knightMove.first + knightMove.second;
            precomputedMaxMoves[square].insert( {directionKey, maxMoves} );
        }
    }
}

// PRIVATE METHODS
void Rules::setPieceInfos() {
    whitePieceInfos = {
        Rules::PieceInfo{0},
        Rules::PieceInfo{PieceType::King, false, {North, South, West, East, NorthWest, NorthEast, SouthWest, SouthEast}},
        Rules::PieceInfo{PieceType::Pawn, false, {}},
        Rules::PieceInfo{PieceType::Knight, false, {
            North + NorthWest, North + NorthEast,
            South + SouthWest, South + SouthEast,
            West + NorthWest, West + SouthWest,
            East + NorthEast, East + SouthEast
        }},
        Rules::PieceInfo{PieceType::Bishop, true, {NorthWest, NorthEast, SouthWest, SouthEast}},
        Rules::PieceInfo{PieceType::Rook, true, {North, South, West, East}},
        Rules::PieceInfo{PieceType::Queen, true, {North, South, West, East, NorthWest, NorthEast, SouthWest, SouthEast}},
    };
    blackPieceInfos = whitePieceInfos;

    //set pawn direction offsets
    whitePieceInfos[PieceType::Pawn].directions = {North};
    blackPieceInfos[PieceType::Pawn].directions = {South};
}

const Rules::PieceInfo & Rules::getPieceInfo(Piece piece) const {
    const array<PieceInfo, PieceType::Max> & pieceTypes = piece.isWhite() ? whitePieceInfos : blackPieceInfos;

    return pieceTypes[piece.getPieceType()];
}


// PUBLIC METHODS

void Rules::getPossibleMoves(vector<Move> & possibleMoves, uint8_t startingSquare) const {
    UnknownName unknownName(*this, startingSquare);

    unknownName.getPossibleMoves(possibleMoves);
    unknownName.getSpecialMoves(possibleMoves);
}



// NESTED CLASS UnknownName  -------------------------------------------------
Rules::UnknownName::UnknownName(const Rules & supervisor, uint8_t startingSquare)
 :  supervisor(supervisor),
    startingSquare(startingSquare),
    pieceInfo(supervisor.getPieceInfo(supervisor.board[startingSquare])),
    maxMoves(supervisor.precomputedMaxMoves[startingSquare]) {

    Piece pieceOnBoard = supervisor.board[startingSquare];

    _piece = pieceOnBoard.getPieceType();
    _side = pieceOnBoard.getSide();

    _enemyLastRow = _side == White ? Rules::BLACK_STARTING_ROW : Rules::WHITE_STARTING_ROW;
}


void Rules::UnknownName::getPossibleMoves( vector<Move> & possibleMoves ) {
    for ( int offsetIndex = 0; offsetIndex < pieceInfo.directions.size(); offsetIndex++ ) {

        int directionOffset = pieceInfo.directions[offsetIndex];

        int movesInOffset = maxMoves.find(directionOffset)->second;
        if ( pieceInfo.infMoves != true && movesInOffset != 0 )
            movesInOffset = 1;

        uint8_t square = startingSquare;
        bool pieceInWay = false;
        for ( int i = 1; i <= movesInOffset && !pieceInWay; i++ ) {

            uint8_t newSquare = square + directionOffset * i;
            Piece pieceInNewSquare = supervisor.board[newSquare];

            if ( pieceInNewSquare != PieceType::None ) {    // piece in a way
                pieceInWay = true;

                if ( pieceInNewSquare.getSide() == _side || _piece == PieceType::Pawn )
                    break;

            }

            addMove(possibleMoves, startingSquare, newSquare);
        }
    }
}

void Rules::UnknownName::getSpecialMoves( vector<Move> & moves ) {
    if ( _piece == PieceType::Pawn ) {
        if ( pieceInfo.directions.size() != 1 )
            throw logic_error("Pawn should have set only one basic direction");

        int pawnDirection = pieceInfo.directions[0];

        int row = startingSquare / LINE_SQUARES;
        bool inStartingPos = ( row == 1 && _side == White ) || ( row == 6 && _side == Black );
        if ( inStartingPos ) {
            uint8_t beforeNewSquare = startingSquare + pawnDirection;
            uint8_t newSquare = startingSquare + pawnDirection * 2;

            if ( supervisor.board[beforeNewSquare] == PieceType::None && supervisor.board[newSquare] == PieceType::None )  // piece in a way
                moves.emplace_back(Move(startingSquare, newSquare));    // no need to call addMove - promotion can't happen

        }

        const size_t numberTakes = 2;
        int possibleTakes[numberTakes] = {pawnDirection + East, pawnDirection + West};
        for ( size_t i = 0; i < numberTakes; i++ ) {
            uint8_t positionOfTake = startingSquare + possibleTakes[i];

            if ( !Rules::canInThisDirection(startingSquare
, possibleTakes[i]) )
                continue;

            Piece targetPiece = supervisor.board[positionOfTake];
            if ( targetPiece.getPieceType() != PieceType::None && targetPiece.getSide() != _side )  // piece in a way
                addMove(moves, startingSquare, positionOfTake);
        }



    } else if ( _piece == PieceType::King ) {   // king and queen castle
        // check king position
        int defaultKingPos = _side == White ? 4 : 60;
        if ( startingSquare != defaultKingPos )
            return;

        uint8_t rightRookPos = _side == White ? 7 : 63;
        uint8_t leftRookPos = _side == White ? 0 : 56;

        Piece possLeftRook = supervisor.board[leftRookPos];
        Piece possRightRook = supervisor.board[rightRookPos];

        bool rightSquaresFree = true;
        bool leftSquaresFree = true;

        // check if squares by right rook are free
        int squaresBetween = startingSquare - leftRookPos - 1;
        uint8_t currentSquare = startingSquare - 1;
        for ( int i = 0; i < squaresBetween; i++ ) {
            if ( supervisor.board[currentSquare] != PieceType::None ) {
                leftSquaresFree = false;
                break;
            }
            currentSquare -= 1;
        }

        squaresBetween = rightRookPos - startingSquare - 1;
        currentSquare = startingSquare + 1;
        for ( int i = 0; i < squaresBetween; i++ ) {
            if ( supervisor.board[currentSquare] != PieceType::None ) {
                rightSquaresFree = false;
                break;
            }
            currentSquare += 1;
        }


        // check for left rook
        if ( possLeftRook.getPieceType() != PieceType::None && possLeftRook.getPieceType() == PieceType::Rook && possLeftRook.getSide() == _side && leftSquaresFree )
            moves.emplace_back( Move(startingSquare, leftRookPos+2, PieceType::None, false, false, true) );
        if ( possRightRook.getPieceType() != PieceType::None && possRightRook.getPieceType() == PieceType::Rook && possRightRook.getSide() == _side && rightSquaresFree )
            moves.emplace_back( Move(startingSquare, rightRookPos-1, PieceType::None, false, true, false) );
    }
}


void Rules::UnknownName::addMove( vector<Move> & moves, uint8_t from, uint8_t to ) {
    Piece targetSquarePiece = supervisor.board[to];
    bool isTake = (targetSquarePiece.getPieceType() != PieceType::None) && (_side != targetSquarePiece.getSide());

    int currentRow = to / LINE_SQUARES;
    bool isOnLastRow = _enemyLastRow == currentRow;

    if ( _piece == PieceType::Pawn && isOnLastRow ) {
        vector<piece_t> promotions = {PieceType::Knight, PieceType::Bishop, PieceType::Rook, PieceType::Queen};
        for ( piece_t promotion : promotions ) {
            moves.emplace_back( Move(from, to, promotion, isTake) );
        }
    } else
        moves.emplace_back( Move(from, to, PieceType::None, isTake) );
}
