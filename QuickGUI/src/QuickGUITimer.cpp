#include "QuickGUITimer.h"

#include "OgreMemoryAllocatorConfig.h"

namespace QuickGUI
{
	TimerDesc::TimerDesc()
	{
		repeat= true;
		timePeriod = -1;
	}

	Timer::Timer(TimerDesc& d) :
		mUpdate(false),
		mTimeAccumulator(0),
		mCallback(NULL)
	{
		mDesc.repeat = d.repeat;
		mDesc.timePeriod = d.timePeriod;
	}

	Timer::~Timer()
	{
		OGRE_DELETE_T(mCallback,TimerCallbackSlot,Ogre::MEMCATEGORY_GENERAL);
	}

	void Timer::clearCallback()
	{
		OGRE_DELETE_T(mCallback,TimerCallbackSlot,Ogre::MEMCATEGORY_GENERAL);
		mCallback = NULL;
	}

	bool Timer::getRepeat()
	{
		return mDesc.repeat;
	}

	float Timer::getTimePeriod()
	{
		return mDesc.timePeriod;
	}

	bool Timer::isUpdating()
	{
		return mUpdate;
	}

	void Timer::reset()
	{
		mTimeAccumulator = 0;
	}

	void Timer::resume()
	{
		mUpdate = true;
	}

	void Timer::setCallback(TimerCallbackSlot* p)
	{
		mCallback = p;
	}

	void Timer::setRepeat(bool repeat)
	{
		mDesc.repeat = repeat;
	}

	void Timer::setTimePeriod(float timeInSeconds)
	{
		mDesc.timePeriod = timeInSeconds;
	}

	void Timer::start()
	{
		mTimeAccumulator = 0;
		mUpdate = true;
	}

	void Timer::stop()
	{
		mUpdate = false;
	}

	void Timer::update(float timeInSeconds)
	{
		if(mUpdate)
		{
			mTimeAccumulator += timeInSeconds;

			if(mTimeAccumulator >= mDesc.timePeriod)
			{
				mTimeAccumulator = 0;
				if(!mDesc.repeat)
					mUpdate = false;
				if(mCallback != NULL)
					mCallback->execute();
			}
		}
	}
}
