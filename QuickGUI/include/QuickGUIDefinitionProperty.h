#ifndef QUICKGUIDEFINITIONPROPERTY_H
#define QUICKGUIDEFINITIONPROPERTY_H

#include "QuickGUIExportDLL.h"

#include "OgrePrerequisites.h"
#include "OgreStringVector.h"

#include <map>
#include <string>
#include <vector>

namespace QuickGUI
{
	class _QuickGUIExport DefinitionProperty
	{
	public:
		friend class ScriptReader;
		friend class SerialWriter;
		friend class TextDesc;
	public:
		Ogre::String getPropertyName();
		Ogre::StringVector getValues();

	protected:
		DefinitionProperty(const Ogre::String& propertyName);

		Ogre::String mPropertyName;
		Ogre::StringVector mValues;

	private:
	};
}

#endif
