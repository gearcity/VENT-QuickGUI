/** Templated Member Function Pointer.
	@remarks
	This code is based off CEGUI's implementation for event handlers.
	(known as subscribers)  A EventHandlerPointer must have its
	type known during declaration.  In order to create lists of
	EventHandlerPointers, the EventHandlerSlot was created.
	@note
	Originally the execute function was done via overloading the () operator.
*/

#ifndef QUICKGUIEVENTHANDLERPOINTER_H
#define QUICKGUIEVENTHANDLERPOINTER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIEventArgs.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	class _QuickGUIExport EventHandlerSlot
	{
	public:
		virtual ~EventHandlerSlot() {};
		virtual void execute(const EventArgs& args) = 0;
		virtual void* getClass() = 0;
	};

	template<typename T>
	class EventHandlerPointer :
		public EventHandlerSlot
	{
	public:
		typedef void (T::*EventHandler)(const EventArgs&);
	public:
		EventHandlerPointer() :
			d_undefined(true),
			d_object(NULL)
		{}
		EventHandlerPointer(EventHandler func, T* obj) :
			d_function(func),
			d_object(obj),
			d_undefined(false)
		{}
		virtual ~EventHandlerPointer() {}

		void execute(const EventArgs& args)
		{
			if(!d_undefined) 
				(d_object->*d_function)(args);
		}

		void* getClass()
		{
			return dynamic_cast<void*>(d_object);
		}

	protected:
		EventHandler	d_function;
		T*				d_object;
		bool			d_undefined;
	};
}

#endif
