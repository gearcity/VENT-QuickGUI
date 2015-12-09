# Cache variables
option(DISABLE_PKG_CONFIG OFF)

find_package(PkgConfig)
if (NOT DISABLE_PKG_CONFIG AND PKG_CONFIG_FOUND)
	# Use Package-Config
	pkg_check_modules(OGRE OGRE)
	pkg_check_modules(OIS OIS)
	if (OGRE_FOUND)
		set(DEP_INCLUDE_PATH ${DEP_INCLUDE_PATH} ${OGRE_INCLUDE_DIRS})
		set(DEP_LIB_PATH ${DEP_LIB_PATH} ${OGRE_LIBRARIES})
	endif ()
	if (OIS_FOUND)
		set(DEP_INCLUDE_PATH ${DEP_INCLUDE_PATH} ${OIS_INCLUDE_DIRS})
		set(DEP_LIB_PATH ${DEP_LIB_PATH} ${OIS_LIBRARIES})
	endif ()
else ()
	# Find packages manually

	# Search paths
	set(CUSTOM_DEP_DIR "" CACHE PATH "")
	set(DEP_INCLUDE_PATH
		"${CUSTOM_DEP_DIR}/include"
		"${CUSTOM_DEP_DIR}/include/OGRE"
		"${CUSTOM_DEP_DIR}/include/OIS"
		/usr/include
		/usr/include/OGRE
		/usr/include/OIS
		/usr/local/include
		/usr/local/include/OGRE
		/usr/local/include/OIS
	)
	set(DEP_LIB_PATH
		"${CUSTOM_DEP_DIR}/lib"
		/usr/lib
		/usr/local/lib
	)
	set(DEP_INCLUDE_DIRS "")
	
	# Find OGRE
	find_path(OGRE_INCLUDE_DIR Ogre.h ${DEP_INCLUDE_PATH})
	find_library(OGRE_LIBRARY_REL NAME OgreMain PATHS ${DEP_LIB_PATH})
	find_library(OGRE_LIBRARY_DBG NAME OgreMain_d PATHS ${DEP_LIB_PATH})
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
	
	# Actually add directories
	include_directories(${DEP_INCLUDE_DIRS})
endif()
