include_directories(include)

option(QUICKGUI_STATIC FALSE)
if (QUICKGUI_STATIC)
	set(QUICKGUI_LIB_TYPE STATIC)
else()
	set(QUICKGUI_LIB_TYPE SHARED)
endif()

file(GLOB QUICKGUI_SRC "src/*.cpp")
file(GLOB QUICKGUI_INCLUDE "include/*.h")

# Build rules
add_library(QuickGUI ${QUICKGUI_LIB_TYPE} ${QUICKGUI_SRC} ${QUICKGUI_INCLUDE})
target_link_libraries(QuickGUI ${OGRE_LIBRARY})
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set_target_properties(QuickGUI PROPERTIES COMPILE_FLAGS "-fPIC ${DEBUG_FLAGS} ${MISC_FLAGS}")
elseif()
	set_target_properties(QuickGUI PROPERTIES COMPILE_FLAGS "${DEBUG_FLAGS} ${MISC_FLAGS}")
endif()

if (UNIX)
	if (APPLE)
		# Deal with Mac OS X's framework system
		set_target_properties(QuickGUI PROPERTIES FRAMEWORK TRUE)
		set_target_properties(QuickGUI PROPERTIES PUBLIC_HEADER "${QUICKGUI_INCLUDE}")
		set_target_properties(QuickGUI PROPERTIES OUTPUT_NAME QuickGUI)
		set_target_properties(QuickGUI PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
		set_target_properties(QuickGUI PROPERTIES INSTALL_NAME_DIR "@executable_path/../Frameworks")
		install(TARGETS QuickGUI FRAMEWORK DESTINATION .)
	else ()
		# Linux install rules
		install(TARGETS QuickGUI LIBRARY DESTINATION lib)
		install(FILES ${QUICKGUI_INCLUDE} DESTINATION include/QuickGUI)
		file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/QuickGUI.h "#include <QuickGUI/QuickGUI.h>\n")
		install(FILES QuickGUI.h DESTINATION include)
	endif ()
endif ()