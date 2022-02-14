
target_sources (chess_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/MoveGeneration.cc
	${CMAKE_CURRENT_SOURCE_DIR}/BoardEvaluation.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AI.cc
	)

target_include_directories (chess_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
