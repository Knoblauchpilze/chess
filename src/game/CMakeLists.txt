
target_include_directories (chess_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

add_subdirectory (pieces)

target_sources (chess_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Round.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Board.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AI.cc

	${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameState.cc
	)
