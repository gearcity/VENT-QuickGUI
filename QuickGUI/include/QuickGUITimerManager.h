#ifndef QUICKGUITIMERMANAGER_H
#define QUICKGUITIMERMANAGER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUITimer.h"

#include "OgreFrameListener.h"
#include "OgreRoot.h"
#include "OgreSingleton.h"

#include <vector>

namespace QuickGUI
{
	class _QuickGUIExport TimerManager :
		public Ogre::Singleton<TimerManager>,
		public Ogre::GeneralAllocatedObject,
		public Ogre::FrameListener
	{
	public:
		TimerManager();
		virtual ~TimerManager();

		static TimerManager& getSingleton(void);
		static TimerManager* getSingletonPtr(void);

		virtual bool frameStarted(const Ogre::FrameEvent& evt);

		Timer* createTimer(TimerDesc& d);

		void destroyTimer(Timer* t);

	protected:
		std::vector<Timer*> mTimers;

	private:
	};
}

#endif
