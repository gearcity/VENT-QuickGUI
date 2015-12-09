#include "QuickGUIDefinitionProperty.h"

namespace QuickGUI
{
	DefinitionProperty::DefinitionProperty(const Ogre::String& propertyName) :
		mPropertyName(propertyName)
	{
	}

	Ogre::String DefinitionProperty::getPropertyName()
	{
		return mPropertyName;
	}

	Ogre::StringVector DefinitionProperty::getValues()
	{
		return mValues;
	}
}
