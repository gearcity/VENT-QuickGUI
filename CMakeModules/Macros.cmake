# Cleaner framework finder
MACRO (find_framework fwk)
	find_library(${fwk}_LIBRARY ${fwk})
	if (${fwk}_LIBRARY)
		set(${fwk}_FOUND TRUE)
		set(${fwk}_INCLUDE "${${fwk}_LIBRARY}/Headers")
	endif (${fwk}_LIBRARY)
ENDMACRO (find_framework)