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
		* Returns a list of Sheets created.
		* NOTE: Sheets that are queued for deletion are not included in this list!
		*/
		std::list<Sheet*> getSheets();

		/**
		* Resize all Sheets.
		*/
		void resizeAllSheets(Size s);
		void resizeAllSheets(int widthInPixels, int heightInPixels);

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
