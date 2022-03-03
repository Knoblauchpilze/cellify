
add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/motion
	)

add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/grid
	)

target_sources (main-app_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/World.cc

	${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
	${CMAKE_CURRENT_SOURCE_DIR}/GameState.cc
	)

target_include_directories (main-app_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
