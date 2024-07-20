project(nvse)
set (VISIBLE_HEADERS
	"nvse/nvse_version.h"
	"nvse/PluginAPI.h"
	"nvse/prefix.h"
	"nvse/SafeWrite.h"
	"nvse/Utilities.h"
)

set (HEADERS
	${VISIBLE_HEADERS}
)
set (SOURCES
	"nvse/SafeWrite.cpp"
	)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  INTERFACE $<INSTALL_INTERFACE:>  
  )
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")
