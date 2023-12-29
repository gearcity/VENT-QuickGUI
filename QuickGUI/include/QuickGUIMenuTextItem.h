#ifndef QUICKGUIMENUTEXTITEM_H
#define QUICKGUIMENUTEXTITEM_H

#include "QuickGUIMenuItem.h"
#include "QuickGUITextUser.h"

namespace QuickGUI
{
	class _QuickGUIExport MenuTextItemDesc :
			public MenuItemDesc,
			public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		MenuTextItemDesc();
		virtual ~MenuTextItemDesc() {}
	public:

		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment menulabel_verticalTextAlignment;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "MenuTextItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "MenuTextItem"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport MenuTextItem :
		public MenuItem,
		public TextUser
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
		/**
		* Returns the vertical alignment of text within this widget's client area.
		*/
		VerticalTextAlignment getVerticalTextAlignment();

		/**
		* Sets the Vertical alignment of Text as displayed within the Label area.
		*/
		void setVerticalTextAlignment(VerticalTextAlignment a);

	protected:
		MenuTextItem(const Ogre::String& name);
		virtual ~MenuTextItem();

		MenuTextItemDesc* mDesc;

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
