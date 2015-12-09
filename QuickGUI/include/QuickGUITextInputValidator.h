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

#ifndef QUICKGUITEXTINPUTVALIDATOR_H
#define QUICKGUITEXTINPUTVALIDATOR_H

#include "QuickGUIExportDLL.h"

#include "OgreUTFString.h"

namespace QuickGUI
{
	class _QuickGUIExport TextInputValidatorSlot
	{
	public:
		virtual ~TextInputValidatorSlot() {};
		virtual bool isInputValid(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText) = 0;
	};

	template<typename T>
	class TextInputValidatorPointer :
		public TextInputValidatorSlot
	{
	public:
		typedef bool (T::*TextInputValidator)(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText);
	public:
		TextInputValidatorPointer() :
			d_undefined(true),
			d_object(NULL)
		{}
		TextInputValidatorPointer(TextInputValidator func, T* obj) :
			d_function(func),
			d_object(obj),
			d_undefined(false)
		{}
		virtual ~TextInputValidatorPointer() {}

		bool isInputValid(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText)
		{
			if(!d_undefined) 
				return (d_object->*d_function)(cp,index,currentText);

			return true;
		}

	protected:
		TextInputValidator	d_function;
		T*					d_object;
		bool				d_undefined;
	};
}

#endif
