project(criticalsections C CXX)





set (VISIBLE_HEADERS
     "include/criticalsections/criticalsections.hpp"
   )
set (HEADERS
    ${VISIBLE_HEADERS}
    "include/criticalsections/hooks.hpp"
    "include/criticalsections/settings.hpp"
     "include/criticalsections/structs.hpp"

	)
set (SOURCES
	"src/criticalsections/criticalsections.cpp"
    "src/criticalsections/hooks.cpp"
    "src/criticalsections/settings.cpp"
    )



add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${VISIBLE_HEADERS}")
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(${PROJECT_NAME} PRIVATE common nvse)


