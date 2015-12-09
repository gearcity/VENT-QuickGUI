#ifndef QUICKGUITIMER_H
#define QUICKGUITIMER_H

#include "QuickGUIExportDLL.h"

#include "OgrePrerequisites.h"

#include <stdio.h>

namespace QuickGUI
{
	class _QuickGUIExport TimerCallbackSlot
	{
	public:
		virtual ~TimerCallbackSlot() {};
		virtual void execute() = 0;
	};

	template<typename T>
	class TimerCallbackPointer :
		public TimerCallbackSlot
	{
	public:
		typedef void (T::*TimerCallback)();
	public:
		TimerCallbackPointer() :
			d_undefined(true)
		{}
		TimerCallbackPointer(TimerCallback func, T* obj) :
			d_function(func),
			d_object(obj),
			d_undefined(false)
		{}
		virtual ~TimerCallbackPointer() {}

		void execute()
		{
			if(!d_undefined) 
				(d_object->*d_function)();
		}
	protected:
		TimerCallback	d_function;
		T*				d_object;
		bool			d_undefined;
	};

	class _QuickGUIExport TimerDesc
	{
	public:
		TimerDesc();

		/// Whether the timer will continue to call the callback every period, or just once.
		bool repeat;
		/// Amount of time in seconds until Timer callback is called. -1 causes callback to fire every frame.
		float timePeriod;
	};

	class _QuickGUIExport Timer
	{
	public:
		friend class TimerManager;
	public:

		/**
		* Clears the callback function called when the timer has exceeded the set period of time.
		*/
		void clearCallback();
		/**
		* Returns true if this timer will continue to execute its callback method every period,
		* false otherwise.
		*/
		bool getRepeat();
		/**
		* Returns the amount of time needed to execute the timer's callback method.
		*/
		float getTimePeriod();

		/**
		* Returns true if the Timer is accumulating time, false otherwise.
		*/
		bool isUpdating();
	
		/**
		* Resets the timer accumulator.
		*/
		void reset();
		/**
		* Resumes capturing of time.
		*/
		void resume();

		/**
		* Sets the method to callback when timer has exceeded the set period of time.
		*/
		template<typename T> void setCallback(void (T::*function)(), T* obj)
		{
			mCallback = OGRE_NEW_T(TimerCallbackPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj);
		}
		/**
		* Sets the method to callback when timer has exceeded the set period of time.
		*/
		void setCallback(TimerCallbackSlot* p);
		/**
		* If set to true, the timer will continue to execute its callback method every period, if set.
		*/
		void setRepeat(bool repeat);
		/**
		* Sets the amount of time until the callback is fired.
		*/
		void setTimePeriod(float timeInSeconds);
		/**
		* Resets timer accumulator and starts capturing time.
		*/
		void start();
		/**
		* Stops capturing time.
		*/
		void stop();

	protected:
		Timer(TimerDesc& d);
		~Timer();

		void update(float timeInSeconds);

		TimerDesc mDesc;

		/// Keeps track of time that has elapsed since timer started or last fired callback
		float mTimeAccumulator;
		/// If true, we increment Accumulator
		bool mUpdate;
		/// Callback to fire
		TimerCallbackSlot* mCallback;

	private:
	};
}

#endif
