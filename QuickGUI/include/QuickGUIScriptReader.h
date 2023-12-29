#ifndef QUICKGUISCRIPTREADER_H
#define QUICKGUISCRIPTREADER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIScriptDefinition.h"
#include "QuickGUIScriptToken.h"

#include "OgreException.h"
#include "OgrePrerequisites.h"
#include "OgreResourceGroupManager.h"
#include "OgreScriptLoader.h"

#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <iomanip>   // format manipulation
#include <string>
#include <list>
#include <vector>

namespace QuickGUI
{
	void _QuickGUIExport registerScriptReader();

	class _QuickGUIExport ScriptReader :
		public Ogre::ScriptLoader,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class Root;
		friend void _QuickGUIExport registerScriptReader();
	public:

		static ScriptReader& getSingleton(void); 
		static ScriptReader* getSingletonPtr(void);

		float getLoadingOrder() const;
		const Ogre::StringVector& getScriptPatterns() const;

		/**
		* Starts isolated parsing.  All files parsed between 'begin' and 'end' create temporary
		* Script Definitions.  This allows for parsing of individual files without interfering with
		* global Definitions.
		*/
		void begin();

		void end();

		/**
		* Performs a recursive search to find a definition of the type and id given.
		*/
		ScriptDefinition* findDefinition(const Ogre::String& type, const Ogre::String& id);

		std::list<ScriptDefinition*> getDefinitions();
		std::list<ScriptDefinition*> getDefinitions(const Ogre::String& type);
		ScriptDefinition* getDefinition(const Ogre::String& type, const Ogre::String& id);

		/**
		* Parses a file and returns all definitions found in the file.
		*/
		std::list<ScriptDefinition*> parseFile(const Ogre::String& fileName);

		void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
//		void parseScript(std::ifstream& stream, const Ogre::String& groupName);

		std::list<ScriptDefinition*> parseString(const Ogre::String& s);

	protected:
		ScriptReader();
		virtual ~ScriptReader();
		ScriptReader(ScriptReader const&);	// copy ctor is hidden
		ScriptReader& operator=(ScriptReader const&);	// assign op is hidden

		static ScriptReader* mSingletonPtr;

		float mLoadOrder;
		Ogre::StringVector mScriptPatterns;

		std::vector<Token> mTokens;

		bool mBegin;

#if USEHASHMAPS
		stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> > mTempDefinitions;
#else
		std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> > mTempDefinitions;
#endif
		

		/**
		* Reads through a string and converts it to tokens.
		* The tokens are added to the token list.
		*/
		void _convertToTokens(Ogre::String s, std::vector<Token>& tokenList);

		// Organized by script definition type, then name
#if USEHASHMAPS
	stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> > mDefinitions;
#else
	std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> > mDefinitions;
#endif
		
		/**
		* Iterates through tokens and creates Definitions and Properties.
		*/
#if USEHASHMAPS
		void _createDefinitions(std::vector<Token>& tokenList, stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList);
		ScriptDefinition* _findDefinition(const Ogre::String& type, const Ogre::String& id, stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList);
		ScriptDefinition* _getDefinition(const Ogre::String& type, const Ogre::String& id, stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(const Ogre::String& type, stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList);

		
#else
		void _createDefinitions(std::vector<Token>& tokenList, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);
		ScriptDefinition* _findDefinition(const Ogre::String& type, const Ogre::String& id, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);
		ScriptDefinition* _getDefinition(const Ogre::String& type, const Ogre::String& id, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(const Ogre::String& type, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);		
#endif
		
	};
}

#endif
