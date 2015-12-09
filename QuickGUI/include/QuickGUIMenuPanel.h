#ifndef QUICKGUIMENUPANEL_H
#define QUICKGUIMENUPANEL_H

#include "QuickGUIWindow.h"

namespace QuickGUI
{
	// forward declarations
	class ListItem;

	class _QuickGUIExport MenuPanelDesc :
			public WindowDesc
	{
	public:
		friend class DescFactory;
	protected:
		MenuPanelDesc();
		virtual ~MenuPanelDesc() {}
	public:

		Widget* menupanel_owner;
		float	menupanel_maxHeight;

		virtual Ogre::String getClass() { return "MenuPanelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "MenuPanel"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

	protected:
		bool window_titleBar;
	};

	class _QuickGUIExport MenuPanel :
		public Window
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Menu class creates and destroys MenuPanel
		friend class Menu;
		// ComboBox class creates and destroys MenuPanel
		friend class ComboBox;
	public:
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Adds a Widget to MenuPanel.
		*/
		void addWidget(Widget* w);

		/**
		* Returns the opacity of this widget as it is drawn on screen.
		* NOTE: Includes parent opacity if widget_inheritOpacity is true.
		*/
		virtual float getAbsoluteOpacity();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the owner of the MenuPanel.
		*/
		Widget* getOwner();

		/**
		* Returns true if this widget is a child of the widget given.
		*/
		virtual bool isChildOf(Widget* w);

		/**
		* Removes a Widget from this MenuPanel.
		*/
		void removeWidget(Widget* w);
		/**
		* Removes all Widgets from MenuPanel.
		*/
		void removeWidgets();

		/**
		* Sets the max height of the MenuPanel.
		*/
		void setMaxHeight(float height);

		/**
		* Recalculate Client widget_dimensions, relative to Widget's actual widget_dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		MenuPanel(const Ogre::String& name);
		virtual ~MenuPanel();

		MenuPanelDesc* mDesc;

		void _adjustHeight();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onLoseFocus(const EventArgs& args);
		void onVisibleChanged(const EventArgs& args);

	private:
	};
}

#endif
