test_sources = []
foreach s : chess_engine_sources
    test_sources += '../' + s
endforeach

board_test = executable('board_test',
    sources: ['Board_test.cpp'] + test_sources,
    dependencies: [sdl2_dep, sdl2_image_dep, catch2_dep],
)

boardController_test = executable('boardController_test',
    sources: ['BoardController_test.cpp'] + test_sources,
    dependencies: [sdl2_dep, sdl2_image_dep, catch2_dep],
)

evaluator_test = executable('evaluator_test',
    sources: ['Evaluator_test.cpp'] + test_sources,
    dependencies: [sdl2_dep, sdl2_image_dep, catch2_dep],
)

piece_test = executable('piece_test',
    sources: ['Piece_test.cpp'] + test_sources,
    dependencies: [sdl2_dep, sdl2_image_dep, catch2_dep],
)

rules_test = executable('rules_test',
    sources: ['Rules_test.cpp'] + test_sources,
    dependencies: [sdl2_dep, sdl2_image_dep, catch2_dep],
)

test('board_test', board_test)
test('boardController_test', boardController_test)
test('evaluator_test', evaluator_test)
test('piece_test', piece_test)
test('rules_test', rules_test)
