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

#ifndef QUICKGUISKINTYPEMANAGER_H
#define QUICKGUISKINTYPEMANAGER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISkinType.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	class _QuickGUIExport SkinTypeManager :
		public Ogre::Singleton<SkinTypeManager>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class SkinDefinitionManager;
	public:

		static SkinTypeManager& getSingleton(void);
		static SkinTypeManager* getSingletonPtr(void);

		void addSkinType(const Ogre::String& className, const Ogre::String& typeName, SkinType* t);

		SkinType* getSkinType(const Ogre::String& className, const Ogre::String& typeName);

		bool hasSkinType(const Ogre::String& className, const Ogre::String& typeName);

		/**
		* This function checks the ScriptReader to see if new SkinType definitions have been parsed.
		* If the SkinType library needs to be updated, it will first be cleared and then rebuilt
		* according to the ScriptDefinitions held by the ScriptReader.
		* NOTE: this function should be called whenever more SkinTypes have been parsed, to make sure the
		*       library is up to date. (ie, if you load resource groups at different times and some contain
		*       SkinType definitions, this function should be called.  When in doubt, call this function.
		*/
		void loadTypes();

		void saveTypesToFile(const Ogre::String& fileName);

	protected:
		SkinTypeManager();
		virtual ~SkinTypeManager();

		std::map<Ogre::String, std::map<Ogre::String,SkinType*> > mSkinTypes;

	private:
	};
}

#endif
