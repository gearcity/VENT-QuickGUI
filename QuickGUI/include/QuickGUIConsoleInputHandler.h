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

#ifndef QUICKGUICONSOLEINPUTHANDLER_H
#define QUICKGUICONSOLEINPUTHANDLER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIText.h"

#include "OgreUTFString.h"

#include <vector>

namespace QuickGUI
{
	// forward declaration
	class Console;

	class _QuickGUIExport ConsoleInputHandlerSlot
	{
	public:
		virtual ~ConsoleInputHandlerSlot() {};
		virtual void onTextSubmitted(Console* console, bool& clearInputBox, bool& addToDisplayArea) = 0;
	};

	template<typename T>
	class ConsoleInputHandlerPointer :
		public ConsoleInputHandlerSlot
	{
	public:
		typedef void (T::*ConsoleInputHandler)(Console* console, bool& clearInputBox, bool& addToDisplayArea);
	public:
		ConsoleInputHandlerPointer() :
			d_undefined(true),
			d_object(NULL)
		{}
		ConsoleInputHandlerPointer(ConsoleInputHandler func, T* obj) :
			d_function(func),
			d_object(obj),
			d_undefined(false)
		{}
		virtual ~ConsoleInputHandlerPointer() {}

		void onTextSubmitted(Console* console, bool& clearInputBox, bool& addToDisplayArea)
		{
			if(!d_undefined) 
				(d_object->*d_function)(console,clearInputBox,addToDisplayArea);
		}

	protected:
		ConsoleInputHandler	d_function;
		T*					d_object;
		bool				d_undefined;
	};
}

#endif
