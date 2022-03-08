
target_sources (main-app_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/AI.cc

	${CMAKE_CURRENT_SOURCE_DIR}/Ant.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Colony.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Pheromon.cc
	)

target_include_directories (main-app_lib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)
