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
