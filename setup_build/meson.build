project('chess', 'cpp',
    default_options: ['cpp_std=c++17', 'optimization=2'])

chess_engine_sources = [

	'src/engineCore/ChessDefinitions.cpp',
	'src/engineCore/PieceIterator.cpp',

	'src/engineCore/chessComponents/Piece.cpp',
	'src/engineCore/chessComponents/Board.cpp',

	'src/engineCore/chessLogic/BoardController.cpp',
	'src/engineCore/chessLogic/Game.cpp',
	'src/engineCore/chessLogic/Rules.cpp',

	'src/evaluation/Evaluator.cpp',
	'src/evaluation/Search.cpp',
	'src/evaluation/Constraint.cpp',

	'src/game/MinimaxGame.cpp',
	'src/game/RandomGame.cpp',

	'src/graphics/Chessboard.cpp',
	'src/graphics/GraphicPieces.cpp',
	'src/graphics/GraphicsGame.cpp',

	'src/pgn/CMI.cpp',
	'src/pgn/HalfMove.cpp',
	'src/pgn/LargeFileStream.cpp',
	'src/pgn/PGN.cpp',

	'src/dataset/parsing/SanGame.cpp',
	'src/dataset/parsing/SanParser.cpp',

	'src/terminal/ProgressShower.cpp',

]

main_file = 'main/' + get_option('main') + '.cpp'
message('Using main file: ' + main_file)

sdl2_dep = dependency('sdl2', required: true)
sdl2_image_dep = dependency('SDL2_image', required: true)

catch2_dep = dependency('catch2-with-main')

executable(get_option('main'),
    sources: chess_engine_sources + [main_file],
    dependencies: [sdl2_dep, sdl2_image_dep, catch2_dep],
)

#subdir('tests')
