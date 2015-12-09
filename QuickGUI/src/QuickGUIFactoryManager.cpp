#include "QuickGUIFactoryManager.h"
#include "QuickGUIDescFactory.h"
#include "QuickGUIWidgetFactory.h"

#include "OgreMemoryAllocatorConfig.h"

template<> QuickGUI::FactoryManager* Ogre::Singleton<QuickGUI::FactoryManager>::ms_Singleton = 0;

namespace QuickGUI
{
	FactoryManager::FactoryManager()
	{
		mWidgetFactory = new WidgetFactory();
		mDescFactory = new DescFactory();
	}

	FactoryManager::~FactoryManager()
	{
		delete mWidgetFactory;
		delete mDescFactory;
	}

	FactoryManager* FactoryManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	FactoryManager& FactoryManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	WidgetFactory* FactoryManager::getWidgetFactory()
	{
		return mWidgetFactory;
	}

	DescFactory* FactoryManager::getDescFactory()
	{
		return mDescFactory;
	}
}
