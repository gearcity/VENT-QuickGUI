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
