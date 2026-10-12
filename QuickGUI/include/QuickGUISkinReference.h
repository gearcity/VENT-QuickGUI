#ifndef QUICKGUISKINREFERENCE_H
#define QUICKGUISKINREFERENCE_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISerializable.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	class _QuickGUIExport SkinReference :
		public Serializable
	{
	public:
		SkinReference(const std::string& alias);

		// Class of the Widget
		std::string className;
		// Name reference for skin
		std::string skinAlias;
		// Name of the Type for this Component Widget
		std::string typeName;

		/**
		* Builds the SkinReference from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
	};
}

#endif
