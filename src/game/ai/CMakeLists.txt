
target_sources (chess_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/MoveGeneration.cc

	${CMAKE_CURRENT_SOURCE_DIR}/AI.cc
	${CMAKE_CURRENT_SOURCE_DIR}/RandomAI.cc
	${CMAKE_CURRENT_SOURCE_DIR}/MinimaxAI.cc
	)

target_include_directories (chess_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
