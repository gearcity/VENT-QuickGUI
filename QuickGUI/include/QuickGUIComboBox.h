#ifndef QUICKGUICOMBOBOX_H
#define QUICKGUICOMBOBOX_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUITextUser.h"

namespace QuickGUI
{
	// forward declarations
	class MenuPanel;
	class Button;
	class ListItem;
	class ListImageItem;
	class ListTextItem;

	class ListItemDesc;
	class ListImageItemDesc;
	class ListTextItemDesc;

	class _QuickGUIExport ComboBoxDesc :
		public ContainerWidgetDesc,
		public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		ComboBoxDesc();
		virtual ~ComboBoxDesc() {}
	public:

		bool					combobox_dropDownButton;
		float					combobox_itemHeight;
		float					combobox_dropDownWidth;
		// A value of 0 means there is no Max Height.
		float					combobox_dropDownMaxHeight;
		Ogre::String			combobox_userHandlers[COMBOBOX_EVENT_COUNT];
		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment	combobox_verticalTextAlignment;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ComboBoxDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ComboBox"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ComboBox :
		public ContainerWidget,
		public TextUser
	{
	public:
		friend class ListItem;

		friend class WidgetFactory;
	public:
		// Skin Constants
		static const Ogre::String COMBOBOXITEM;
		static const Ogre::String DEFAULT;
		static const Ogre::String DOWN;
		static const Ogre::String OVER;
		static const Ogre::String DROPDOWNBUTTON;
		static const Ogre::String DROPDOWNMENUPANEL;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);
		/**
		* Notifies the Widget of its GUIManager. (for component/container widgets this function is recursive)
		*/
		virtual void _setGUIManager(GUIManager* gm);
		/**
		* Notifies the Widget of its Sheet. (for component/container widgets this function is recursvie)
		*/
		virtual void _setSheet(Sheet* sheet);

		/**
		* Adds a user-defined event handler to this widget.  The difference between this function and the addWidgetEventHandler
		* is that the handlerName gets serialized with the Widget. This means you can defined handlers outside the creation of the
		* GUI Layout, and when the sheet is loaded from disk, all of the handlers will work properly.
		* NOTE: Only 1 user defined Event Handler can be assigned to an Event. Old values will be overwritten.
		* NOTE: A handler of "" essentially removes the handler.
		* NOTE: If no eventHandler exists with the name provided, nothing will happen.
		*/
		void addUserDefinedComboBoxEventHandler(ComboBoxEvent EVENT, const Ogre::String& handlerName);
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
		template<typename T> void addComboBoxEventHandler(ComboBoxEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addComboBoxEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addComboBoxEventHandler(ComboBoxEvent EVENT, EventHandlerSlot* function);

		/**
		* Removes all ListItems from the list.
		*/
		void clearItems();
		/**
		* Clears ComboBox Selection.
		*/
		void clearSelection();
		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedComboBoxEventHandler(ComboBoxEvent EVENT);
		/**
		* Adds a ListItem to the end of the DropDownList.
		*/
		ListItem* createItem(ListItemDesc* d, int index = -1);
		/**
		* Adds a ListImageItem to the end of the DropDownList.
		*/
		ListImageItem* createImageItem(const Ogre::String& imageName, int index = -1);
		ListImageItem* createImageItem(int index = -1);
		/**
		* Adds a ListTextItem to the end of the DropDownList
		*/
		ListTextItem* createTextItem(const Ogre::UTFString& text, int index = -1);
		ListTextItem* createTextItem(std::vector<TextSegment> segments, int index = -1);
		ListTextItem* createTextItem(int index = -1);


		/**
		* Removes an Item from the List.
		*/
		bool destroyItem(int index);
		/**
		* Removes an Item from the List.
		*/
		void destroyItem(ListItem* i);

		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		Widget* findWidget(const Ogre::String& widget_name);
		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireComboBoxEvent(ComboBoxEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the width of the Drop down MenuPanel.
		*/
		float getDropDownWidth();
		/**
		* Returns the Item at the index given.  If the index is invalid,
		* NULL is returned.
		*/
		ListItem* getItem(int index);

		/**
		*Get's a Item List
		**/
		std::list<ListItem*> getItemList();

		/**
		* Gets the height of each ListItem within this List.
		*/
		float getItemHeight();
		/**
		* Gets the maximum height of the Drop Down List.
		*/
		float getMaxDropDownHeight();
		/**
		* Gets the number of ListItems within this List.
		*/
		int getNumberOfItems();
		/**
		* Returns a pointer to a list of selected items.
		*/
		ListItem* getSelectedItem();
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();

		/**
		* Hides Drop Down List.
		*/
		void hideDropDownList();

		/**
		* Selects the item at index given.  If index is not valid,
		* all items will be de-selected.
		*/
		void selectItem(int index);
		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Sets the width of the Drop down MenuPanel.
		*/
		void setDropDownWidth(float width);
		/**
		* Sets the height of each ListItem within the List.
		*/
		void setItemHeight(float height);
		/**
		* Sets the maximum height of the Drop Down List.
		*/
		void setMaxDropDownHeight(float height);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Shows the Drop Down List.
		*/
		void showDropDownList();

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client widget_dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

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
		ComboBox(const Ogre::String& name);
		virtual ~ComboBox();

		ComboBoxDesc* mDesc;

		Button* mDropDownButton;

		// Window containing child ListItems
		MenuPanel* mMenuPanel;

		int mAutoNameCounter;

		// This list is assumed to be in order at all times.
		std::list<ListItem*> mItems;
		ListItem* mSelectedItem;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mComboBoxEventHandlers[COMBOBOX_EVENT_COUNT];

		// Current settings applied to newly added text.
		Ogre::String mCurrentFontName;
		ColourValue mCurrentColourValue;

		Ogre::String mImageName;
		bool mTileImage;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* Event handler for Left Mouse Button Down event. (Show/Hide Drop Down List)
		*/
		virtual void onMouseButtonDown(const EventArgs& args);
		/**
		* Event handler for Left Mouse Button Down on Drop Down Button. (Show/Hide Drop Down List)
		*/
		void onDropDownButtonDown(const EventArgs& args);
		void onMouseEnter(const EventArgs& args);
		void onMouseLeave(const EventArgs& args);

		/**
		* Function allowing ListItems to notify List when user left clicks on them.
		*/
		void selectItem(const MouseEventArgs& mea);

		/**
		* Clears selection list by de-selecting all selected units and clearing list.
		*/
		void _clearSelection();

		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();
	};
}

#endif
