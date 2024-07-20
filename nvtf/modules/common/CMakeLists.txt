project(common CXX)
set (VISIBLE_HEADERS
	"include/common/common.hpp"
	"include/common/structs.hpp"
	"include/common/settings.hpp"

)

set (HEADERS
	${VISIBLE_HEADERS}
	)
set (SOURCES
	"src/common/settings.cpp"
	"src/common/common.cpp"
	)





add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${VISIBLE_HEADERS}")

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE nvse)
