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

#ifndef QUICKGUITREEVIEWCHECKBOXNODE_H
#define QUICKGUITREEVIEWCHECKBOXNODE_H

#include "QuickGUITreeViewNode.h"

namespace QuickGUI
{
	// forward declarations
	class CheckBox;

	class _QuickGUIExport TreeViewCheckBoxNodeDesc :
		public TreeViewNodeDesc
	{
	public:
		friend class DescFactory;
	protected:
		TreeViewCheckBoxNodeDesc();
		virtual ~TreeViewCheckBoxNodeDesc() {}
	public:

		bool treeviewcheckboxnode_checked;
		Ogre::String treeviewcheckboxnode_userHandlers[TREEVIEWCHECKBOXNODE_EVENT_COUNT];

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TreeViewCheckBoxNodeDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TreeViewCheckBoxNode"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TreeViewCheckBoxNode :
		public TreeViewNode
	{
	public:
		// Skin Constants
		static const Ogre::String CHECKBOX;
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
		void addUserDefinedTreeViewCheckBoxNodeEventHandler(TreeViewCheckBoxNodeEvent EVENT, const Ogre::String& handlerName);
		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WIDGET_EVENT_SIZE_CHANGED, WIDGET_EVENT_MOUSE_WHEEL, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WIDGET_EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addTreeViewCheckBoxNodeEventHandler(TreeViewCheckBoxNodeEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addTreeViewCheckBoxNodeEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addTreeViewCheckBoxNodeEventHandler(TreeViewCheckBoxNodeEvent EVENT, EventHandlerSlot* function);

		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedTreeViewCheckBoxNodeEventHandler(TreeViewCheckBoxNodeEvent EVENT);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireTreeViewCheckBoxNodeEvent(TreeViewCheckBoxNodeEvent e, EventArgs& args);

		/**
		* Returns true if checked, false otherwise.
		*/
		bool getChecked();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

		/**
		* Removes all Event Handlers registered by the given object.
		*/
		virtual void removeEventHandlers(void* obj);

		/**
		* Sets whether this checkbox is checked or not.
		*/
		void setChecked(bool checked);

		/**
		* Recalculate Client widget_dimensions, relative to Widget's actual widget_dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		TreeViewCheckBoxNode(const Ogre::String& name);
		virtual ~TreeViewCheckBoxNode();

		TreeViewCheckBoxNodeDesc* mDesc;

		CheckBox* mCheckBox;

		// TreeViewCheckBoxNode specific event handlers
		std::vector<EventHandlerSlot*> mTreeViewCheckBoxNodeEventHandlers[TREEVIEWCHECKBOXNODE_EVENT_COUNT];

		void onCheckChanged(const EventArgs& args);
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
	};
}

#endif
