#ifndef QUICKGUISCRIPTDEFINITION_H
#define QUICKGUISCRIPTDEFINITION_H

#include "QuickGUIDefinitionProperty.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"

#include "OgrePrerequisites.h"

#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

namespace QuickGUI
{
	class _QuickGUIExport ScriptDefinition
	{
	public:
		friend class ScriptReader;
		// ScriptWriter Cleans up remaining ScriptDefinitions
		friend class ScriptWriter;
		friend class SerialReader;
		friend class SerialWriter;
		friend class TextDesc;
	public:
		/**
		* Performs a recursive search to find a definition of the type and id given.
		*/
		ScriptDefinition* findDefinition(const Ogre::String& type, const Ogre::String& id);

		std::list<ScriptDefinition*> getDefinitions();
		std::list<ScriptDefinition*> getDefinitions(const Ogre::String& type);
		/**
		* Searches through immediate sub definitions and returns the definition matching given type and id, if it exists.
		*/
		ScriptDefinition* getDefinition(const Ogre::String& type, const Ogre::String& id);
		Ogre::String getID();
		Ogre::String getType();
		DefinitionProperty* getProperty(const Ogre::String& propertyName);
		std::list<DefinitionProperty*> getProperties();

		/**
		* Returns true if this definition has no properties or sub Definitions,
		* false otherwise.
		*/
		bool isEmpty();

	protected:
		ScriptDefinition(const Ogre::String& type, const Ogre::String& id);
		~ScriptDefinition();

		Ogre::String mType;
		Ogre::String mID;

		ScriptDefinition* mParentDefinition;

#if USEHASHMAPS
		std::unordered_map<Ogre::String, std::unordered_map<Ogre::String,ScriptDefinition*> > mSubDefinitions;
#else
		std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> > mSubDefinitions;
#endif
		std::list<ScriptDefinition*> mSubDefinitionsInOrder;

#if USEHASHMAPS
		std::unordered_map<Ogre::String, DefinitionProperty*> mProperties;
#else
		std::map<Ogre::String, DefinitionProperty*> mProperties;
#endif

	private:
	};
}

#endif
