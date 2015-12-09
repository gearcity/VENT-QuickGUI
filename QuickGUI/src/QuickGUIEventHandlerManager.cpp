#include "QuickGUIEventHandlerManager.h"

template<> QuickGUI::EventHandlerManager* Ogre::Singleton<QuickGUI::EventHandlerManager>::ms_Singleton = 0;

namespace QuickGUI
{
	EventHandlerManager::EventHandlerManager()
	{
	}

	EventHandlerManager::~EventHandlerManager()
	{
	}

	EventHandlerManager* EventHandlerManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	EventHandlerManager& EventHandlerManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	void EventHandlerManager::executEventHandler(const Ogre::String& handlerName, EventArgs& args)
	{
		std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);

		if(it != mUserDefinedEventHandlers.end())
			mUserDefinedEventHandlers[handlerName]->execute(args);
	}

	bool EventHandlerManager::hasEventHandler(const Ogre::String& handlerName)
	{
		return (mUserDefinedEventHandlers.find(handlerName) != mUserDefinedEventHandlers.end());
	}

	void EventHandlerManager::registerEventHandler(const Ogre::String& handlerName, EventHandlerSlot* function)
	{
		std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);

		if(it != mUserDefinedEventHandlers.end())
			OGRE_DELETE_T(it->second,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);

		mUserDefinedEventHandlers[handlerName] = function;
	}

	void EventHandlerManager::unregisterEventHandler(const Ogre::String& handlerName)
	{
		std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);

		if(it != mUserDefinedEventHandlers.end())
		{
			EventHandlerSlot* s = it->second;
			mUserDefinedEventHandlers.erase(it);
			OGRE_DELETE_T(s,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}
}
