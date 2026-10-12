#ifndef QUICKGUITEXTINPUTVALIDATOR_H
#define QUICKGUITEXTINPUTVALIDATOR_H

#include "QuickGUIExportDLL.h"

#include "QuickGUIUTFString.h"

namespace QuickGUI
{
	class _QuickGUIExport TextInputValidatorSlot
	{
	public:
		virtual ~TextInputValidatorSlot() {};
		virtual bool isInputValid(UTFString::code_point cp, unsigned int index, const UTFString& currentText) = 0;
	};

	template<typename T>
	class TextInputValidatorPointer :
		public TextInputValidatorSlot
	{
	public:
		typedef bool (T::*TextInputValidator)(UTFString::code_point cp, unsigned int index, const UTFString& currentText);
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

		bool isInputValid(UTFString::code_point cp, unsigned int index, const UTFString& currentText)
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
