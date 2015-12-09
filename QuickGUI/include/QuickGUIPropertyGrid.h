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

#ifndef QUICKGUIPROPERTYGRID_H
#define QUICKGUIPROPERTYGRID_H

#include "QuickGUITreeView.h"
#include "QuickGUIText.h"

#include <list>

namespace QuickGUI
{
	// forward declarations
	class PropertyGridSection;
	class PropertyGridItem;

	class PropertyGridSectionDesc;

	class _QuickGUIExport PropertyGridDesc :
		public ContainerWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		PropertyGridDesc();
		virtual ~PropertyGridDesc() {}
	public:

		bool propertygrid_doubleClickToToggle;
		bool propertygrid_keyNavigation;
		float propertygrid_propertyFieldWidth;
		float propertygrid_itemHeight;
		Ogre::String propertygrid_userHandlers[PROPERTYGRID_EVENT_COUNT];

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGrid"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGrid :
		public ContainerWidget
	{
	public:
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String EXPAND;
		static const Ogre::String MINIMIZE;
		static const Ogre::String TEXTFIELD;
		static const Ogre::String BOOLFIELD;
		static const Ogre::String MULTIFIELD;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Item updates label width
		friend class PropertyGridItem;
		friend class PropertyGridSection;

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
		void addUserDefinedPropertyGridEventHandler(PropertyGridEvent EVENT, const Ogre::String& handlerName);
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
		template<typename T> void addPropertyGridEventHandler(PropertyGridEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addPropertyGridEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addPropertyGridEventHandler(PropertyGridEvent EVENT, EventHandlerSlot* function);

		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedPropertyGridEventHandler(PropertyGridEvent EVENT);
		/**
		* Adds a Section to the PropertyGrid at the index given.
		* NOTE: if the index is not valid, the Section is added to the end of the list.
		*/
		PropertyGridSection* createSection(int index = -1);
		PropertyGridSection* createSection(const Ogre::UTFString& text, int index = -1);
		PropertyGridSection* createSection(std::vector<TextSegment> segments, int index = -1);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool firePropertyGridEvent(PropertyGridEvent e, EventArgs& args);

		/**
		* Returns the skin name of all Boolean ComboBox fields.
		*/
		Ogre::String getBoolFieldSkin();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the skin name of all ComboBox fields.
		*/
		Ogre::String getComboBoxFieldSkin();
		/**
		* Returns true if double clicking on a node will toggle its state, false otherwise.
		*/
		bool getDoubleClickToToggle();
		/**
		* Returns the name of the skinType used to draw nodes in expanded form.
		*/
		Ogre::String getExpandSkinTypeName();
		/**
		* Returns true if up/down keys will select previous/next node and left/right expands/minimizes node, false otherwise.
		*/
		bool getKeyNavigation();
		/**
		* Returns the name of the skinType used to draw nodes in minimized form.
		*/
		Ogre::String getMinimizeSkinTypeName();
		/**
		* Gets the height of every created or existing Item in this PropertyGrid.
		*/
		float getItemHeight();
		/**
		* Returns the number of immediate sections in this PropertyGrid.
		*/
		int getNumberOfSections();
		/**
		* Gets the width of all property fields.
		*/
		float getPropertyFieldWidth();
		/**
		* Gets the width of all property labels.
		*/
		float getPropertyWidth();
		/**
		* Returns a list of root sections in this grid.
		*/
		std::list<PropertyGridSection*> getRootSections();
		/**
		* Returns the currently selected Property, or NULL if no node is selected.
		*/
		PropertyGridItem* getSelectedProperty();
		/**
		* Returns the skin name of all TextBox fields.
		*/
		Ogre::String getTextFieldSkin();

		/**
		* Removes all Event Handlers registered by the given object.
		*/
		virtual void removeEventHandlers(void* obj);

		/**
		* Sets whether double clicking on a node will toggle is state.
		*/
		void setDoubleClickToToggle(bool enable);
		/**
		* If set, up/down keys will select previous/next node and left/right expands/minimizes node.
		*/
		void setKeyNavigation(bool enable);
		/**
		* Selects a Property in the grid.
		* NOTE: Sections are Properties
		*/
		void selectItem(PropertyGridItem* pgp);
		/**
		* Sets the height of every created or existing Item in this PropertyGrid.
		*/
		void setItemHeight(float heightInPixels);
		/**
		* Sets the width of all property fields.
		*/
		void setPropertyFieldWidth(float width);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);

		/**
		* Recursively updates property positions and indices.
		*/
		void updateItems();
		/**
		* Recalculate Virtual dimensions, the minimum size required to encapsulate the client area and all Child widgets.
		*/
		virtual void updateVirtualDimensions();

	protected:
		PropertyGrid(const Ogre::String& name);
		virtual ~PropertyGrid();

		PropertyGridDesc* mDesc;

		float mTotalHeight;

		std::list<PropertyGridSection*> mSections;

		PropertyGridItem* mSelectedProperty;

		// TreeViewRadioButtonNode specific event handlers
		std::vector<EventHandlerSlot*> mPropertyGridEventHandlers[PROPERTYGRID_EVENT_COUNT];

		/**
		* Recursively calls children and returns the largest relative x position of any child nodes.
		*/
		float _getFarthestX();

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Adds a Section to the PropertyGrid at the index given.
		* NOTE: if the index is not valid, the Section is added to the end of the list.
		*/
		PropertyGridSection* createSection(PropertyGridSectionDesc* d, int index = -1);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* Clears selection.
		*/
		void onMouseDown(const EventArgs& args);

	private:
	};
}

#endif
