#ifndef QUICKGUIWIDGET_H
#define QUICKGUIWIDGET_H

#include "QuickGUIAnchors.h"
#include "QuickGUIBorderEnums.h"
#include "QuickGUIBrush.h"
#include "QuickGUIDesc.h"
#include "QuickGUIEventArgs.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIEventHandlerPointer.h"
#include "QuickGUIRect.h"
#include "QuickGUISerializable.h"
#include "QuickGUISkinTypeManager.h"
#include "QuickGUIStringConverter.h"
#include "QuickGUIEventTypes.h"

#include "OgrePrerequisites.h"

namespace QuickGUIEditor
{
	// forward declarations
	class MainForm;
}

namespace QuickGUI
{
	// forward declarations
	class GUIManager;
	class Window;
	class Sheet;

	// Global function to create Widgets

	class _QuickGUIExport WidgetDesc :
		public Desc
	{
	public:
		friend class DescFactory;
	protected:
		WidgetDesc();
		virtual ~WidgetDesc() {}
	public:

		ColourValue	widget_baseColor;
		BrushFilterMode		widget_brushFilterMode;
		bool				widget_consumeKeyboardEvents;
		Ogre::String		widget_contextMenuName;
		bool				widget_enabled;
		Rect				widget_dimensions;
		Ogre::String		widget_disabledSkinType;
		bool				widget_dragable;
		HorizontalAnchor	widget_horizontalAnchor;
		float				widget_hoverTime;
		bool				widget_inheritOpacity;
		Size				widget_maxSize;
		Size				widget_minSize;
		bool				widget_moveBaseWidgetOnDrag;
		Ogre::String		widget_name;
		float				widget_relativeOpacity;
		bool				widget_positionRelativeToParentClientDimensions;
		bool				widget_resizeFromBottom;
		bool				widget_resizeFromLeft;
		bool				widget_resizeFromRight;
		bool				widget_resizeFromTop;
		bool				widget_scrollable;
		bool				widget_serialize;
		Ogre::String		widget_skinTypeName;
		bool				widget_transparencyPicking;
		Ogre::String		widget_userHandlers[WIDGET_EVENT_COUNT];
		VerticalAnchor		widget_verticalAnchor;
		bool				widget_visible;

		GUIManager* guiManager;
		Sheet* sheet;
		
		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "WidgetDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Widget"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();
		
		/**
		* Outlines how the desc class is written to script and read from script.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Widget :
		public Serializable
	{
	public:
		friend class GUIManager;
		// calls protected draw function
		friend class ComponentWidget;
		// calls protected draw function
		friend class ContainerWidget;
		friend class Sheet;
		// calls protected draw function
		friend class TabPage;
		friend class Menu;
		// calls protected setWidth function, when ContextMenu's width change
		friend class ContextMenu;
		// ListPanel overrides draw order and needs access to draw method
		friend class ListPanel;
		// MenuPanel overrides draw order and needs access to draw method
		friend class MenuPanel;
		// calls protected setWidth/setPosition/setMaxSize function
		friend class PropertyGridSection;
		friend class PropertyGridItem;
		// TextArea overrides setParent function and needs to call setParent on child/component widgets
		friend class TextArea;
		// ToolBar sets child widgets vertical/horizontal anchors
		friend class ToolBar;
		// Makes sure nodes are set visible on expanded node path
		friend class TreeViewNode;
		friend class Window;
		// Factory class creates and destroys Widgets
		friend class WidgetFactory;
		friend class QuickGUIEditor::MainForm;
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
		void addUserDefinedWidgetEventHandler(WidgetEvent EVENT, const Ogre::String& handlerName);
		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WIDGET_EVENT_SIZE_CHANGED, WIDGET_EVENT_MOUSE_WHEEL, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return void, and take QuickGUI::EventArgs as its parameters.
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
		template<typename T> void addWidgetEventHandler(WidgetEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addWidgetEventHandler(EVENT,OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		virtual void addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function);

		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedWidgetEventHandler(WidgetEvent EVENT);

		/**
		* Queues this widget for destruction, likely to happen in the next frame, before drawing occurs.
		* NOTE: Widgets that are components of another widget cannot be destroyed using this API.
		*/
		virtual void destroy();

		/**
		* Recursively searches through children and returns the first widget found of the class given.
		*/
		virtual Widget* findFirstWidgetOfClass(const Ogre::String& className);
		/**
		* Recursively searches through children and returns first widget found with widget_name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& widget_name);
		/**
		* Checks if point p is within this widget's widget_dimensions.
		* NULL is returned if the point is outside widget_dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);
		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireWidgetEvent(WidgetEvent EVENT, EventArgs& args);

		/**
		* Returns the opacity of this widget as it is drawn on screen.
		* NOTE: Includes parent opacity if widget_inheritOpacity is true.
		*/
		virtual float getAbsoluteOpacity();
		/**
		* This widget might be a part of a ComponentWidget.  This function will return the ComponentWidget
		* that owns this widget, or if the widget is not a component of a widget, returns itself.
		*/
		Widget* getBaseWidget();
		/**
		* Returns the color used to draw SkinElements of this widget.
		* NOTE: The alpha channel is not used, it is affected only by the opacity attribute.
		*/
		ColourValue getBaseColor();
		/**
		* Returns true if this widget is being dragged, false otherwise.
		*/
		bool getBeingDragged();
		/**
		* Returns the Border underneath the point p.
		* NOTE: Point is assumed to be relative to the Window.
		*/
		virtual BorderSide getBorderSide(Point p);
		/**
		* Returns the filtering used when drawing the skin of this widget.
		*/
		BrushFilterMode getBrushFilterMode();
		/**
		* Returns the class widget_name of the widget, ie "Button", "Window", etc.
		*/
		virtual Ogre::String getClass() = 0;
		/**
		* Returns the rectangle inside the borders of the widget, if borders exist.
		*/
		Rect getClientDimensions();
		/**
		* Returns true if this widget accepts notification of keyboard events,
		* false otherwise.
		*/
		bool getConsumeKeyboardEvents();
		/**
		* Returns the name of the context menu for this widget.  If this widget does not have
		* a context menu, empty string is returned. ("")
		*/
		Ogre::String getContextMenuName();
		/*
		* Gets the position and size of this widget.
		* NOTE: Position is relative to its parent.
		*/
		Rect getDimensions();
		/**
		* Returns the widget_name of the SkinType applied when the widget is disabled.
		* NOTE: "" is returned if the widget's appearance does not change when disabled.
		*/
		Ogre::String getDisabledSkinType();
		/**
		* Returns true if this widget can be dragged, false otherwise.
		*/
		bool getDragable();
		/**
		* Returns true if this widget is able to receive injected input events,
		* false otherwise.
		* NOTE: Disabled widgets can still have events fired manually.
		*/
		bool getEnabled();
		/**
		* Returns true if this widget is grabbed, false otherwise.
		*/
		bool getGrabbed();
		/**
		* Returns a reference to the GUIManager this widget belongs to.
		*/
		GUIManager* getGUIManager();
		/**
		* Returns the height in pixels of this widget.
		*/
		float getHeight();
		/**
		* Returns the horizontal anchor set for this widget.
		*/
		virtual HorizontalAnchor getHorizontalAnchor();
		/**
		* Get the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired.
		*/
		float getHoverTime();
		/**
		* Returns true if this widgets opacity is affected by its parent's opacity, false otherwise.
		*/
		bool getInheritOpacity();
		/**
		* Returns the maximum size this widget can be.
		*/
		Size getMaxSize();
		/**
		* Returns the minimum size this widget can be.
		*/
		Size getMinSize();
		/**
		* Returns true if this widget drags its Base Widget when dragged.
		* NOTE: This is only applicable to widgets that are components of another widget.
		*/
		bool getMoveBaseWidgetOnDrag();
		/**
		* Returns the widget_name of the Widget.
		*/
		Ogre::String getName();
		/**
		* Returns the parent of this widget, or NULL if this widget has no parent.
		*/
		Widget* getParentWidget();
		/**
		* Get the position of this widget relative to its parent.
		* NOTE: This does not take scrolling into account.
		*/
		Point getPosition();
		/**
		* Returns true if the widget's origin is at the Parent's client area origin, false if
		* the widget's origin is at the Parent's origin. (The client area is typically the area inside a Widget's
		* borders)
		*/
		bool getPositionRelativeToParentClientDimensions();
		/**
		* Returns the relative opacity of the widget.
		*/
		float getRelativeOpacity();
		/**
		* Returns true if the bottom border of this widget can be used with the mouse cursor to resize the widget,
		* false otherwise.
		*/
		bool getResizeFromBottom();
		/**
		* Returns true if the left border of this widget can be used with the mouse cursor to resize the widget,
		* false otherwise.
		*/
		bool getResizeFromLeft();
		/**
		* Returns true if the right border of this widget can be used with the mouse cursor to resize the widget,
		* false otherwise.
		*/
		bool getResizeFromRight();
		/**
		* Returns true if the top border of this widget can be used with the mouse cursor to resize the widget,
		* false otherwise.
		*/
		bool getResizeFromTop();
		/**
		* Gets the position of the widget as seen on the screen.
		*/
		Point getScreenPosition();
		/**
		* Gets the amount of pixels this widget is scrolled horizontally and vertically.
		*/
		Point getScroll();
		/**
		* Returns true if this widget can be scrolled by its parent, false otherwise.
		*/
		bool getScrollable();
		/**
		* Gets the Scrollable ContainerWidget this widget belongs to, or NULL if this widget
		* is not or does not belong to a Scrollable container.
		*/
		Widget* getScrollableContainerWidget();
		/**
		* Returns true if this widget serializes itself to disk when its Sheet gets serialized to disk,
		* false if this widget does not serialize to disk.
		*/
		bool getSerialize();
		/**
		* Returns the sheet this widget belongs to.
		*/
		Sheet* getSheet();
		/**
		* Returns the size of the widget;
		*/
		Size getSize();
		/**
		* Gets the position of the Widget as seen on the texture its drawn on.
		*/
		Point getTexturePosition();
		/**
		* Returns true if this widget uses transparency picking, false otherwise.  For
		* Widgets that do not use transparency picking, they will receive mouse over events
		* simply by having the mouse over their texture widget_dimensions.
		*/
		bool getTransparencyPicking();
		/**
		* Returns the vertical anchor set for this widget.
		*/
		VerticalAnchor getVerticalAnchor();
		/**
		* Returns true if the widget is configured as widget_visible, false otherwise.
		* NOTE: Due to scrolling, a widget may be marked as widget_visible, but not seen on the screen.
		*/
		bool getVisible();
		/**
		* Retrieves the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		Ogre::String getSkinTypeName();
		/**
		* Returns the width in pixels of this widget.
		*/
		float getWidth();
		/**
		* Returns a pointer to the Window this widget belongs to.
		* NOTE: A Sheet is a Window.
		*/
		Window* getWindow();

		/**
		* Returns true if there exists a Widget Event handler was registered by the object given,
		* false otherwise.
		*/
		bool hasEventHandler(WidgetEvent EVENT, void* obj);

		/**
		* Returns true if there exists a Widget Event handler for any object,
		* false otherwise.
		*/
		bool hasAnyEventHandlersForThisType(WidgetEvent EVENT);
	
		/**
		* Returns true if this widget is a child of the widget given.
		*/
		virtual bool isChildOf(Widget* w);
		/**
		* Returns true if this widget is made up of more than 1 widget.
		*/
		virtual bool isComponentWidget();
		/**
		* Returns true if this widget is a component of another widget. (ie button on a ScrollBar)
		*/
		bool isComponentOfAWidget();
		/**
		* Returns true if this widget is a component of the widget given.
		*/
		bool isComponentOf(Widget* w);
		/**
		* Returns true if this widget is able to have child widgets.
		*/
		virtual bool isContainerWidget();
		/**
		* Returns true if this widget is a menu item, false otherwise.
		*/
		virtual bool isMenuItem();
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
		* Returns true if the point is over a border, false otherwise.
		* NOTE: Point is assumed to be relative to the Window.
		*/
		virtual bool overBorderSide(Point p);

		/**
		* Flags the parent window as dirty causing its texture to be updated (redrawn).
		*/
		virtual void redraw();
		/**
		* Removes the Event Handler registered by the obj.
		*/
		void removeEventHandler(WidgetEvent EVENT, void* obj);
		
		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Sets the color used to draw SkinElements of this widget.
		* NOTE: The alpha channel is not used, it is affected only by the opacity attribute.
		*/
		void setBaseColor(ColourValue cv);
		/**
		* Sets the filtering used when drawing the skin of this widget.
		*/
		void setBrushFilterMode(BrushFilterMode m);
		/**
		* Sets the context menu displayed when the right mouse button goes up over this widget.
		* NOTE: Empty string is valid, and removes any context menu associated with this widget.
		*/
		void setContextMenuName(const Ogre::String& contextMenuName);
		/**
		* Sets the widget_name of the SkinType applied when the widget is disabled.
		* NOTE: Use "" to prevent the widget's apperance from changing when disabled.
		*/
		void setDisabledSkinType(const Ogre::String& SkinTypeName);
		/**
		* Enabled Widgets receive mouse and keyboard events via injections to the GUIManager.
		* Disabled Widgets can only receive these events if they are manually fired.
		*/
		virtual void setEnabled(bool enabled);
		/**
		* Notify the widget it is grabbed or not grabbed.
		*/
		void setGrabbed(bool grabbed);
		/**
		* Set the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired. (Commonly used for tool tips)
		*/
		void setHoverTime(float seconds);
		/**
		* Sets whether this widget will inherit the opacity of its parent.
		* NOTE: Widgets will always inherit the opacity of their owner Window or Sheet,
		*  whichever comes first.
		*/
		void setInheritOpacity(bool inherit);
		/**
		* Sets the name of the Widget.  If the name is already taken, it will be resolved,
		* with a different name.  The resolved name is returned by the function.
		*/
		virtual Ogre::String setName(const Ogre::String& name);
		/**
		* Sets the opacity of this widget.  Values will be curbed within the range [0,1]
		* NOTE: Widgets will always inherit the opacity of their owner Window or Sheet,
		*  whichever comes first.
		*/
		void setRelativeOpacity(float opacity);
		/**
		* Scroll the widget horizontally and vertically.
		*/
		void setScroll(unsigned int scrollX, unsigned int scrollY);
		/**
		* Scroll the widget horizontally.
		*/
		void setScrollX(unsigned int scrollX);
		/**
		* Scroll the widget vertically.
		*/
		void setScrollY(unsigned int scrollY);
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
		/**
		* Recalculate Screen and client widget_dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		// The following functions are protected because all inheritted classes should not allow
		// public access to them.  For example, a MenuItem is a Widget, but should not allow users
		// to manipulate its size or position.

		/**
		* Offsets the widget.
		*/
		virtual void drag(int xOffset, int yOffset);

		/**
		* Resizes the widget so that its widget_dimensions hit the point given.
		* NOTE: The point p is assumed to be global coords.
		*/
		virtual void resize(BorderSide s, Point position);

		/**
		* Set true if this widget accepts notification of keyboard events,
		* false otherwise.
		*/
		void setConsumeKeyboardEvents(bool consume);
		/**
		* Sets the size and position (position relative to parent) of this Widget, respectively.
		*/
		virtual void setDimensions(const Rect& r);
		/**
		* Set whether or not this widget can be dragged by the mouse cursor.
		*/
		virtual void setDragable(bool dragable);
		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);
		/**
		* Sets Horizontal Anchor of this widget. A Left anchor will enforce the widget to maintain
		* its distance from the left side of its parent. A right anchor will enforce the widget to maintain
		* its distance from the right side of its parent. Left and Right Anchor will grow and shrink the
		* widget to maintain distance from both sides.
		*/
		virtual void setHorizontalAnchor(HorizontalAnchor a);
		/**
		* Sets the maximum size this widget can be.
		*/
		void setMaxSize(const Size& s);
		/**
		* Sets the minimum size this widget can be.
		*/
		void setMinSize(const Size& s);
		/**
		* Sets whether this widget drags its Base Widget when dragged.
		* NOTE: This is only applicable to widgets that are components of another widget.
		* NOTE: If this widget is a component of another widget, and moveBaseWidget is true,
		*  the widget will become dragable.
		*/
		virtual void setMoveBaseWidgetOnDrag(bool moveBaseWidget);
		/**
		* Sets the x and y position of this widget, relative to this widget's parent.
		*/
		virtual void setPosition(const Point& position);
		/**
		* If set true, Widget's origin is in Parent's client area origin. If set false
		* Widget's origin is at Parent's origin.
		*/
		void setPositionRelativeToParentClientDimensions(bool relativeToClientOrigin);
		/**
		* Convenience methods allowing all sides to enable/disable resizing using the mouse cursor.
		*/
		void setResizeFromAllSides(bool resizable);
		/**
		* If set true, the bottom border of this widget can be used with the mouse cursor to resize the widget.
		*/
		void setResizeFromBottom(bool resizable);
		/**
		* If set true, the left border of this widget can be used with the mouse cursor to resize the widget.
		*/
		void setResizeFromLeft(bool resizable);
		/**
		* If set true, the right border of this widget can be used with the mouse cursor to resize the widget.
		*/
		void setResizeFromRight(bool resizable);
		/**
		* If set true, the top border of this widget can be used with the mouse cursor to resize the widget.
		*/
		void setResizeFromTop(bool resizable);
		/**
		* Sets whether the widget can be scrolled by its parent widget.
		*/
		void setScrollable(bool scrollable);
		/**
		* If set true, this widget serializes itself to disk when its Sheet gets serialized to disk,
		* otherwise it will not get saved to disk.
		*/
		void setSerialize(bool serialize);
		/**
		* Sets the width and height of this widget.
		*/
		virtual void setSize(Size size);
		/**
		* Sets whether the widget will receive mouse over events simply by having the mouse over
		* its texture widget_dimensions, or only when the cursor is over non transparent parts.
		*/
		virtual void setTransparencyPicking(bool transparencyPicking);
		/**
		* Sets Vertical Anchor of this widget. A Top anchor will enforce the widget to maintain
		* its distance from the top side of its parent. A bottom anchor will enforce the widget to maintain
		* its distance from the bottom side of its parent. Top and Bottom Anchor will grow and shrink the
		* widget to maintain distance from both sides.
		*/
		virtual void setVerticalAnchor(VerticalAnchor a);
		/**
		* Sets whether or not the widget gets drawn.
		*/
		virtual void setVisible(bool visible);
		/**
		* Sets the width of the widget.
		*/
		virtual void setWidth(float pixelWidth);

