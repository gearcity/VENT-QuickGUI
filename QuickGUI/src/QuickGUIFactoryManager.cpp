#include "QuickGUIFactoryManager.h"
#include "QuickGUIDescFactory.h"
#include "QuickGUIWidgetFactory.h"

#include "OgreMemoryAllocatorConfig.h"

template<> QuickGUI::FactoryManager* Ogre::Singleton<QuickGUI::FactoryManager>::msSingleton = 0;

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
		return msSingleton; 
	}

	FactoryManager& FactoryManager::getSingleton(void) 
	{ 
		assert( msSingleton );  
		return ( *msSingleton ); 
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
