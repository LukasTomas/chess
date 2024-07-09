#include "../../src/engineCore/ChessDefinitions.h"
#include "../../src/dataset/parsing/ParsedGame.h"
#include "../../src/evaluation/Constraint.h"
#include "../../src/evaluation/Constraints.h"

#include <vector>
#include <fstream>
using namespace std;

struct extractedData {
    int satisfied_after;
    array<set<uint8_t>, MAX_ARRAY_SIZE> white_positions, black_positions;

    string positionstToStringArray(piece_t pieceType, side_t side) {
        array<set<uint8_t>, MAX_ARRAY_SIZE> & positions = side == White ? white_positions : black_positions;

        // if ( pieceType == King )
        //     throw runtime_error("King is not allowed");

        string result = "[";
        for ( uint8_t position : positions[pieceToIndex(pieceType)] )
            result += to_string(position) + ",";
        result += "]";
        return result;
    }
};

// returns the move number when the constraint is satisfied
bool analyse( ParsedGame & game, const Constraint & constraint, extractedData & data) {
    //const Board & board = game.getController().board;

    for ( int moveNumber = 1; game.isNext(); moveNumber++ ) {
        game.doNextMove();

        /*
        array<int, MAX_ARRAY_SIZE_NO_KING> pieceCounts = {0};
        for (int i = 0; i < 64; i++) {
            if (board[i] != Piece()) {
                if (board[i].getPieceType() == King || board[i].getSide() == Black)
                    continue;

                pieceCounts[ board[i].toIndexWithoutKing() ]++;
            }
        }*/

        /*array<int, MAX_ARRAY_SIZE_NO_KING> pieceCounts = game.getController().getPieceCounts(White);
        for ( int i = 0; i < MAX_ARRAY_SIZE_NO_KING; i++ )
            cout << pieceCounts[i] << " ";
        cout << endl << endl;*/

        if( game.getController().getPieceCounts(White) == constraint.constraints ) {
            data.satisfied_after = moveNumber;
            data.white_positions = game.getController().getPositions(White);
            data.black_positions = game.getController().getPositions(Black);
            return true;
        }

    }

    return false;
}

int main() {
    const string input_file = "../chess_resources/parsed_games_2017-02.chess";
    const string output_file = "../chess_resources/csv/";

    for ( const Constraint & constraint : constrains ) {
        cout << constraint.toString() << endl;

        ifstream i(input_file, ios::binary);
        ofstream csv(output_file + constraint.toString() + ".csv");

        // csv header
        vector<string> columns = {
            "id", "result", "white_elo", "black_elo", "time_control", "eco", "opening", "satisfied_after",
            "w_king_pos", "w_queen_pos", "w_rook_pos", "w_bishop_pos", "w_knight_pos", "w_pawn_pos",
            "b_king_pos", "b_queen_pos", "b_rook_pos", "b_bishop_pos", "b_knight_pos", "b_pawn_pos"
        };

        for ( int i = 0; i < columns.size(); i++ )
            csv << columns[i] << (i == columns.size() - 1 ? "" : ";");
        csv << endl;

        ParsedGame game;
        for( int gameId = 0; game.load(i); gameId++ ) {

            if (gameId % 100000 == 0)
                cout << "game " << gameId << endl;

            extractedData data;
            if ( !analyse(game, constraint, data) )
                continue;

            csv << gameId << ";" << game.result << ";" << game.whiteElo << ";" << game.blackElo << ";" << game.timeControl << ";" << game.eco << ";" << game.opening << ";";
            csv << data.satisfied_after << ";";

            vector<piece_t> pieceTypes = {King, Queen, Rook, Bishop, Knight, Pawn};
            for ( int i = 0; i < pieceTypes.size(); i++ )
                csv << data.positionstToStringArray(pieceTypes[i], White) << ";";

            for ( int i = 0; i < pieceTypes.size(); i++ )
                csv << data.positionstToStringArray(pieceTypes[i], Black) << (i == pieceTypes.size() - 1 ? "" : ";");

            csv << endl;
        }
    }

    return 0;
}
