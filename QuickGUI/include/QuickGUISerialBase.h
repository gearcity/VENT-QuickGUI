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

#ifndef QUICKGUISERIALBASE_H
#define QUICKGUISERIALBASE_H

#include "QuickGUIAnchors.h"
#include "QuickGUIBrushEnums.h"
#include "QuickGUIConsoleEnums.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIProgressBarEnums.h"
#include "QuickGUIRect.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIScrollBarButtonLayouts.h"
#include "QuickGUITextAlignment.h"
#include "QuickGUIToolBarEnums.h"
#include "QuickGUIOgreEquivalents.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	// forward declaration
	class ScriptDefinition;

	/**
	* Abstract class setting the foundation for the 
	* SerialWriter and SerialReader class.
	*/
	class _QuickGUIExport SerialBase
	{
	public:
		SerialBase();

		virtual bool begin(const Ogre::String& definitionType, const Ogre::String& definitionID) = 0;

		virtual void end() = 0;

		ScriptDefinition* getCurrentDefinition();

		virtual bool isSerialWriter() = 0;
		virtual bool isSerialReader() = 0;
		// Serialization of properties
		virtual void IO(const Ogre::String& propertyName, unsigned short* member, unsigned short defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, BrushFilterMode* member, BrushFilterMode defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ConsoleLayout* member, ConsoleLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, int* member, int defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, unsigned int* member, unsigned int defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ColourValue* member, ColourValue defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ToolBarItemLayout* member, ToolBarItemLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, Ogre::String* member, Ogre::String defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, bool* member, bool defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, float* member, float defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, Point* member, Point defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarFillDirection* member, ProgressBarFillDirection defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarLayout* member, ProgressBarLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member, ProgressBarClippingEdge defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, Rect* member, Rect defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member, VScrollBarButtonLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member, HScrollBarButtonLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, Size* member, Size defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, HorizontalTextAlignment* member, HorizontalTextAlignment defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, HorizontalAnchor* member, HorizontalAnchor defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, VerticalAnchor* member, VerticalAnchor defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, VerticalTextAlignment* member, VerticalTextAlignment defaultValue) = 0;

	protected:
		ScriptDefinition* mCurrentDefinition;

	private:
	};
}

#endif
