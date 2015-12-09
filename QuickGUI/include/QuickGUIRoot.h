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

#ifndef QUICKGUIROOT_H
#define QUICKGUIROOT_H

#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIManager.h"

#include "OgrePrerequisites.h"
#include "OgreSingleton.h"

#include <map>

namespace Ogre
{
	// forward declarations
	class Font;
}

namespace QuickGUI
{
	// forward declarations
	class Sheet;

	class _QuickGUIExport Root :
		public Ogre::Singleton<Root>
	{
	public:
		friend class Sheet;
		// Needs access to first available GUIManager
		friend class ComboBox;
		friend class ContainerWidget;
		friend class ContextMenu;
		friend class List;
		friend class ListItem;
		friend class Menu;
		friend class MenuItem;
		friend class TabControl;
		friend class ToolBarItem;
		friend class Widget;
	public:
		Root();
		~Root();

		static Root& getSingleton(void); 
		static Root* getSingletonPtr(void);

		GUIManager* createGUIManager(GUIManagerDesc& d);

		void destroyGUIManager(GUIManager* gm);
		void destroyGUIManager(const std::string& name);

		GUIManager* getGUIManager(const std::string& name);
		/**
		* Gets the default color used by widgets on initialization.
		*/
		ColourValue getDefaultColor();
		/**
		* By default, the name of the first font found on initialization of Root is returned.
		* The default font name can be changed using "setDefaultFontName"
		*/
		Ogre::String getDefaultFontName();
		/**
		* Get the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired.
		*/
		float getDefaultHoverTime();

		/**
		* Sets the default color used by widgets on initialization.
		*/
		void setDefaultColor(const ColourValue& cv);
		/**
		* Sets the default font used by widgets on initialization.
		*/
		void setDefaultFontName(const Ogre::String& fontName);
		/**
		* Set the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired. (Commonly used for tool tips)
		*/
		void setDefaultHoverTime(float seconds);

	protected:
		ColourValue mDefaultColor;
		Ogre::Font* mDefaultFont;

		int mGUIManagerCounter;

		std::map<std::string,GUIManager*> mGUIManagers;

		void notifyWidgetDestroyed(Widget* w);
		void notifySheetDestroyed(Sheet* sheet);

		// The amount of time the cursor has to hover over a widget before
		// the ON_HOVER event is fired. (3 seconds by default)
		float mDefaultHoverTime;
	};
}

#endif
