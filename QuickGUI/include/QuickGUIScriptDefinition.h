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
		ScriptDefinition* findDefinition(const std::string& type, const std::string& id);

		std::list<ScriptDefinition*> getDefinitions();
		std::list<ScriptDefinition*> getDefinitions(const std::string& type);
		/**
		* Searches through immediate sub definitions and returns the definition matching given type and id, if it exists.
		*/
		ScriptDefinition* getDefinition(const std::string& type, const std::string& id);
		std::string getID();
		std::string getType();
		DefinitionProperty* getProperty(const std::string& propertyName);
		std::list<DefinitionProperty*> getProperties();

		/**
		* Returns true if this definition has no properties or sub Definitions,
		* false otherwise.
		*/
		bool isEmpty();

	protected:
		ScriptDefinition(const std::string& type, const std::string& id);
		~ScriptDefinition();

		std::string mType;
		std::string mID;

		ScriptDefinition* mParentDefinition;

#if USEHASHMAPS
		std::unordered_map<std::string, std::unordered_map<std::string,ScriptDefinition*> > mSubDefinitions;
#else
		std::map<std::string, std::map<std::string,ScriptDefinition*> > mSubDefinitions;
#endif
		std::list<ScriptDefinition*> mSubDefinitionsInOrder;

#if USEHASHMAPS
		std::unordered_map<std::string, DefinitionProperty*> mProperties;
#else
		std::map<std::string, DefinitionProperty*> mProperties;
#endif

	private:
	};
}

#endif
