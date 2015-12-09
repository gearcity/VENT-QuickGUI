#ifndef QUICKGUICOLORPICKER_H
#define QUICKGUICOLORPICKER_H

#include "QuickGUIComponentWidget.h"
#include "QuickGUIImage.h"
#include "QuickGUIButton.h"

namespace QuickGUI
{
	class _QuickGUIExport ColorPickerDesc :
			public ComponentWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		ColorPickerDesc();
		virtual ~ColorPickerDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ColorPickerDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ColorPicker"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ColorPicker :
		public ComponentWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String COLORSOURCE;
		static const Ogre::String TRANSPARENCYBACKGROUND;
		static const Ogre::String TRANSPARENCYSLIDER;
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

	protected:
		ColorPicker(const Ogre::String& name);
		virtual ~ColorPicker();

		ColorPickerDesc* mDesc;

		Image* mColorSource;
		Button* mTransparencySlider;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onMouseDownOnImage(const EventArgs& args);
		void onMouseMovedOverImage(const EventArgs& args);

	private:
	};
}

#endif
