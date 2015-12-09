# Find packages
find_framework(OGRE)
find_framework(OIS)

# Add include paths
set(DEP_INCLUDE_DIRS
	${OGRE_INCLUDE}
	${OIS_INCLUDE}
)
include_directories(${DEP_INCLUDE_DIRS})
