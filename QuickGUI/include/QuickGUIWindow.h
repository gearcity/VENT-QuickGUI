#ifndef QUICKGUIWINDOW_H
#define QUICKGUIWINDOW_H

#include "QuickGUIPanel.h"

#include "OgreHardwarePixelBuffer.h"
#include "OgreRenderTexture.h"
#include "OgreTextureManager.h"

namespace QuickGUI
{
	// forward declaration
	class GUIManager;
	class Sheet;
	class TitleBar;

	class _QuickGUIExport WindowDesc :
			public PanelDesc
	{
	public:
		friend class DescFactory;
	protected:
		WindowDesc();
		virtual ~WindowDesc() {}
	public:

		bool					window_titleBar;
		bool					window_titleBarDragable;
		Ogre::String			window_titleBarSkinType;
		bool					window_titleBarCloseButton;
		/// number of pixels between from the top, bottom, and right of TitleBar right end
		float					window_titleBarCloseButtonPadding;
		Ogre::String			window_titleBarCloseButtonSkinType;
		
		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment	window_titlebarVerticalTextAlignment;

		/// Describes the Text used in this Window's TitleBar
		TextDesc				textDesc;

		virtual Ogre::String getClass() { return "WindowDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Window"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Window :
		public Panel
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String TITLEBAR;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;
		friend class Sheet;
		friend class ComponentWidget;
		friend class ContainerWidget;
		friend class Menu;
		friend class Widget;

		friend class WidgetFactory;
	public:
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

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
		template<typename T> void addWindowEventHandler(WindowEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addWindowEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addWindowEventHandler(WindowEvent EVENT, EventHandlerSlot* function);

		/**
		* Moves the Window to the end of the list, meaning it will be drawn last, on top of other Windows.
		*/
		void bringToFront();

		/**
		* Positions the window so that it is displayed in the center of the Sheet it belongs to.
		*/
		void center();

		/**
		* Queues this widget for destruction, likely to happen in the next frame, before drawing occurs.
		* NOTE: Widgets that are components of another widget cannot be destroyed using this API.
		*/
		virtual void destroy();

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireWindowEvent(WindowEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns true if the TitleBar is dragable, false otherwise.
		*/
		bool getTitleBarDragable();
		/**
		* Gets the TitleBar's text in UTFString form.
		*/
		Ogre::UTFString getTitleBarText();
		/**
		* Returns the filtering used when drawing the text of this widget.
		*/
		BrushFilterMode getTitleBarTextBrushFilterMode();

		/**
		* Returns true if there exists a Widget Event handler was registered by the object given,
		* false otherwise.
		*/
		bool hasWindowEventHandler(WindowEvent EVENT, void* obj);

		/**
		* Flags the window as dirty causing its texture to be updated (redrawn).
		*/
		virtual void redraw();
		/**
		* Removes the Event Handler registered by the obj.
		*/
		void removeWindowEventHandler(WindowEvent EVENT, void* obj);
		/**
		* Resizes the widget so that its widget_dimensions hit the point given.
		* NOTE: The point p is assumed to be global coords.
		*/
		virtual void resize(BorderSide s, Point position);

		/**
		* Writes the windows generated texture to file.
		*/
		void saveTextureToFile(const Ogre::String& filename);
		/**
		* Moves the Window to the beginning of the list, meaning it will be drawn first, underneath any other windows.
		*/
		void sendToBack();
		/**
		* Sets the name of the Widget.  If the name is already taken, it will be resolved,
		* with a different name.  The resolved name is returned by the function.
		*/
		virtual Ogre::String setName(const Ogre::String& name);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets whether the titlebar can be dragged, in effect dragging the window.
		*/
		void setTitleBarDragable(bool dragable);
		/**
		* Sets the text for this object.
		*/
		void setTitleBarText(Ogre::UTFString s);
		/**
		* Sets the text for this object.
		*/
		void setTitleBarText(Ogre::UTFString s, Ogre::FontPtr fp, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setTitleBarText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv);
		/**
		* Sets the Text using Text Segments.
		*/
		void setTitleBarText(std::vector<TextSegment> segments);
		/**
		* Sets the max width of the Text inside the TitleBar.  If this value is non-zero,
		* text exceeding the allotted width will be wrapped and displayed underneath the previous line of text.
		*/
		void setTitleBarTextAllottedWidth(float allottedWidth);
		/**
		* Sets all characters of the text to the specified color.
		*/
		void setTitleBarTextColor(const ColourValue& cv);
		/**
		* Sets the character at the index given to the specified color.
		*/
		void setTitleBarTextColor(const ColourValue& cv, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified color.
		*/
		void setTitleBarTextColor(const ColourValue& cv, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setTitleBarTextColor(const ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setTitleBarTextColor(const ColourValue& cv, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets all characters of the text to the specified font.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the filtering used when drawing the text.
		*/
		void setTitleBarTextBrushFilterMode(BrushFilterMode m);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::drag;
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
		using Widget::setSerialize;
		using Widget::setSize;
		using Widget::setTransparencyPicking;
		using Widget::setVerticalAnchor;
		using Widget::setVisible;
		using Widget::setWidth;

	protected:
		Window(const Ogre::String& name);
		virtual ~Window();

		WindowDesc* mDesc;

		TitleBar* mTitleBar;

		// Toggled true if the window's texture needs to be updated (redrawn).
		bool mDirty;

		Ogre::TexturePtr mTexture;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mWindowEventHandlers[WINDOW_EVENT_COUNT];

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

		/**
		* Creates the Render Target (texture) used for this window and its child widgets.
		*/
		void createRenderTarget();

		/**
		* Resizes the window's texture to the correct size
		*/
		void resizeRenderTarget();

	private:
	};
}

#endif
