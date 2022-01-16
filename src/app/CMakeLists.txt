
target_sources (chess_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/olcEngine.cc
	${CMAKE_CURRENT_SOURCE_DIR}/TexturePack.cc
	${CMAKE_CURRENT_SOURCE_DIR}/PGEApp.cc
	)

target_include_directories (chess_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
