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

#ifndef QUICKGUITITLEBAR_H
#define QUICKGUITITLEBAR_H

#include "QuickGUIComponentWidget.h"
#include "QuickGUITextUser.h"

namespace QuickGUI
{
	// forward declaration
	class Button;

	class _QuickGUIExport TitleBarDesc :
		public ComponentWidgetDesc,
		public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		TitleBarDesc();
		virtual ~TitleBarDesc() {}
	public:

		bool titlebar_closeButton;
		float titlebar_closeButtonPadding;
		Ogre::String titlebar_closeButtonSkinType;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TitleBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TitleBar"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();
	};

	class TitleBar :
		public ComponentWidget,
		public TextUser
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String CLOSE_BUTTON;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;
		friend class Window;
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		TitleBar(const Ogre::String& name);
		virtual ~TitleBar();

		Button* mCloseButton;

		TitleBarDesc* mDesc;

		int mHeightBuffer;

		/**
		* Hides the window.  Only relevant if this TitleBar has a close button.
		*/
		void onCloseButtonUp(const EventArgs& args);
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
