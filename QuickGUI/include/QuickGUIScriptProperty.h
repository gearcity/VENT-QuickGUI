#ifndef QUICKGUISCRIPTPROPERTY_H
#define QUICKGUISCRIPTPROPERTY_H

#include "QuickGUIExportDLL.h"

#include "OgreStringVector.h"

namespace QuickGUI
{
	class _QuickGUIExport ScriptProperty
	{
	public:
		friend class ScriptReader;
	public:
		Ogre::String getPropertyName();
		Ogre::StringVector& getValues();

	protected:
		ScriptProperty(const Ogre::String& propertyName);

		Ogre::String mPropertyName;
		Ogre::StringVector mValues;

	private:
	};
}

#endif
