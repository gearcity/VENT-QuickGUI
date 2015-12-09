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

#ifndef QUICKGUIVSCROLLBAR_H
#define QUICKGUIVSCROLLBAR_H

#include "QuickGUIComponentWidget.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIScrollBarButtonLayouts.h"

namespace QuickGUI
{
	// forward declarations
	class Button;

	class _QuickGUIExport VScrollBarDesc :
			public ComponentWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		VScrollBarDesc();
		virtual ~VScrollBarDesc() {}
	public:

		/// The amount the slider slides when an up/down button is clicked.
		float vscrollbar_buttonScrollPercent;
		/// The amount the slider slides when the bar is clicked.
		float vscrollbar_barScrollPercent;

		VScrollBarButtonLayout vscrollbar_scrollBarButtonLayout;
		/// Height of the slider in pixels.
		float vscrollbar_sliderHeight;
		/// Position of the slider as a percentage. 0 is at the top, 100 is at the bottom. (as far down as it can go)
		float vscrollbar_sliderPercentage;
		Ogre::String vscrollbar_userHandlers[SCROLLBAR_EVENT_COUNT];

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "VScrollBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "VScrollBar"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport VScrollBar :
		public ComponentWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BAR;
		static const Ogre::String UP1;
		static const Ogre::String UP2;
		static const Ogre::String DOWN1;
		static const Ogre::String DOWN2;
		static const Ogre::String SLIDER;
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
		void addUserDefinedScrollBarEventHandler(ScrollBarEvent EVENT, const Ogre::String& handlerName);
		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WINDOW_EVENT_FOCUS_LOST, WINDOW_EVENT_FOCUS_GAINED, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WINDOW_EVENT_FOCUS_LOST,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addScrollBarEventHandler(ScrollBarEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addScrollBarEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addScrollBarEventHandler(ScrollBarEvent EVENT, EventHandlerSlot* function);

		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedScrollBarEventHandler(ScrollBarEvent EVENT);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireScrollBarEvent(ScrollBarEvent e, EventArgs& args);

		/**
		* Gets the amount the sliders moves when the bar is clicked.
		*/
		float getBarScrollPercent();
		/**
		* Returns the Button layout of the ScrollBar.
		*/
		VScrollBarButtonLayout getButtonLayout();
		/**
		* Gets the amount the slider moves when the up/down arrows are clicked.
		*/
		float getButtonScrollPercent();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the position of the slider relative to the bar bounds. 0 percent is at the top, 100 at the bottom.
		*/
		float getPercentage();
		/**
		* Returns the dimensions the slider can move in, relative to the ScrollBar's dimensions.
		*/
		Rect getSliderBounds();

		/**
		* Removes all Event Handlers registered by the given object.
		*/
		virtual void removeEventHandlers(void* obj);

		/**
		* Scrolls down by ButtonScrollPercent.
		*/
		void scrollDown();
		/**
		* If true, slider will be positioned at bottom-most position within scrolling area.
		* NOTE: we do not need an equivalent "scrollToBeginning", since you can simply call setPercentage(0).
		*/
		void scrollToEnd();
		/**
		* Scrolls up by ButtonScrollPercent.
		*/
		void scrollUp();
		/**
		* Sets the amount the sliders moves when the bar is clicked.
		*/
		void setBarScrollPercent(float percent);
		/**
		* Sets the Button layout of the ScrollBar.
		*/
		void setButtonLayout(VScrollBarButtonLayout l);
		/**
		* Sets the amount the slider moves when the up/down arrows are clicked.
		*/
		void setButtonScrollPercent(float percent);
		/**
		* Sets the position of the slider relative to the bar bounds. 0 percent is at the top, 100 at the bottom.
		*/
		void setPercentage(float percentage);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets the height of the slider.  Disabled ScrollBar from being Live.
		*/
		void setSliderHeight(float height);

		/**
		* Recalculate Client widget_dimensions, relative to Widget's actual widget_dimensions.
		*/
		virtual void updateClientDimensions();

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::drag;
		using Widget::resize;
		using Widget::setConsumeKeyboardEvents;
		using Widget::setDimensions;
		using Widget::setDragable;
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
		using Widget::setTransparencyPicking;
		using Widget::setVerticalAnchor;
		using Widget::setVisible;

		using ComponentWidget::setHeight;
		using ComponentWidget::setSize;
		using ComponentWidget::setWidth;

	protected:
		VScrollBar(const Ogre::String& name);
		virtual ~VScrollBar();

		VScrollBarDesc* mDesc;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mScrollBarEventHandlers[SCROLLBAR_EVENT_COUNT];

		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();

		Button* mButton_Up1;
		Button* mButton_Up2;
		Button* mButton_Down1;
		Button* mButton_Down2;
		// Toggles button visibility and defines slider bounds
		void _applyButtonLayout();

		Button* mButton_Slider;
		/// Updates the slider's position if it is outside slider bounds
		void _checkSliderPosition();

		/// The area in which the slider can reside. (defines top/bottom)
		Point mSliderBounds;
		/// Sets the Slider Bounds, according to button layout and button positions
		void _determineSliderBounds();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onSliderDragged(const EventArgs& args);

		void onUpClicked(const EventArgs& args);
		void onDownClicked(const EventArgs& args);
		void onBarClicked(const EventArgs& args);

	private:
	};
}

#endif
