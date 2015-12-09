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

#ifndef QUICKGUITOOLBARITEM_H
#define QUICKGUITOOLBARITEM_H

#include "QuickGUIContainerWidget.h"

namespace QuickGUI
{
	// forward declaration
	class ToolBar;

	class _QuickGUIExport ToolBarItemDesc :
			public ContainerWidgetDesc
	{
	public:
		friend class ToolBar;
		friend class DescFactory;
	protected:
		ToolBarItemDesc();
		virtual ~ToolBarItemDesc() {}
	public:

		int index;

		/// ToolBar that this Widget belongs to
		ToolBar* toolBar;

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ToolBarItem :
		public ContainerWidget
	{
	public:
		friend class ToolBar;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Queues this widget for destruction, likely to happen in the next frame, before drawing occurs.
		* NOTE: Widgets that are components of another widget cannot be destroyed using this API.
		*/
		virtual void destroy();

		/**
		* Returns true if this widget is able to receive injected input events,
		* false otherwise.
		* NOTE: Disabled widgets can still have events fired manually.
		*/
		bool getEnabled();
		/**
		* Returns the index of the MenuItem in its owner List.
		*/
		int getIndex();

		/**
		* Returns true if this widget's position is managed by another widget.
		*/
		virtual bool isPositionManaged();
		/**
		* Returns true if this widget's size is managed by another widget.
		*/
		virtual bool isSizeManaged();
		/**
		* Returns true if this widget is a ToolBarItem, false otherwise.
		*/
		virtual bool isToolBarItem();

		/**
		* Enabled Widgets receive mouse and keyboard events via injections to the GUIManager.
		* Disabled Widgets can only receive these events if they are manually fired.
		*/
		virtual void setEnabled(bool enabled);

	protected:
		ToolBarItem(const Ogre::String& name);
		virtual ~ToolBarItem();

		/**
		* Updates the ToolBar Link.
		*/
		virtual void notifyToolBarParent(ToolBar* b);

		/// Derivative ToolBar Items will implement the following functions to define their default behavior.
		virtual void onMouseButtonDown(const EventArgs& args);
		virtual void onMouseButtonUp(const EventArgs& args);
		virtual void onMouseEnter(const EventArgs& args);
		virtual void onMouseLeave(const EventArgs& args);

		/**
		* Sets the index of the MenuItem
		*/
		void setIndex(unsigned int index);

	private:
	};
}

#endif
