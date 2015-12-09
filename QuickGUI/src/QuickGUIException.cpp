#include "QuickGUIException.h"
#include <iostream>

namespace QuickGUI
{
	Exception::Exception( int number, const Ogre::String& description, const Ogre::String& source ) :
		Ogre::Exception(number,description,source)
	{
	std::cerr << "Exception: " << description << " (" << source << ")" << std::endl;
	}
}
