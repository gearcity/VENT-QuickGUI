#ifndef QUICKGUIMENUIMAGEITEM_H
#define QUICKGUIMENUIMAGEITEM_H

#include "QuickGUIMenuItem.h"

namespace QuickGUI
{
	class _QuickGUIExport MenuImageItemDesc :
			public MenuItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		MenuImageItemDesc();
		virtual ~MenuImageItemDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "MenuImageItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "MenuImageItem"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport MenuImageItem :
		public MenuItem
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String DOWN;
		static const Ogre::String OVER;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.  This is not a recursive function!
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

	protected:
		MenuImageItem(const Ogre::String& name);
		virtual ~MenuImageItem();

		MenuImageItemDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* Closes all Menus belonging to parent ToolBar.
		*/
		virtual void onMouseButtonUp(const EventArgs& args);
		/**
		* Applies menu label "over" image
		*/
		virtual void onMouseEnter(const EventArgs& args);
		/**
		* Applies menu label "default" image
		*/
		void onMouseLeave(const EventArgs& args);

		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();

	private:
	};
}

#endif
