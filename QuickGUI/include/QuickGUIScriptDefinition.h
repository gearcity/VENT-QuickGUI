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

#ifndef QUICKGUISCRIPTDEFINITION_H
#define QUICKGUISCRIPTDEFINITION_H

#include "QuickGUIDefinitionProperty.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"

#include "OgrePrerequisites.h"

#include <iostream>
#include <list>
#include <map>

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

		std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> > mSubDefinitions;
		std::list<ScriptDefinition*> mSubDefinitionsInOrder;
		std::map<Ogre::String, DefinitionProperty*> mProperties;

	private:
	};
}

#endif
