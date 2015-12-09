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
