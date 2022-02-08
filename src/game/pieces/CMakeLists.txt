
target_sources (chess_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Coordinates.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Piece.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Common.cc

	${CMAKE_CURRENT_SOURCE_DIR}/Pawn.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Knight.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Bishop.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Rook.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Queen.cc
	${CMAKE_CURRENT_SOURCE_DIR}/King.cc
	)

target_include_directories (chess_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
