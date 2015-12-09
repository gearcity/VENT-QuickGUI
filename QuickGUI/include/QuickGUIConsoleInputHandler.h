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