	protected:
		Widget(const Ogre::String& widget_name);
		virtual ~Widget();

		// Used mainly for serialization
		Ogre::String mName;

		bool mNameRegisteredWithSheet;

		WidgetDesc* mWidgetDesc;
		/**
		* Internal use only. Creates desc corresponding to the widget class.
		*/
		void _createDescObject();
		/**
		* Internal use only.  Allows Widgets to create other widgets using factory.
		* NOTE: After widget creation, its _initialize function is called, using the Desc given.
		*/
		Widget* _createWidget(WidgetDesc* d);

		bool mGrabbed;
		bool mBeingDragged;

		Widget* mParentWidget;
		Window* mWindow;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mWidgetEventHandlers[WIDGET_EVENT_COUNT];
		Ogre::String mUserDefinedHandlers[WIDGET_EVENT_COUNT];

		// Area for child widgets, inside borders if they exist.
		Rect mClientDimensions;
		// Absolute position of the widget relative to it's Window's texture
		// For Windows (Sheet), this is always 0,0
		Point mTexturePosition;

		// Internal function that sets the SkinType without modifying the desc's property (allows enable/disable skin)
		void _setSkinType(const Ogre::String& skinName);
		// The skin currently used to draw the widget
		SkinType* mSkinType;
		// The skin element to use for border detection and client area calculations.
		SkinElement* mSkinElement;
		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();

		// Prevents initializing the widget twice.
		bool mInitialized;

		// True if this widget is used as a component, false otherwise.
		// Components do not have to be drawn inside client widget_dimensions.
		bool mComponentOfAWidget;

		Point mScrollOffset;

		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw() = 0;

	private:

	};
}

#endif
