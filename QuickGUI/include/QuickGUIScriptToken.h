#ifndef QUICKGUISCRIPTTOKEN_H
#define QUICKGUISCRIPTTOKEN_H

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	class Token
	{
	public:
		enum Type
		{
			TYPE_CLOSEBRACE,
			TYPE_DEFINITION,
			TYPE_EOF,
			TYPE_PROPERTY,
			TYPE_TEXT,
			TYPE_NEWLINE,
			TYPE_OPENBRACE
		};
	public:
		Token(Type t, Ogre::String s) : type(t), value(s) {}
		
		Type type;
		Ogre::String value;
	};
}

#endif
