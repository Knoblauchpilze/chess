
target_sources (chess_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
	)

target_include_directories (chess_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
