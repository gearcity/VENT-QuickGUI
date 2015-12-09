# Cache variables
option(USE_OGRE_SDK ON)

if (USE_OGRE_SDK)
	# Find SDK installation
	
	# Search paths
	set(OGRE_SDK_PATH "" CACHE PATH "")
	set(DEP_INCLUDE_PATH
		"${OGRE_SDK_PATH}/include"
		"${OGRE_SDK_PATH}/include/OIS"
	)
	set(DEP_LIB_PATH "${OGRE_SDK_PATH}/lib")
else()
	# Find source installation
	
	# Search paths
	set(DEP_INCLUDE_PATH
	)
	set(DEP_LIB_PATH
	)
endif()

set(DEP_INCLUDE_DIRS "")
message(${DEP_LIB_PATH})

# Find Ogre
find_path(OGRE_INCLUDE_DIR Ogre.h ${DEP_INCLUDE_PATH})
find_library(OGRE_LIBRARY_REL NAME OgreMain.lib PATHS ${DEP_LIB_PATH})
find_library(OGRE_LIBRARY_DBG NAME OgreMain_d.lib PATHS ${DEP_LIB_PATH})
message(${OGRE_LIBRARY_REL})
if (OGRE_LIBRARY_REL AND OGRE_LIBRARY_DBG)
	set(OGRE_LIBRARY optimized ${OGRE_LIBRARY_REL} debug ${OGRE_LIBRARY_DBG})
elseif (OGRE_LIBRARY_REL)
	set(OGRE_LIBRARY ${OGRE_LIBRARY_REL})
elseif (OGRE_LIBRARY_DBG)
	set(OGRE_LIBRARY ${OGRE_LIBRARY_DBG})
endif()
if (OGRE_INCLUDE_DIR AND OGRE_LIBRARY)
	set(OGRE_FOUND TRUE)
	set(DEP_INCLUDE_DIRS ${DEP_INCLUDE_DIRS} ${OGRE_INCLUDE_DIR})
endif()

# Find OIS
find_path(OIS_INCLUDE_DIR OIS.h ${DEP_INCLUDE_PATH})
find_library(OIS_LIBRARY_REL NAME OIS PATHS ${DEP_LIB_PATH})
find_library(OIS_LIBRARY_DBG NAME OIS_d PATHS ${DEP_LIB_PATH})
if (OIS_LIBRARY_REL AND OIS_LIBRARY_DBG)
	set(OIS_LIBRARY optimized ${OIS_LIBRARY_REL} debug ${OIS_LIBRARY_DBG})
elseif (OIS_LIBRARY_REL)
	set(OIS_LIBRARY ${OIS_LIBRARY_REL})
elseif (OIS_LIBRARY_DBG)
	set(OIS_LIBRARY ${OIS_LIBRARY_DBG})
endif()	
if (OIS_INCLUDE_DIR AND OIS_LIBRARY)
	set(OIS_FOUND TRUE)
	set(DEP_INCLUDE_DIRS ${DEP_INCLUDE_DIRS} ${OIS_INCLUDE_DIR})
endif()

# Alert CMake about the new includes
include_directories(${DEP_INCLUDE_DIRS})