#ifndef QUICKGUIBUTTON_H
#define QUICKGUIBUTTON_H

#include "QuickGUIFont.h"
#include "QuickGUILabel.h"

namespace QuickGUI
{
	class _QuickGUIExport ButtonDesc :
			public LabelDesc
	{
	public:
		friend class DescFactory;
	protected:
		ButtonDesc();
		virtual ~ButtonDesc() {}
	public:

		/// Name of the image file to display
		std::string	button_imageName;
		bool			button_tileImage;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual std::string getClass() { return "ButtonDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual std::string getWidgetClass() { return "Button"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Button :
		public Label
	{
	public:
		// Skin Constants
		static const std::string DEFAULT;
		static const std::string OVER;
		static const std::string DOWN;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:		
		// ComboBox changes its buttons state.
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
		virtual std::string getClass();
		/**
		* Returns the name of the image file used for display.
		*/
		std::string getImageName();
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
		void setImage(const std::string& name);
		/**
		* Sets the text for this object.
		*/
		void setText(UTFString s, const Font& font, const ColourValue& cv);
		QUICKGUI_DEPRECATED("Use the overload that takes a Font from Root::getFont.")
		void setText(UTFString s, Ogre::FontPtr fp, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(UTFString s, const std::string& fontName, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(UTFString s);
		/**
		* Sets the Text using Text Segments.
		*/
		void setText(std::vector<TextSegment> segments);
		/**
		* Sets whether or not Image will tile within client dimensions.
		*/
		void setTileImage(bool tile);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		
		virtual void setHiddenID(int ID);
		virtual int getHiddenID();

		virtual void setHiddenString(std::string str);
		virtual std::string getHiddenString();

	protected:
		Button(const std::string& name);
		virtual ~Button();

		ButtonDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_OVER)
		*/
		void onMouseEnter(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_DEFAULT)
		*/
		void onMouseLeave(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_DOWN)
		*/
		void onMouseLeftButtonDown(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_OVER)
		*/
		void onMouseLeftButtonUp(const EventArgs& args);

		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();

	private:
	  int hiddenInt;
	  std::string hiddenString;
	};
}

#endif
