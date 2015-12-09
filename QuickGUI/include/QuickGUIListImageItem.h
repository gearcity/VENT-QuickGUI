#ifndef QUICKGUILISTIMAGEITEM_H
#define QUICKGUILISTIMAGEITEM_H

#include "QuickGUIListItem.h"

namespace QuickGUI
{
	class _QuickGUIExport ListImageItemDesc :
		public ListItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		ListImageItemDesc();
		virtual ~ListImageItemDesc() {}
	public:

		Ogre::String listimageitem_imageName;
		bool listimageitem_tileImage;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListImageItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ListImageItem"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ListImageItem :
		public ListItem
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
		/**
		* Returns the name of the image file used for display.
		*/
		Ogre::String getImageName();
		/**
		* Returns true if image is tiled within client dimensions, false otherwise.
		*/
		bool getTileImage();

		/**
		* Sets the name of the image file to be displayed.  For example, name could could be
		* qgui.image.png.
		* NOTE: Image must be listed within an Ogre resource directory.
		* NOTE: Setting the name to "" will result in only the background showing.
		*/
		void setImage(const Ogre::String& name);
		/**
		* Sets whether or not Image will tile within client dimensions.
		*/
		void setTileImage(bool tile);

	protected:
		ListImageItem(const Ogre::String& name);
		virtual ~ListImageItem();

		ListImageItemDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
