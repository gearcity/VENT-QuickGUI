#include "QuickGUIScriptWriter.h"
#include "QuickGUIScriptReader.h"

template<> QuickGUI::ScriptWriter* Ogre::Singleton<QuickGUI::ScriptWriter>::msSingleton = 0;

namespace QuickGUI
{
	ScriptWriter::ScriptWriter() :
		mWritingToFile(false),
		mFileName(""),
		mNumSpacesPerLevel(4)
	{
	}

	ScriptWriter::~ScriptWriter()
	{
		for(std::list<ScriptDefinition*>::iterator it = mDefinitions.begin(); it != mDefinitions.end(); ++it)
			OGRE_DELETE_T((*it),ScriptDefinition,Ogre::MEMCATEGORY_GENERAL);
	}

	ScriptWriter* ScriptWriter::getSingletonPtr(void) 
	{ 
		return msSingleton; 
	}

	ScriptWriter& ScriptWriter::getSingleton(void) 
	{ 
		assert( msSingleton );  
		return ( *msSingleton ); 
	}

	void ScriptWriter::addDefinition(ScriptDefinition* d)
	{
		for(std::list<ScriptDefinition*>::iterator it = mDefinitions.begin(); it != mDefinitions.end(); ++it)
		{
			// Overwrite any ScriptDefinitions that have the same type and ID
			if(((*it)->getType() == d->getType()) && ((*it)->getID() == d->getID()))
			{
				OGRE_DELETE_T((*it),ScriptDefinition,Ogre::MEMCATEGORY_GENERAL);
				(*it) = d;
				return;
			}
		}

		mDefinitions.push_back(d);
	}

	void ScriptWriter::begin(const Ogre::String& fileName)
	{
		if(mWritingToFile)
			throw Exception(Exception::ERR_SCRIPT_WRITING,"ScriptWriter already in the process of writing to a file!","ScriptWriter::begin");

		mWritingToFile = true;
		mFileName = fileName;
	}

	void ScriptWriter::end()
	{
		if(!mWritingToFile)
			throw Exception(Exception::ERR_SCRIPT_WRITING,"ScriptWriter has not been setup to write to a file! (missing call to ScripWriter::begin()?)","ScriptWriter::end");

		std::fstream file(mFileName.c_str(), std::ios::out);

		for(std::list<ScriptDefinition*>::iterator it = mDefinitions.begin(); it != mDefinitions.end(); ++it)
		{
			writeDefinition(file,(*it),0);
		}

        file.close();

		mWritingToFile = false;
	}

	void ScriptWriter::writeDefinition(std::fstream& stream, ScriptDefinition* d, int level)
	{
		// Write the definition header
		stream << getSpaces(level * mNumSpacesPerLevel) + d->getType() + " " + d->getID() << std::endl;
		// Write opening curly brace
		stream << getSpaces(level * mNumSpacesPerLevel) + "{" << std::endl;
		// Write out properties
		std::list<DefinitionProperty*> propList = d->getProperties();
		for(std::list<DefinitionProperty*>::iterator it = propList.begin(); it != propList.end(); ++it)
		{
			DefinitionProperty* p = (*it);
			stream << getSpaces((level + 1) * mNumSpacesPerLevel) + p->getPropertyName();

			Ogre::StringVector sv = p->getValues();
			for(Ogre::StringVector::iterator propertyIter = sv.begin(); propertyIter != sv.end(); ++propertyIter)
			{
				stream << " " + (*propertyIter);
			}

			stream << std::endl;
		}
		// Write out sub Definitions
		std::list<ScriptDefinition*> subDefList = d->getDefinitions();
		if(!subDefList.empty())
		{
			if(!propList.empty())
				stream << std::endl;

			for(std::list<ScriptDefinition*>::iterator it = subDefList.begin(); it != subDefList.end(); ++it)
			{
				writeDefinition(stream,(*it),(level + 1));
				stream << std::endl;
			}
		}
		// Write closing curly brace
		stream << getSpaces(level * mNumSpacesPerLevel) + "}" << std::endl;
	}

	Ogre::String ScriptWriter::getSpaces(unsigned int numberOfSpaces)
	{
		Ogre::String s = "";
		s.append(numberOfSpaces, ' ');
		return s;
	}

	bool ScriptWriter::writingToFile()
	{
		return mWritingToFile;
	}
}
