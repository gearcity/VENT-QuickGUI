#include "QuickGUIDefinitionProperty.h"

namespace QuickGUI
{
	DefinitionProperty::DefinitionProperty(const std::string& propertyName) :
		mPropertyName(propertyName)
	{
	}

	std::string DefinitionProperty::getPropertyName()
	{
		return mPropertyName;
	}

	std::vector<std::string> DefinitionProperty::getValues()
	{
		return mValues;
	}
}
