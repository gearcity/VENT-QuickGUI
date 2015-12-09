#ifndef QUICKGUILIST_H
#define QUICKGUILIST_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	// forward declarations
	class ListItem;
	class ListImageItem;
	class ListPanelItem;
	class ListTextItem;

	class ListItemDesc;
	class ListImageItemDesc;
	class ListPanelItemDesc;
	class ListTextItemDesc;

	class _QuickGUIExport ListDesc :
		public ContainerWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		ListDesc();
		virtual ~ListDesc() {}
	public:

		float			list_itemHeight;
		bool			list_supportMultiSelect;
		Ogre::String	list_userHandlers[LIST_EVENT_COUNT];

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "List"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport List :
		public ContainerWidget
	{
	public:
		friend class ListItem;
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String LISTITEM;
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
		void addUserDefinedListEventHandler(ListEvent EVENT, const Ogre::String& handlerName);
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
		template<typename T> void addListEventHandler(ListEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addListEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addListEventHandler(ListEvent EVENT, EventHandlerSlot* function);

		/**
		* Removes all ListItems from the list.
		*/
		void clearItems();
		/**
		* De-selects any selected ListItems in the list.
		*/
		void clearSelection();
		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedListEventHandler(ListEvent EVENT);
		/**
		* Adds a ListItem to the end of the List.
		*/
		ListItem* createItem(ListItemDesc* d, int index = -1);
		/**
		* Adds a ListImageItem to the end of the List.
		*/
		ListImageItem* createImageItem(const Ogre::String& imageName, int index = -1);
		ListImageItem* createImageItem(int index = -1);
		/**
		* Adds a ListPanelItem to the end of the List.
		*/
		ListPanelItem* createPanelItem(int index = -1);
		/**
		* Adds a ListTextItem to the end of the List.
		*/
		ListTextItem* createTextItem(const Ogre::UTFString& text, int index = -1);
		ListTextItem* createTextItem(std::vector<TextSegment> segments, int index = -1);
		ListTextItem* createTextItem(int index = -1);

		/**
		* De-selects the item at the index given.  If index is invalid, nothing happens.
		*/
		void deselectItem(unsigned int index);
		/**
		* Removes an Item from the List and destroys it.
		*/
		void destroyItem(unsigned int index);
		void destroyItem(ListItem* i);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireListEvent(ListEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the ListItem at the index given.  If the index is invalid,
		* NULL is returned.
		*/
		ListItem* getItem(unsigned int index);
		/**
		* Gets the height of each ListItem within this List.
		*/
		float getItemHeight();
		/**
		* Returns true if multiple items can be selected simultaneously, false otherwise.
		*/
		bool getMultiSelect();
		/**
		* Gets the number of ListItems within this List.
		*/
		int getNumberOfItems();
		/**
		* Returns a pointer to a list of selected items.
		*/
		std::list<ListItem*> getSelection();

		/**
		* Selects the item at index given.  If index is not valid,
		* all items will be de-selected.
		*/
		void selectItem(unsigned int index);
		/**
		* Sets the height of each ListItem within the List.
		*/
		void setItemHeight(float height);
		/**
		* If set true, multiple items can be selected simultaneously, otherwise only
		* single selections can be made.
		*/
		void setMultiSelect(bool MultiSelect);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);

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
		using Widget::setHeight;
		using Widget::setHorizontalAnchor;
		using Widget::setMaxSize;
		using Widget::setMinSize;
		using Widget::setPosition;
		using Widget::setPositionRelativeToParentClientDimensions;
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
		List(const Ogre::String& name);
		virtual ~List();

		ListDesc* mDesc;

		int mAutoNameCounter;

		std::list<ListItem*> mItems;
		std::list<ListItem*> mSelectedItems;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mListEventHandlers[LIST_EVENT_COUNT];

		unsigned int mPrevSelectedIndex;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Function allowing ListItems to notify List when user left clicks on them.
		*/
		void selectItem(const MouseEventArgs& mea);

		/**
		* Clears selection list by de-selecting all selected units and clearing list.
		*/
		void _clearSelection();

		/**
		* Returns the count of the list item within the list.
		*/
		unsigned int _getItemIndex(ListItem* li);

	private:
	};
}

#endif
