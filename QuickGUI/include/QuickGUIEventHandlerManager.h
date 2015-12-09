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

#ifndef QUICKGUIEVENTHANDLERMANAGER_H
#define QUICKGUIEVENTHANDLERMANAGER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIEventHandlerPointer.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	/*
	* This manager manages user-defined Event Handlers.
	*/
	class _QuickGUIExport EventHandlerManager :
		public Ogre::Singleton<EventHandlerManager>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class Root;
	public:
		static EventHandlerManager& getSingleton(void); 
		static EventHandlerManager* getSingletonPtr(void);

		/** 
		* Executes the event handler registered under the name given.
		* NOTE: If there is no event registered under this name, nothing handler will be called.
		*/
		void executEventHandler(const Ogre::String& handlerName, EventArgs& args);

		/** 
		* Returns true if a handler is registered under the name given, false otherwise.
		*/
		bool hasEventHandler(const Ogre::String& handlerName);

		/** Registers an event handler
			@param
				handlerName Name used to identify the handler.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return void, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WIDGET_EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
			@note
				Handlers are not called unless the Handler is added to a Widget for a particular Event.
			@note
				If a handler already exists with the name provided, it is overwritten.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::EventHandlerManager::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void registerEventHandler(const Ogre::String& handlerName, void (T::*function)(const EventArgs&), T* obj)
		{
			registerEventHandler(handlerName,OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void registerEventHandler(const Ogre::String& handlerName, EventHandlerSlot* function);

		/**
		* Removes the handler registered under the name given.
		*/
		void unregisterEventHandler(const Ogre::String& handlerName);

	protected:
		EventHandlerManager();
		virtual ~EventHandlerManager();

		std::map<Ogre::String,EventHandlerSlot*> mUserDefinedEventHandlers;

	private:
	};
}

#endif
