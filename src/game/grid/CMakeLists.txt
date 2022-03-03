
target_sources (main-app_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Tiles.cc

	${CMAKE_CURRENT_SOURCE_DIR}/Element.cc

	${CMAKE_CURRENT_SOURCE_DIR}/Grid.cc
	)

target_include_directories (main-app_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
