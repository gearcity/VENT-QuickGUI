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

#ifndef QUICKGUISHEETMANAGER_H
#define QUICKGUISHEETMANAGER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISize.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	//forward declarations
	class Sheet;
	class SheetDesc;

	/*
	* The Sheet manager manages creation, deletion, and access of Sheets
	*/
	class _QuickGUIExport SheetManager :
		public Ogre::Singleton<SheetManager>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		// Root creates/destroys SheetManager
		friend class Root;
		// GUIManager calls 'cleanup' function
		friend class GUIManager;
	public:
		static SheetManager& getSingleton(void); 
		static SheetManager* getSingletonPtr(void);

		/**
		* Create a Sheet from a desc object.
		*/
		Sheet* createSheet(SheetDesc* d);
		Sheet* createSheet(const Size& s);
		/**
		* Create a Sheet from a string.
		* If loadFromFile is true, s is treated as a File, and opened, parsed and used to build a Sheet.
		* If loadFromFile is false, s is treated as a String, and parsed and used to build a Sheet.
		*/
		Sheet* createSheet(const Ogre::String& s, bool loadFromFile);

		/**
		* Queues a Sheet for destruction.
		*/
		void destroySheet(Sheet* s);
	
		/**
		* Returns the first Sheet with the name given, or NULL if no Sheets with the
		* name given exist.
		* NOTE: There is no enforcement that Sheet names be unique.
		*/
		Sheet* getSheet(const std::string& sheetName);
		/**
		* Returns a list of Sheets created.
		* NOTE: Sheets that are queued for deletion are not included in this list!
		*/
		std::list<Sheet*> getSheets();

		/**
		* Resize all Sheets.
		*/
		void resizeAllSheets(Size s);
		void resizeAllSheets(int widthInPixels, int heightInPixels);

		/**
		* Returns true if any created sheets with the name given are found, false otherwise.
		* NOTE: There is no enforcement that Sheet names be unique.
		*/
		bool sheetExists(const std::string& sheetName);

	protected:
		SheetManager();
		virtual ~SheetManager();

		/**
		* Destroys all sheets queued for deletion.
		*/
		void cleanup();

		std::list<Sheet*> mSheets;
		std::list<Sheet*> mFreeList;

	private:
	};
}

#endif
