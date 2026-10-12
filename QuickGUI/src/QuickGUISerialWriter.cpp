#include "QuickGUISerialWriter.h"
#include "QuickGUIScriptWriter.h"

template<> QuickGUI::SerialWriter* Ogre::Singleton<QuickGUI::SerialWriter>::msSingleton = 0;

namespace QuickGUI
{
	SerialWriter::SerialWriter() :
		SerialBase()
	{
	}

	SerialWriter* SerialWriter::getSingletonPtr(void) 
	{ 
		return msSingleton; 
	}

	SerialWriter& SerialWriter::getSingleton(void) 
	{ 
		assert( msSingleton );  
		return ( *msSingleton ); 
	}

	bool SerialWriter::begin(const std::string& definitionType, const std::string& definitionID)
	{
		ScriptDefinition* newDef = OGRE_NEW_T(ScriptDefinition,Ogre::MEMCATEGORY_GENERAL)(definitionType,definitionID);
		newDef->mParentDefinition = mCurrentDefinition;

		if(mCurrentDefinition != NULL)
		{
			if(mCurrentDefinition->mSubDefinitions[definitionType].find(definitionID) != mCurrentDefinition->mSubDefinitions[definitionType].end())
				throw Exception(Exception::ERR_SERIALIZATION,"Definition \"" + mCurrentDefinition->getType() + " " + mCurrentDefinition->getID() + "\" already contains a definition \"" + definitionType + " " + definitionID + "!","SerialWriter::begin");
			mCurrentDefinition->mSubDefinitions[definitionType][definitionID] = newDef;
			mCurrentDefinition->mSubDefinitionsInOrder.push_back(newDef);
		}

		mCurrentDefinition = newDef;

		return true;
	}

	void SerialWriter::end()
	{
		// If this is the root ScriptDefinition, add it to ScriptWriter
		if(mCurrentDefinition->mParentDefinition == NULL)
			ScriptWriter::getSingletonPtr()->addDefinition(mCurrentDefinition);
		
		mCurrentDefinition = mCurrentDefinition->mParentDefinition;
	}

	void SerialWriter::IO(const std::string& propertyName, unsigned short* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, BrushFilterMode* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));

		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, ConsoleLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));

		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, int* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(*member));

		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, unsigned int* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, ColourValue* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->r));
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->g));
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->b));
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->a));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, VScrollBarButtonLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GEOMETRY)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, HScrollBarButtonLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, ToolBarItemLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, std::string* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(*member);
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, bool* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, float* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, Point* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->x));
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->y));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, ProgressBarFillDirection* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, ProgressBarLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, ProgressBarClippingEdge* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, Rect* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->position.x));
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->position.y));
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->size.width));
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->size.height));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, Size* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->width));
		newProp->mValues.push_back(Ogre::StringConverter::toString(member->height));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, HorizontalTextAlignment* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, HorizontalAnchor* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, VerticalAnchor* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	void SerialWriter::IO(const std::string& propertyName, VerticalTextAlignment* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(StringConverter::toString(*member));
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	bool SerialWriter::isSerialWriter()
	{
		return true;
	}

	bool SerialWriter::isSerialReader()
	{
		return false;
	}
}
