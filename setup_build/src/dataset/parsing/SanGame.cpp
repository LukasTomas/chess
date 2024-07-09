#include "SanGame.h"

#include <ctype.h>


bool SanGame::makeMove( string san ) {
    //cout << "\t|" << san << "|" << endl;
    string printSan = san;
    //cout << printSan;


    bool isCheck = false;
    bool isCapture = false;
    piece_t promotion = None;

    char lastC = san[san.length()-1];
    // !, !!, ??, ?!, !?
    for ( int i = 0; i < 2; i++ ) {
        if ( lastC == '?' || lastC == '!' ) {
            san.pop_back();
            lastC = san[san.length()-1];
        } else
            break;
    }

    lastC = san[san.length()-1];
    isCheck = lastC == '+';
    // erase last character if check or checkmate - Bxg6+ -> Bxg6
    if ( lastC == '+' || lastC == '#' )
        san.pop_back();

    if ( san == "O-O" || san == "O-O-O" ) {
        //cout << endl;
        makeCastle(san == "O-O");
        //cout << "========== castle" << endl;
        return true;
    }

    piece_t piece = letterToPiece(san[0]);
    //erase first character - Bxg6 -> xg6
    if ( piece != Pawn )
        san.erase(0, 1);
    else {      // possible pawn promotion
        // can look like now e8=Q
        if ( san[san.size()-2] == '=' ) {
            promotion = letterToPiece(san[san.size()-1]) | game.sidePlaying();
            san.pop_back();
            san.pop_back();
        }
    }

    uint8_t to = toCoordinates(san.substr(san.length() - 2));
    uint8_t from = -1;
    // erase last two characters that denotes destiantion sqaure - xg6 -> x
    san.pop_back();
    san.pop_back();

    // erase 'x' caputere character if is there
    if ( san.length() != 0 && san[san.length()-1] == 'x' ) {
        isCapture = true;
        san.pop_back();
    }

    char column = ' ';
    if ( san.length() != 0 ) {
        if ( isCapture == true && piece == Pawn ) {
            //cout << "============== pawn take" << endl;
            column = san[san.length()-1];
            san.pop_back();

            if ( game.getBoard()[to] == None ) {
                return false;
            }
        } else {    // disambiguating move
            //cout << "=========== Disambiguating move " << endl;
            if ( san.size() == 2 ) {    // exact position
                from = toCoordinates(san);
                san.pop_back();
                san.pop_back();
            } else {                     // only column
                column = san[san.length()-1];
                san.pop_back();
            }
        }
    }

    if ( from == (uint8_t)-1 )
        from = getFromSquare(piece, to, column);

    //cout <<  " - " << Piece::getName(piece) << ", from " << (int)from << " - to " << (int)to << endl;
    if ( san != "" ) {
        //cout << " ------------------------------- zbyva '" << san << "'" << endl;
        throw logic_error("san not empty");
    }

    Move move(from, to, promotion, isCapture);
    bool made = game.doMove(move);
    if ( !made )
        throw logic_error("couldn't make this move");
    return true;
}

void SanGame::makeCastle(bool isKingCastle) {
    vector<Move> moves;
    game.getPseudoLegalMoves(moves, King) ;
    for ( Move & m : moves ) {
        if ( (m.isKingCastle && isKingCastle) || (m.isQueenCastle && !isKingCastle) ) {
            bool made = game.doMove(m);
            if ( !made )
                throw logic_error("couldn't make castle");
            break;
        }
    }
}

int SanGame::getColumnNumber(char columnLetter) {
    //cout << "column letter: |" << columnLetter << "|" << endl;
    if ( columnLetter < 'a' || columnLetter > 'h' )
        throw invalid_argument("invalid column letter: " + columnLetter);

    return 7 - ('h' - columnLetter);
}

uint8_t SanGame::toCoordinates( string san ) {
    if ( san.length() != 2 )
        throw invalid_argument("cannot convert non-2-length san sqaure to coordination");

    char columnLetter = san[0];
    int column = getColumnNumber(columnLetter);

    int row = san[1] - '0' - 1;
    return row * 8 + column;
}

piece_t SanGame::letterToPiece( char letter ) {
    auto it = PIECE_LETTER.find(letter);
    if ( it == PIECE_LETTER.end() )
        return Pawn;
    return it->second;
}

uint8_t SanGame::getFromSquare(piece_t piece, uint8_t to, char columnLetter) {
    uint8_t from = -1;
    const Game::PieceInfo & pieceInfo = game.getPlayingPieceInfo(piece);

    if ( columnLetter != ' ' ) {
        //cout << "|" <<  columnLetter << "|" << endl;

        bool isRow = false;
        if (isdigit(columnLetter)) // is not column, but ROW
            isRow = true;

        int columnOrRow = isRow ? ((columnLetter - '0') - 1) : getColumnNumber(columnLetter);

        for ( uint8_t piecePosition : pieceInfo.positions ) {      // for every piece of this type
            int pieceColumn = piecePosition % 8;
            int pieceRow = piecePosition / 8;

            if ( (!isRow && pieceColumn == columnOrRow) || (isRow && pieceRow == columnOrRow) ) {
                // previous mistake right away return - there can be two pawns on same column -> check for possible positions
                vector<Move> moves;
                game.getPseudoLegalMovesFromSquare(moves, piecePosition);
                for ( Move & m : moves ) {
                    if ( m.to == to )
                        return piecePosition;
                }
            }
        }

        throw logic_error("No piece of this type is on such column");
    }

    vector<Move> moves;
    //game.getPseudoLegalMoves(moves, piece) ;
    game.getLegalMoves(moves, piece) ;
    for ( Move & m : moves ) {
        if ( m.to == to )
            return m.from;
    }

    throw logic_error("No piece of this type can move to such position");
}
