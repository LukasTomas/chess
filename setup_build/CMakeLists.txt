cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(neural)

set(CMAKE_CXX_STANDARD 17)

# Include the library's include directories

set(SOURCES
    src/engineCore/ChessDefinitions.cpp
	src/engineCore/PieceIterator.cpp

	src/engineCore/chessComponents/Board.cpp
	src/engineCore/chessComponents/Piece.cpp
	src/engineCore/chessComponents/Bitboard.cpp

	src/engineCore/chessLogic/BoardController.cpp
	src/engineCore/chessLogic/Game.cpp
	src/engineCore/chessLogic/Rules.cpp

    src/evaluation/Constraint.cpp
	src/evaluation/Evaluator.cpp
	src/evaluation/Search.cpp

	src/game/MinimaxGame.cpp
	src/game/RandomGame.cpp

	#src/graphics/Chessboard.cpp
	#src/graphics/GraphicPieces.cpp
	#src/graphics/GraphicsGame.cpp

	src/pgn/CMI.cpp
	src/pgn/HalfMove.cpp
	src/pgn/LargeFileStream.cpp
	src/pgn/PGN.cpp

    #src/dataset/ChessDataset.cpp
	src/dataset/ChessDataset_2.cpp
	src/dataset/ChessDataset_analyzed.cpp
	src/dataset/DatasetIterator.cpp
	src/dataset/BitboardAnalyzed.cpp
	src/dataset/parsing/ParsedGame.cpp
	src/dataset/parsing/SanGame.cpp
	src/dataset/parsing/SanParser.cpp

    src/neural/SimpleNet.cpp
	src/neural/ConvNet.cpp
	src/neural/ResidualBlock.cpp
	src/neural/ResNet.cpp

	src/terminal/ProgressShower.cpp
	src/terminal/Space.cpp
	src/terminal/UnitCircle.cpp
	src/terminal/Logging.cpp
	src/terminal/Color.cpp
	src/terminal/DrawingChars.cpp

	main/common/Program.cpp
	main/common/Helper.cpp
)

#find_package(CUD)
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(neural main/GeneratePositions.cpp)
target_sources(neural PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(neural "${TORCH_LIBRARIES}" -pthread) #//pridat cudnn sem a v kodu se da nastavi .use = true, -predat parametr CAFFE2_USE_CUDNN=1
set_property(TARGET neural PROPERTY CXX_STANDARD 17)

target_compile_options(neural PRIVATE -O2)
