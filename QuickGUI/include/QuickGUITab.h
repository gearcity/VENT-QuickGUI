#ifndef QUICKGUIHTAB_H
#define QUICKGUIHTAB_H

#include "QuickGUIFont.h"
#include "QuickGUILabel.h"

namespace QuickGUI
{
	class _QuickGUIExport TabDesc :
			public LabelDesc
	{
	public:
		friend class DescFactory;
	protected:
		TabDesc();
		virtual ~TabDesc() {}
	public:

		/// width in pixels of the front edge of the tab.
		float tab_frontWidth;
		std::string tab_imageName;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual std::string getClass() { return "TabDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual std::string getWidgetClass() { return "Tab"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Tab :
		public Label
	{
	public:
		// Skin Constants
		static const std::string FRONT;
		static const std::string MAIN;
		static const std::string ICON;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Needed for TabPage to serial write TextDesc to disk
		friend class TabPage;
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the Border underneath the point p.
		* NOTE: Point is assumed to be relative to the Window.
		* NOTE: Exception is thrown if point is not over a border.
		*/
		virtual BorderSide getBorderSide(Point p);
		/**
		* Returns the class name of this Widget.
		*/
		virtual std::string getClass();

		/**
		* Returns true if the point is over a border, false otherwise.
		* NOTE: Point is assumed to be relative to the Window.
		*/
		virtual bool overBorderSide(Point p);

		/**
		* Sets all characters of the text to the specified font.
		*/
		void setFont(const std::string& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setFont(const std::string& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setFont(const std::string& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const std::string& fontName, UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const std::string& fontName, UTFString s, bool allOccurrences);
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
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		Tab(const std::string& name);
		virtual ~Tab();

		TabDesc* mDesc;

		// adjusts allotted width and width of Tab based on Text.
		void _adjustTabWidth();

		Size mImageSize;
		// Shrinks or grow the image for the tab so that it will fit the tab height.
		// Width is adjusted proportionately.
		void _deriveImageSize();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);

		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();

	private:
	};
}

#endif
