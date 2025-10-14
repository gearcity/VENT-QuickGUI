#ifndef QUICKGUIIMAGE_H
#define QUICKGUIIMAGE_H

#include "QuickGUIWidget.h"
#include "QuickGUITimerManager.h"

namespace QuickGUI
{
	class _QuickGUIExport ImageDesc :
			public WidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		ImageDesc();
		virtual ~ImageDesc() {}
	public:

		/// Name of the image file to display
		Ogre::String	image_imageName;
		bool			image_tileImage;
		/// Useful for applying render to texture (RTT) to Image
		bool			image_updateEveryFrame;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ImageDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Image"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Image :
		public Widget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
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
		* Gets whether or not the Image will be redrawn every frame.
		*/
		bool getUpdateEveryFrame();

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
		/**
		* Sets whether or not the Image will be redrawn every frame.
		* NOTE: This also causes the Images parent window to be re-drawn, which can be costly.
		* The main use for this is for RTT. (Render to Texture)
		*/
		void setUpdateEveryFrame(bool update);

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
		Image(const Ogre::String& name);
		virtual ~Image();

		ImageDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/// Timer that toggles cursor on and off.
		Timer* mUpdateTimer;
		/**
		* Forces an update of the Image.
		*/
		void updateTimerCallback();

		/**
		* Sets whether the widget will receive mouse over events simply by having the mouse over
		* its texture dimensions, or only when the cursor is over non transparent parts.
		*/
		void setTransparencyPicking(bool transparencyPicking);

	private:
	};
}

#endif
