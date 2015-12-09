/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

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
		// SkinTypeManager clears the flag storing if the SkinTypeManager needs to be updated.
		friend class SkinTypeManager;
		friend void _QuickGUIExport registerScriptReader();
	public:

		static ScriptReader& getSingleton(void); 
		static ScriptReader* getSingletonPtr(void);

		Ogre::Real getLoadingOrder() const;
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

		bool mSkinTypesNeedUpdate;

		static ScriptReader* mSingletonPtr;

		Ogre::Real mLoadOrder;
		Ogre::StringVector mScriptPatterns;

		std::vector<Token> mTokens;

		bool mBegin;
		std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> > mTempDefinitions;

		/**
		* Reads through a string and converts it to tokens.
		* The tokens are added to the token list.
		*/
		void _convertToTokens(Ogre::String s, std::vector<Token>& tokenList);

		// Organized by script definition type, then name
		std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> > mDefinitions;

		/**
		* Iterates through tokens and creates Definitions and Properties.
		*/
		void _createDefinitions(std::vector<Token>& tokenList, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);

		ScriptDefinition* _findDefinition(const Ogre::String& type, const Ogre::String& id, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);
		ScriptDefinition* _getDefinition(const Ogre::String& type, const Ogre::String& id, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);
		std::list<ScriptDefinition*> _getDefinitions(const Ogre::String& type, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList);
	};
}

#endif
