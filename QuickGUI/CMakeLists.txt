cmake_minimum_required(VERSION 3.18)
project(QuickGUI)

# Options
option(QUICKGUI_STATIC "Build QuickGUI as a static library" FALSE)

# Prompt the user to select the OGRE_LIBRARY using CMake GUI
set(OGRE_LIBRARY "/usr/local/lib/" CACHE FILEPATH "Path to the OGRE library")

# Explicitly specify the OGRE3D include directory
set(OGRE_INCLUDE_DIR "/usr/local/include/" CACHE PATH "Path to OGRE3D include directory")

# Check if the directory exists
if(NOT EXISTS ${OGRE_INCLUDE_DIR}/Ogre.h)
    message(FATAL_ERROR "The specified OGRE_INCLUDE_DIR does not contain Ogre.h. Please specify a valid path.")
endif()

# Source files
file(GLOB QUICKGUI_SRC "src/*.cpp")
file(GLOB QUICKGUI_INCLUDE "include/*.h")

# Library type
if (QUICKGUI_STATIC)
    add_library(QuickGUI STATIC ${QUICKGUI_SRC} ${QUICKGUI_INCLUDE})
else()
    add_library(QuickGUI SHARED ${QUICKGUI_SRC} ${QUICKGUI_INCLUDE})
endif()

# Include directories
target_include_directories(QuickGUI 
    PUBLIC 
    ${OGRE_INCLUDE_DIR}  # Add OGRE3D include directory
    include
)

# Linking
target_link_libraries(QuickGUI ${OGRE_LIBRARY})

# Compiler options based on architecture
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    target_compile_options(QuickGUI PRIVATE -fPIC ${DEBUG_FLAGS} ${MISC_FLAGS})
else()
    target_compile_options(QuickGUI PRIVATE ${DEBUG_FLAGS} ${MISC_FLAGS})
endif()

# macOS specific settings
if (APPLE)
    set_target_properties(QuickGUI PROPERTIES FRAMEWORK TRUE)
    set_target_properties(QuickGUI PROPERTIES PUBLIC_HEADER "${QUICKGUI_INCLUDE}")
    set_target_properties(QuickGUI PROPERTIES OUTPUT_NAME QuickGUI)
    set_target_properties(QuickGUI PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
    install(TARGETS QuickGUI FRAMEWORK DESTINATION .)
else()
    # Linux install rules
    install(TARGETS QuickGUI LIBRARY DESTINATION lib)
    install(FILES ${QUICKGUI_INCLUDE} DESTINATION include/QuickGUI)
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/QuickGUI.h "#include <QuickGUI/QuickGUI.h>\n")
    install(FILES QuickGUI.h DESTINATION include)
endif()