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
		std::string getPropertyName();
		std::vector<std::string> getValues();

	protected:
		DefinitionProperty(const std::string& propertyName);

		std::string mPropertyName;
		std::vector<std::string> mValues;

	private:
	};
}

#endif
