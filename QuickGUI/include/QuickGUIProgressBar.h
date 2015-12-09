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

#ifndef QUICKGUIPROGRESSBAR_H
#define QUICKGUIPROGRESSBAR_H

#include "QuickGUIWidget.h"
#include "QuickGUITextUser.h"

#include "OgreImage.h"

#include <vector>

namespace Ogre
{
	// forward declarations
	class Font;
	class Texture;
}

namespace QuickGUI
{
	class _QuickGUIExport ProgressBarDesc :
			public WidgetDesc,
			public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		ProgressBarDesc();
		virtual ~ProgressBarDesc() {}
	public:

		ProgressBarFillDirection progressbar_fillDirection;
		ProgressBarLayout progressbar_layout;
		ProgressBarClippingEdge progressbar_clippingEdge;

		/// Percentage of progressbar_progress of the bar. (0-100)
		float progressbar_progress;

		Ogre::String progressbar_userHandlers[PROGRESSBAR_EVENT_COUNT];

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ProgressBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ProgressBar"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ProgressBar :
		public Widget,
		public TextUser
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String BAR;
		static const Ogre::String CLIPMAP;
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
		void addUserDefinedProgressBarEventHandler(ProgressBarEvent EVENT, const Ogre::String& handlerName);
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
		template<typename T> void addProgressBarEventHandler(ProgressBarEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addProgressBarEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addProgressBarEventHandler(ProgressBarEvent EVENT, EventHandlerSlot* function);

		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedProgressBarEventHandler(ProgressBarEvent EVENT);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireProgressBarEvent(ProgressBarEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the side of the bar texture that is clipped to simulate progressbar_progress.
		*/
		ProgressBarClippingEdge getClippingEdge();
		/**
		* Gets the direction the progressbar_progress bar fills, as it gains progressbar_progress.
		*/
		ProgressBarFillDirection getFillDirection();
		/**
		* Gets the progressbar_progress of the ProgressBar, that is a visual indicator of percent complete.
		*/
		float getProgress();

		/**
		* Removes all Event Handlers registered by the given object.
		*/
		virtual void removeEventHandlers(void* obj);

		/**
		* Gets the axis of progressbar_progress growth, either horizontal or vertical.
		*/
		ProgressBarLayout setLayout();		
		/**
		* Sets the side of the bar texture that is clipped to simulate progressbar_progress.
		*/
		void setClippingEdge(ProgressBarClippingEdge e);
		/**
		* Sets the direction the progressbar_progress bar fills, as it gains progressbar_progress.
		*/
		void setFillDirection(ProgressBarFillDirection d);
		/**
		* Sets the axis of progressbar_progress growth, either horizontal or vertical.
		*/
		void setLayout(ProgressBarLayout l);
		/**
		* Sets the progressbar_progress of the ProgressBar, that is a visual indicator of percent complete.
		* NOTE: values will be capped to range [0.0,100.0].
		*/
		void setProgress(float percent);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

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
		using Widget::setTransparencyPicking;
		using Widget::setVerticalAnchor;
		using Widget::setVisible;
		using Widget::setWidth;

	protected:
		ProgressBar(const Ogre::String& name);
		virtual ~ProgressBar();

		/// Pointer pointing to mWidgetDesc object, but casted for quick use.
		ProgressBarDesc* mDesc;

		void _processClipMap();
		/// Array of bools tracking transparent and non-transparent pixels of the clipmap.
		std::vector<bool> mClipMap;

		/// Modified texture to draw on top of background
		Ogre::Texture* mOutputBarTexture;

		Ogre::Image mBarImage;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mProgressBarEventHandlers[PROGRESSBAR_EVENT_COUNT];

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
