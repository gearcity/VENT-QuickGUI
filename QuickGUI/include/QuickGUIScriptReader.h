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
#include <unordered_map>

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
		const std::vector<std::string>& getScriptPatterns() const;

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
		ScriptDefinition* findDefinition(const std::string& type, const std::string& id);

		std::list<ScriptDefinition*> getDefinitions();
		std::list<ScriptDefinition*> getDefinitions(const std::string& type);
		ScriptDefinition* getDefinition(const std::string& type, const std::string& id);

		/**
		* Parses a file and returns all definitions found in the file.
		*/
		std::list<ScriptDefinition*> parseFile(const std::string& fileName);

		void parseScript(Ogre::DataStreamPtr &stream, const std::string &groupName);
//		void parseScript(std::ifstream& stream, const std::string& groupName);

		std::list<ScriptDefinition*> parseString(const std::string& s);

	protected:
		ScriptReader();
		virtual ~ScriptReader();
		ScriptReader(ScriptReader const&);	// copy ctor is hidden
		ScriptReader& operator=(ScriptReader const&);	// assign op is hidden

		static ScriptReader* mSingletonPtr;

		float mLoadOrder;
		std::vector<std::string> mScriptPatterns;

		std::vector<Token> mTokens;

		bool mBegin;

#if USEHASHMAPS
		std::unordered_map<std::string, std::unordered_map<std::string,ScriptDefinition*> > mTempDefinitions;
#else
		std::map<std::string, std::map<std::string,ScriptDefinition*> > mTempDefinitions;
#endif
		

		/**
		* Reads through a string and converts it to tokens.
		* The tokens are added to the token list.
		*/
		void _convertToTokens(std::string s, std::vector<Token>& tokenList);

		// Organized by script definition type, then name
#if USEHASHMAPS
	std::unordered_map<std::string, std::unordered_map<std::string,ScriptDefinition*> > mDefinitions;
#else
	std::map<std::string, std::map<std::string,ScriptDefinition*> > mDefinitions;
#endif
		
		/**
		* Iterates through tokens and creates Definitions and Properties.
		*/
#if USEHASHMAPS
		void _createDefinitions(std::vector<Token>& tokenList, std::unordered_map<std::string, std::unordered_map<std::string,ScriptDefinition*> >& defList);
		ScriptDefinition* _findDefinition(const std::string& type, const std::string& id, std::unordered_map<std::string, std::unordered_map<std::string,ScriptDefinition*> >& defList);
		ScriptDefinition* _getDefinition(const std::string& type, const std::string& id, std::unordered_map<std::string, std::unordered_map<std::string,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(std::unordered_map<std::string, std::unordered_map<std::string,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(const std::string& type, std::unordered_map<std::string, std::unordered_map<std::string,ScriptDefinition*> >& defList);

		
#else
		void _createDefinitions(std::vector<Token>& tokenList, std::map<std::string, std::map<std::string,ScriptDefinition*> >& defList);
		ScriptDefinition* _findDefinition(const std::string& type, const std::string& id, std::map<std::string, std::map<std::string,ScriptDefinition*> >& defList);
		ScriptDefinition* _getDefinition(const std::string& type, const std::string& id, std::map<std::string, std::map<std::string,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(std::map<std::string, std::map<std::string,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(const std::string& type, std::map<std::string, std::map<std::string,ScriptDefinition*> >& defList);		
#endif
		
	};
}

#endif
