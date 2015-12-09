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

#ifndef QUICKGUITABDESC_H
#define QUICKGUITABDESC_H

/**
* Descs in this class are not forward declared, however their corresponding Widgets
* are.
*/

#include "QuickGUIContainerWidget.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	class _QuickGUIExport PanelDesc :
			public ContainerWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		PanelDesc();
		virtual ~PanelDesc() {}
	public:

		virtual Ogre::String getClass() { return "PanelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Panel"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		//virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TabPageDesc :
			public PanelDesc
	{
	public:
		friend class DescFactory;
	protected:
		TabPageDesc();
		virtual ~TabPageDesc() {}
	public:

		/// Describes the Text used in this Label
		TextDesc textDesc;

		/// width in pixels of the front edge of the tab.
		float					tabpage_tabFrontWidth;
		Ogre::String			tabpage_tabImageName;

		int						tabpage_index;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TabPageDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TabPage"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TabControlDesc :
			public ContainerWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		TabControlDesc();
		virtual ~TabControlDesc() {}
	public:

		int				tabcontrol_selectedTab;
		/// Amount of overlap between Tabs
		float			tabcontrol_tabOverlap;
		/// Pixel height of Tabs
		float			tabcontrol_tabHeight;
		/// Determines whether tabs can be reordered using click and drag
		bool			tabcontrol_tabReordering;
		Ogre::String	tabcontrol_userHandlers[TABCONTROL_EVENT_COUNT];

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TabControlDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TabControl"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};
}

#endif
