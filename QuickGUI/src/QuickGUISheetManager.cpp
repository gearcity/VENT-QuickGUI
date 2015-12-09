#include "QuickGUISheetManager.h"
#include "QuickGUISheet.h"
#include "QuickGUIDescManager.h"

template<> QuickGUI::SheetManager* Ogre::Singleton<QuickGUI::SheetManager>::ms_Singleton = 0;

namespace QuickGUI
{
	SheetManager::SheetManager()
	{
	}

	SheetManager::~SheetManager()
	{
		cleanup();

		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
			OGRE_DELETE_T((*it),Sheet,Ogre::MEMCATEGORY_GENERAL);
	}

	SheetManager* SheetManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	SheetManager& SheetManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	void SheetManager::cleanup()
	{
		std::list<Sheet*>::iterator it;
		while(!mFreeList.empty())
		{
			// Remove from mSheets list if found
			it = std::find(mSheets.begin(),mSheets.end(),mFreeList.front());
			if(it != mSheets.end())
				mSheets.erase(it);
			// Delete Sheet
			OGRE_DELETE_T(mFreeList.front(),Sheet,Ogre::MEMCATEGORY_GENERAL);
			// Update FreeList
			mFreeList.pop_front();
		}
	}

	Sheet* SheetManager::createSheet(SheetDesc* d)
	{
		Sheet* newSheet = OGRE_NEW_T(Sheet,Ogre::MEMCATEGORY_GENERAL)(d);
		mSheets.push_back(newSheet);

		return newSheet;
	}

	Sheet* SheetManager::createSheet(const Size& s)
	{
		SheetDesc* sd = DescManager::getSingleton().getDefaultSheetDesc();
		sd->resetToDefault();
		sd->widget_dimensions.size = s;

		Sheet* newSheet = OGRE_NEW_T(Sheet,Ogre::MEMCATEGORY_GENERAL)(sd);
		mSheets.push_back(newSheet);

		return newSheet;
	}

	Sheet* SheetManager::createSheet(const Ogre::String& s, bool loadFromFile)
	{
		Sheet* newSheet = OGRE_NEW_T(Sheet,Ogre::MEMCATEGORY_GENERAL)(s,loadFromFile);

		mSheets.push_back(newSheet);

		return newSheet;
	}

	void SheetManager::destroySheet(Sheet* s)
	{
		std::list<Sheet*>::iterator it = std::find(mFreeList.begin(),mFreeList.end(),s);
		if(it == mFreeList.end())
			mFreeList.push_back(s);
	}

	std::list<Sheet*> SheetManager::getSheets()
	{
		std::list<Sheet*> sheets;

		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			// Only add sheets that are not currently queued for deletion.
			if(std::find(mFreeList.begin(),mFreeList.end(),(*it)) == mFreeList.end())
				sheets.push_back((*it));
		}

		return sheets;
	}

	void SheetManager::resizeAllSheets(Size s)
	{
		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
			(*it)->setSize(s);
	}

	void SheetManager::resizeAllSheets(int widthInPixels, int heightInPixels)
	{
		resizeAllSheets(Size(widthInPixels,heightInPixels));
	}
}
