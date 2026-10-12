#include "QuickGUIException.h"
#include <iostream>

namespace QuickGUI
{
	Exception::Exception( int number, const std::string& description, const std::string& source ) :
		Ogre::Exception(number,description,source)
	{
	std::cerr << "Exception: " << description << " (" << source << ")" << std::endl;
	}
}
