#include "QuickGUIScriptDefinition.h"

namespace QuickGUI
{
	ScriptDefinition::ScriptDefinition(const Ogre::String& type, const Ogre::String& id) :
		mType(type),
		mID(id),
		mParentDefinition(NULL)
	{
	}

	ScriptDefinition::~ScriptDefinition()
	{
		for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = mSubDefinitions.begin(); it1 != mSubDefinitions.end(); ++it1)
		{
			for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
			{
				OGRE_DELETE_T((*it2).second,ScriptDefinition,Ogre::MEMCATEGORY_GENERAL);
			}
		}

		for(std::map<Ogre::String, DefinitionProperty*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
			OGRE_DELETE_T((*it).second,DefinitionProperty,Ogre::MEMCATEGORY_GENERAL);
	}

	ScriptDefinition* ScriptDefinition::findDefinition(const Ogre::String& type, const Ogre::String& id)
	{
		if((mType == type) && (mID == id))
			return this;

		ScriptDefinition* d = NULL;
		for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = mSubDefinitions.begin(); it1 != mSubDefinitions.end(); ++it1)
		{
			for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
			{
				d = (*it2).second->findDefinition(type,id);
				if(d != NULL)
					return d;
			}
		}

		return d;
	}

	std::list<ScriptDefinition*> ScriptDefinition::getDefinitions()
	{
		std::list<ScriptDefinition*> defList(mSubDefinitionsInOrder);

		return defList;
	}

	std::list<ScriptDefinition*> ScriptDefinition::getDefinitions(const Ogre::String& type)
	{
		if(mSubDefinitions.find(type) == mSubDefinitions.end())
		{
			std::cout << "No definitions for type \"" + type + "\" found!" << std::endl;
//			return mSubDefinitions[type];
		}

		std::list<ScriptDefinition*> defList;
		for(std::map<Ogre::String,ScriptDefinition*>::iterator it = mSubDefinitions[type].begin(); it != mSubDefinitions[type].end(); ++it)
			defList.push_back((*it).second);

		return defList;
	}

	ScriptDefinition* ScriptDefinition::getDefinition(const Ogre::String& type, const Ogre::String& id)
	{
		if(mSubDefinitions.find(type) == mSubDefinitions.end())
		{
			std::cout << "No definitions for type \"" + type + "\" found!" << std::endl;
			return mSubDefinitions[type][id];
		}

		if(mSubDefinitions[type].find(id) == mSubDefinitions[type].end())
		{
			std::cout << "No \"" + type + "\" definition with id \"" + id + "\" found!" << std::endl;
			return mSubDefinitions[type][id];
		}

		return mSubDefinitions[type][id];
	}

	Ogre::String ScriptDefinition::getID()
	{
		return mID;
	}

	Ogre::String ScriptDefinition::getType()
	{
		return mType;
	}

	DefinitionProperty* ScriptDefinition::getProperty(const Ogre::String& propertyName)
	{
		if(mProperties.find(propertyName) == mProperties.end())
			return NULL;

		return mProperties[propertyName];
	}

	std::list<DefinitionProperty*> ScriptDefinition::getProperties()
	{
		std::list<DefinitionProperty*> propList;
		for(std::map<Ogre::String, DefinitionProperty*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
			propList.push_back((*it).second);

		return propList;
	}

	bool ScriptDefinition::isEmpty()
	{
		if(!mProperties.empty())
			return false;

		if(!mSubDefinitions.empty())
			return false;

		return true;
	}
}
