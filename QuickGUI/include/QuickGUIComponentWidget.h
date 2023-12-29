#ifndef QUICKGUICOMPONENTWIDGET_H
#define QUICKGUICOMPONENTWIDGET_H

#include "QuickGUIWidget.h"

#include <map>
#include <vector>

namespace QuickGUI
{
	// forward declaration
	class GUIManager;
	class SerialReader;

	class _QuickGUIExport ComponentWidgetDesc :
			public WidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		ComponentWidgetDesc();
		virtual ~ComponentWidgetDesc() {}
	public:

		/// Sets whether clipping region will be set to client area prior to component drawing
		bool componentwidget_clipComponentsToDimensions;

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ComponentWidget :
		public Widget
	{
	public:
		friend class GUIManager;
		friend class SerialReader;
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
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);

		/**
		* Returns true if clipping region will be set to client area prior to component drawing, false otherwise.
		*/
		bool getClipComponentsToDimensions();

		/**
		* Returns true if this widget is made up of more than 1 widget.
		*/
		virtual bool isComponentWidget();

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
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		// The following functions are protected because all inheritted classes should not allow
		// public access to them.  For example, a MenuItem is a Widget, but should not allow users
		// to manipulate its size or position.

		/**
		* Returns true if clipping region will be set to client area prior to component drawing, false otherwise.
		*/
		void setClipComponentsToDimensions(bool clip);
		/**
		* Sets whether this widget drags its Base Widget when dragged.
		* NOTE: This is only applicable to widgets that are components of another widget.
		*/
		virtual void setMoveBaseWidgetOnDrag(bool moveBaseWidget);

	protected:
		ComponentWidget(const Ogre::String& name);
		virtual ~ComponentWidget();

#if USEHASHMAPS
		stdext::hash_map<Ogre::String,Widget*> mComponents;
#else
		std::map<Ogre::String,Widget*> mComponents;
#endif
		/**
		* Adds a component to this widget.
		*/
		void addComponent(const Ogre::String& alias, Widget* w);

		/**
		* Internal function used to apply anchor to a widget, after its parent has changed in size.
		*/
		void applyAnchor(Widget* child, Size changeInSize);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);
	private:
	};
}

#endif
