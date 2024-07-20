project(nvtf C CXX)
cmake_minimum_required(VERSION 3.10)
if(NOT MSVC)
	message("This project only supports MSVC due to calling conventions")
	return()

elseif(NOT WIN32)
	message("Only for Win32")
	return()
endif()
set(CMAKE_CXX_STANDARD 17)

if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
endif()







set (VISIBLE_HEADERS
	"src/main.h"
	)
set(HEADERS
	${VISIBLE_HEADERS}
)

set(SOURCES
	"src/main.cpp"
)
	
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../lib" "libs")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules")
target_link_libraries(${PROJECT_NAME} PRIVATE nvse)
target_link_libraries(${PROJECT_NAME} PRIVATE common)
target_link_libraries(${PROJECT_NAME} PRIVATE criticalsections)