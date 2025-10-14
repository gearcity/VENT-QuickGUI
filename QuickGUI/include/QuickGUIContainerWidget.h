#ifndef QUICKGUICONTAINERWIDGET_H
#define QUICKGUICONTAINERWIDGET_H

#include "QuickGUIComponentWidget.h"

namespace QuickGUI
{
	// forward declaration
	class GUIManager;
	class SerialReader;
	class HScrollBar;
	class VScrollBar;

	class _QuickGUIExport ContainerWidgetDesc :
			public ComponentWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		ContainerWidgetDesc();
		virtual ~ContainerWidgetDesc() {}
	public:

		/// Sets whether clipping region will be set to client area prior to children drawing
		bool	containerwidget_clipChildrenToDimensions;
		/// Amount of Scroll occuring when the horizontal bar is pressed.
		float	containerwidget_horzBarScrollPercent;
		/// Amount of Scroll occuring when the left/right buttons are pressed.
		float	containerwidget_horzButtonScrollPercent;
		/// If true, scrollbars will be created.  Some ContainerWidgets,
		/// especially menu and toolbar widgets, don't need scrollbars.
		bool	containerwidget_supportScrollBars;
		/// How thick the vertical and horizontal scrollbars are
		float	containerwidget_scrollBarThickness;
		/// Amount of Scroll occuring when the vertical bar is pressed.
		float	containerwidget_vertBarScrollPercent;
		/// Amount of Scroll occuring when the up/down buttons are pressed.
		float	containerwidget_vertButtonScrollPercent;
		/// The horizontal scroll applied to all children
		float	containerwidget_xScrollOffset;
		/// The vertical scroll applied to all children
		float	containerwidget_yScrollOffset;

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ContainerWidget :
		public ComponentWidget
	{
	public:
		// Skin Constants
		static const Ogre::String HSCROLLBAR;
		static const Ogre::String VSCROLLBAR;
	public:
		friend class GUIManager;
		friend class SerialReader;
		friend class Menu;
		// Widget::destroy needs to be able to call protected destroyChild API
		friend class Widget;
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
		* Recursively searches through children and returns the first widget found of the class given.
		*/
		virtual Widget* findFirstWidgetOfClass(const Ogre::String& className);
		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& name);
		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);

		/**
		* Returns a list of all child widgets.
		*/
		std::vector<Widget*> getChildren();
		/**
		* Returns true if clipping region will be set to client area prior to children drawing, false otherwise.
		*/
		bool getClipChildrenToDimensions();
		/**
		* Returns true if children are drawn even when this widget is not visible, false otherwise.
		*/
		bool getDrawChildrenWhenNotVisible();

		/**
		* Returns true if this widget is able to have child widgets.
		*/
		virtual bool isContainerWidget();

		/**
		* Adjusts ScrollBars so that Widget is in view.
		*/
		void scrollChildIntoView(Widget* child);
		/**
		* Adjusts ScrollBars so that Point is in view.
		* NOTE: It is assumed the point given is relative to this widget.
		*/
		void scrollPointIntoView(const Point& p);
		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Sets the amount of Scroll occuring when the horizontal bar is pressed.
		*/
		void setHorzBarScrollPercent(float percent);
		/**
		* Sets the amount of Scroll occuring when the horizontal buttons are pressed.
		*/
		void setHorzButtonScrollPercent(float percent);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets the amount of Scroll occuring when the vertical bar is pressed.
		*/
		void setVertBarScrollPercent(float percent);
		/**
		* Sets the amount of Scroll occuring when the vertical buttons are pressed.
		*/
		void setVertButtonScrollPercent(float percent);
		/**
		* Returns true if this Widget supports scrolling, false otherwise.
		*/
		bool supportsScrolling();

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();
		/**
		* Recalculate Virtual dimensions, the minimum size required to encapsulate the client area and all Child widgets.
		*/
		virtual void updateVirtualDimensions();

		HScrollBar* mHScrollBar;
		VScrollBar* mVScrollBar;

		bool manualHScrollBarVisibility; 
		

	protected:
		// The following functions are protected because all inheritted classes should not allow
		// public access to them.  For example, a MenuItem is a Widget, but should not allow users
		// to manipulate its size or position.

		/**
		* Returns true if clipping region will be set to client area prior to children drawing, false otherwise.
		*/
		void setClipChildrenToDimensions(bool clip);
		/**
		* Normally Children are not drawn when this widget is not visible, but if set true,
		* children are still drawn.
		*/
		void setDrawChildrenWhenNotVisible(bool draw);

	protected:
		ContainerWidget(const Ogre::String& name);
		virtual ~ContainerWidget();

		std::vector<Widget*> mChildren;

		ContainerWidgetDesc* mDesc;

		
		/// Hides the ScrollBars if the VirtualArea is the same size as the ClientArea.
		void _determineScrollBarVisibility();
		
		bool mUpdatingClientDimensions;
		// Used to implement scrolling functionality
		Size mVirtualSize;
		void _setScrollX(float percentage);
		virtual void _setScrollY(float percentage);

		void onHorizontalScroll(const EventArgs& args);
		void onVerticalScroll(const EventArgs& args);

		void onChildVisibilityChanged(const EventArgs& args);

		virtual void onChildDimensionsChanged(const EventArgs& args);

		void onMouseWheel(const EventArgs& args);

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Destroys a child belonging to this widget.
		*/
		virtual void destroyChild(Widget* w);
		/**
		* Destroys all children belonging to this widget.
		*/
		void destroyChildren();

		/**
		* Removes a child widget from this container widget.
		*/
		virtual void removeChild(Widget* w);
		/**
		* Removes all children from this container widget.
		*/
		void removeChildren();

		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();
	private:
	};
}

#endif
