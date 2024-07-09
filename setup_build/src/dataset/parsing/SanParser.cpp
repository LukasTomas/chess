#include "SanParser.h"

#include "ParsedGame.h"

SanParser::SanParser() : o("../" + RESOURCES_DIRECTORY_NAME + PARSED_GAMES_FILENAME_BASE_NAME + PGN_FILE_DATE + PARSED_GAMES_FILENAME_EXTENSION, ios::binary) {
    pgn.FromFile("../" + RESOURCES_DIRECTORY_NAME + PGN_STARTING_NAME + PGN_FILE_DATE + ".pgn");

#ifdef SHOW_CHESSBOARD
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Chessboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        680, 680, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
#endif

    //cout << "../../" + RESOURCES_DIRECTORY_NAME + PGN_STARTING_NAME + PGN_FILE_DATE + ".pgn" << endl;
    //cout << "../../" + RESOURCES_DIRECTORY_NAME + PARSED_GAMES_FILENAME_BASE_NAME + PGN_FILE_DATE + PARSED_GAMES_FILENAME_EXTENSION << endl;
}

SanParser::~SanParser() {
    o.close();

#ifdef SHOW_CHESSBOARD
    SDL_DestroyWindow(window);
    SDL_Quit();
#endif
}


void SanParser::parseAll() {
    // this number of moves denotes a pointless game
    int pointlessGameMoves = 1;
    int pointlessGames = 0;

    int nlp_number = 0;

    const int printEvery = 1000;
    int gameNumber = 1;

    try {

        while ( true /*&& gameNumber <= 10*/ ) {
            pgn.ParseNextGame();


            pgnp::HalfMove * moves=new pgnp::HalfMove();
            pgn.GetMoves(moves); // Get the tree of half moves
            const int length=moves->GetLength(); // Get the number of half move in the current MainLine

            if ( length <= pointlessGameMoves ) {
                gameNumber++;
                cout << "skipping pointless game n. " << gameNumber << endl;

                pointlessGames++;
                continue;
            }

            if ( gameNumber % printEvery == 0 )
                cout  << "Game n." << gameNumber << " | " << length << " number of moves" << endl;

            SanGame sanGame;

            #ifdef SHOW_CHESSBOARD
                Chessboard chessboard = Chessboard(surface, Chessboard::DEFAULT_COLORS, sanGame.game.getBoard());
                chessboard.render();
                SDL_UpdateWindowSurface(window);
            #endif

            for ( int i = 0; i < length; i++ ) {
                bool nlp = sanGame.makeMove(moves->move);
                if ( nlp == false ) {
                    nlp_number++;
                    break;
                }

                #ifdef SHOW_CHESSBOARD
                    chessboard.render();
                    SDL_UpdateWindowSurface(window);
                    SDL_Delay(100);
                #endif

                moves = moves->MainLine;
            }
            saveNext(sanGame);

            gameNumber++;
        }

    } catch( pgnp::NoGameFound & e ){
        cout << e.what() << endl;
        cout << "all games" << endl;
    }

    cout << "Skipped " << pointlessGames << " pointless games" << endl;
    cout << "Total skipped games: " << pointlessGames << " out of " << gameNumber << endl;
}

void SanParser::saveNext( SanGame & sanGame ) {
    try {
        ParsedGame game(
            pgn.GetTagValue("Result"),
            pgn.GetTagValue("WhiteElo"),
            pgn.GetTagValue("BlackElo"),
            pgn.GetTagValue("TimeControl"),
            pgn.GetTagValue("ECO"),
            pgn.GetTagValue("Opening"),
            sanGame.game.getGameInfo().moves
        );

        game.save(o);
    } catch (exception e ) {
        cout << e.what() << endl;
        cout << "Error saving game - skipping" << endl;
    }
}
