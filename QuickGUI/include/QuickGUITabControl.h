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

#ifndef QUICKGUITABCONTROL_H
#define QUICKGUITABCONTROL_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIForwardDeclaredDesc.h"

namespace QuickGUI
{
	// forward declarations
	class TabPage;

	class _QuickGUIExport TabControl :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Adds a user-defined event handler to this widget.  The difference between this function and the addWidgetEventHandler
		* is that the handlerName gets serialized with the Widget. This means you can defined handlers outside the creation of the
		* GUI Layout, and when the sheet is loaded from disk, all of the handlers will work properly.
		* NOTE: Only 1 user defined Event Handler can be assigned to an Event. Old values will be overwritten.
		* NOTE: A handler of "" essentially removes the handler.
		* NOTE: If no eventHandler exists with the name provided, nothing will happen.
		*/
		void addUserDefinedTabControlEventHandler(TabControlEvent EVENT, const Ogre::String& handlerName);
		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: BUTTON_EVENT_STATE_CHANGED, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::BUTTON_EVENT_STATE_CHANGED,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addTabControlEventHandler(TabControlEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addTabControlEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addTabControlEventHandler(TabControlEvent EVENT, EventHandlerSlot* function);

		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedTabControlEventHandler(TabControlEvent EVENT);
		/**
		* Creates a child TabPage.
		*/
		TabPage* createTabPage(int index = -1);
		TabPage* createTabPage(const Ogre::UTFString& text, int index = -1);
		TabPage* createTabPage(std::vector<TextSegment> segments, int index = -1);
		TabPage* createTabPageWithIcon(const Ogre::String& imageName, int index = -1);
		TabPage* createTabPageWithIcon(const Ogre::String& imageName, const Ogre::UTFString& text, int index = -1);
		TabPage* createTabPageWithIcon(const Ogre::String& imageName, std::vector<TextSegment> segments, int index = -1);

		/**
		* Removes an TabPage from the TabControl.
		*/
		void destroyTabPage(unsigned int index);

		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, unsigned int queryFilter = -1, bool ignoreDisabled = true);
		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireTabControlEvent(TabControlEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the currently selected TabPage.
		*/
		TabPage* getSelectedTabPage();
		/**
		* Returns the tab height of each TabPage in this TabControl.
		*/
		float getTabHeight();
		/**
		* Returns the amount of overlap each tab has over the previous one.
		*/
		float getTabOverlap();
		/**
		* Returns the TabPage at the index given.  If there are no Tabs, NULL is returned.
		* if the index is out of bounds, the last Tab is returned.
		*/
		TabPage* getTabPage(int index);
		/**
		* Returns true if the Tabs can be reordered by mouse clicking and dragging.
		*/
		bool getTabReordering();

		/**
		* Removes all Event Handlers registered by the given object.
		*/
		virtual void removeEventHandlers(void* obj);
		/**
		* Moves a TabPage within the collection of tabs.
		*/
		void reorderTabPage(TabPage* p, int index);

		/**
		* Selects the TabPage given.
		*/
		void selectTabPage(TabPage* p);
		void selectTabPage(int index);
		/**
		* Sets the tab height of each TabPage in this TabControl.
		*/
		void setTabHeight(float height);
		/**
		* Sets the amount of overlap each tab has over the previous one.
		*/
		void setTabOverlap(float overlap);
		/**
		* Sets whether tabs can be reorderd using click and drag.
		*/
		void setTabReordering(bool reordering);
		/**
		* Sets whether the widget will receive mouse over events simply by having the mouse over
		* its texture widget_dimensions, or only when the cursor is over non transparent parts.
		*/
		virtual void setTransparencyPicking(bool transparencyPicking);

		/**
		* Updates TabPage names to make sure they are based off their index,
		* and sets the position of each tab.
		*/
		void updateTabPositions();

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::drag;
		using Widget::resize;
		using Widget::setConsumeKeyboardEvents;
		using Widget::setDimensions;
		using Widget::setDragable;
		using Widget::setHeight;
		using Widget::setHorizontalAnchor;
		using Widget::setMaxSize;
		using Widget::setMinSize;
		using Widget::setPosition;
		using Widget::setPositionRelativeToParentClientDimensions;
		using Widget::setQueryFlags;
		using Widget::setResizeFromAllSides;
		using Widget::setResizeFromBottom;
		using Widget::setResizeFromLeft;
		using Widget::setResizeFromRight;
		using Widget::setResizeFromTop;
		using Widget::setScrollable;
		using Widget::setSerialize;
		using Widget::setSize;
		using Widget::setVerticalAnchor;
		using Widget::setVisible;	
		using Widget::setWidth;

	protected:
		TabControl(const Ogre::String& name);
		virtual ~TabControl();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		TabControlDesc* mDesc;

		TabPage* mSelectedTab;

		std::list<TabPage*> mTabs;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mTabControlEventHandlers[TABCONTROL_EVENT_COUNT];

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		TabPage* createTabPage(TabPageDesc* d, int index = -1);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
	private:
	};
}

#endif
