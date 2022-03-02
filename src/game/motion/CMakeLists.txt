
target_sources (main-app_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Node.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Path.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AStarNodes.cc
	${CMAKE_CURRENT_SOURCE_DIR}/AStar.cc
	)

target_include_directories (main-app_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
