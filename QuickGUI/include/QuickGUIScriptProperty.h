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
		std::string getPropertyName();
		std::vector<std::string>& getValues();

	protected:
		ScriptProperty(const std::string& propertyName);

		std::string mPropertyName;
		std::vector<std::string> mValues;

	private:
	};
}

#endif
