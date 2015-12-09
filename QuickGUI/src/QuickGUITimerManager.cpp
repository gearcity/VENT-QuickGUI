#include "QuickGUITimerManager.h"

template<> QuickGUI::TimerManager* Ogre::Singleton<QuickGUI::TimerManager>::msSingleton = 0;

namespace QuickGUI
{
	TimerManager::TimerManager()
	{
		Ogre::Root::getSingleton().addFrameListener(this);
	}

	TimerManager::~TimerManager()
	{
		Ogre::Root::getSingleton().removeFrameListener(this);

		for(std::vector<Timer*>::iterator it = mTimers.begin(); it != mTimers.end(); ++it)
			OGRE_DELETE_T((*it),Timer,Ogre::MEMCATEGORY_GENERAL);
	}

	TimerManager* TimerManager::getSingletonPtr(void) 
	{ 
		return msSingleton; 
	}

	TimerManager& TimerManager::getSingleton(void) 
	{ 
		assert( msSingleton );  
		return ( *msSingleton ); 
	}

	bool TimerManager::frameStarted(const Ogre::FrameEvent& evt)
	{
		for(std::vector<Timer*>::iterator it = mTimers.begin(); it != mTimers.end(); ++it)
			(*it)->update(evt.timeSinceLastFrame);

		return true;
	}

	Timer* TimerManager::createTimer(TimerDesc& d)
	{
		Timer* newTimer = OGRE_NEW_T(Timer,Ogre::MEMCATEGORY_GENERAL)(d);
		mTimers.push_back(newTimer);
		return newTimer;
	}

	void TimerManager::destroyTimer(Timer* t)
	{
		for(std::vector<Timer*>::iterator it = mTimers.begin(); it != mTimers.end(); ++it)
		{
			if((*it) == t)
			{
				Timer* t = (*it);
				mTimers.erase(it);
				OGRE_DELETE_T(t,Timer,Ogre::MEMCATEGORY_GENERAL);
				return;
			}
		}
	}
}
