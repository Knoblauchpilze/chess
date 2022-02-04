
target_sources (chess_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Pieces.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Move.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Board.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AI.cc

	${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
	)

target_include_directories (chess_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
