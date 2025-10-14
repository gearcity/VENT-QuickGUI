#ifndef QUICKGUILISTTEXTITEM_H
#define QUICKGUILISTTEXTITEM_H

#include "QuickGUIListItem.h"
#include "QuickGUITextUser.h"

namespace QuickGUI
{
	class _QuickGUIExport ListTextItemDesc :
		public ListItemDesc,
		public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		ListTextItemDesc();
		virtual ~ListTextItemDesc() {}
	public:


		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment listtextitem_verticalTextAlignment;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListTextItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ListTextItem"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ListTextItem :
		public ListItem,
		public TextUser
	{
	public:
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// ComboBox works with ListTextItems specifically and needs to be able to destroy them.
		friend class ComboBox;

		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

	protected:
		ListTextItem(const Ogre::String& name);
		virtual ~ListTextItem();

		ListTextItemDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
