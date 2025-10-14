#include "QuickGUIEventHandlerManager.h"

template<> QuickGUI::EventHandlerManager* Ogre::Singleton<QuickGUI::EventHandlerManager>::msSingleton = 0;

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
		return msSingleton;
	}

	EventHandlerManager& EventHandlerManager::getSingleton(void) 
	{ 
		assert( msSingleton );
		return ( *msSingleton ); 
	}

	void EventHandlerManager::executEventHandler(const Ogre::String& handlerName, EventArgs& args)
	{
#if USEHASHMAPS
	std::unordered_map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);
#else
	std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);
#endif
		
		if(it != mUserDefinedEventHandlers.end())
			mUserDefinedEventHandlers[handlerName]->execute(args);
	}

	bool EventHandlerManager::hasEventHandler(const Ogre::String& handlerName)
	{
		return (mUserDefinedEventHandlers.find(handlerName) != mUserDefinedEventHandlers.end());
	}

	void EventHandlerManager::registerEventHandler(const Ogre::String& handlerName, EventHandlerSlot* function)
	{
#if USEHASHMAPS
			std::unordered_map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);
#else
			std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);
#endif
		
		if(it != mUserDefinedEventHandlers.end())
			OGRE_DELETE_T(it->second,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);

		mUserDefinedEventHandlers[handlerName] = function;
	}

	void EventHandlerManager::unregisterEventHandler(const Ogre::String& handlerName)
	{
#if USEHASHMAPS
			std::unordered_map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);
#else
			std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);
#endif
		
		if(it != mUserDefinedEventHandlers.end())
		{
			EventHandlerSlot* s = it->second;
			mUserDefinedEventHandlers.erase(it);
			OGRE_DELETE_T(s,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}
}
