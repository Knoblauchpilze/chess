
target_sources (chess_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Viewport.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CoordinateFrame.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TopViewFrame.cc
	)

target_include_directories (chess_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
