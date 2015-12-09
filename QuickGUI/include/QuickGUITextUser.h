#ifndef QUICKGUIITEXTUSER_H
#define QUICKGUIITEXTUSER_H

#include "QuickGUIDesc.h"
#include "QuickGUIText.h"
#include "QuickGUIException.h"

namespace QuickGUI
{
	// forward declaration
	class Widget;
	class WidgetDesc;

	class _QuickGUIExport TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		TextUserDesc();
		virtual ~TextUserDesc() {}
	public:

		/// Describes the Text used in this Label
		TextDesc			textDesc;
		ColourValue	text_defaultColor;
		Ogre::String		text_defaultFontName;

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	/**
	* TextUser is an interface outlining all common interfaces a Widget that
	* manipulates Text would have.  This allows all Text using Widgets to share the
	* same convenience APIs.
	*/
	class _QuickGUIExport TextUser
	{
	public:

		/**
		* Internal function, do not use.
		*/
		void _initialize(Widget* owner, WidgetDesc* d);

		/**
		* Adds text to this object.
		*/
		void addText(Ogre::UTFString s, Ogre::FontPtr fp, const ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addText(Ogre::UTFString s);
		/**
		* Adds Text using Text Segments.
		*/
		void addText(std::vector<TextSegment> segments);

		/**
		* Clears the Text of this widget.
		*/
		void clearText();

		/**
		* Gets the color used when adding text without specifying color.
		*/
		ColourValue getDefaultColor();
		/**
		* Gets the font used when adding text without specifying font.
		*/
		Ogre::String getDefaultFont();
		/**
		* Returns the horizontal alignment of text within this widget's client area.
		*/
		virtual HorizontalTextAlignment getHorizontalTextAlignment();
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();
		/**
		* Returns the amount of width defined for this text.  Word wrapping occurs
		* when the allotted width is smaller than the width of the text.
		* NOTE: a value of 0 means no word wrapping will occur.
		*/
		float getTextAllottedWidth();
		/**
		* Returns the filtering used when drawing the text of this widget.
		*/
		BrushFilterMode getTextBrushFilterMode();
		/**
		* Returns the height of the Text;
		*/
		float getTextHeight();
		/**
		* Returns a list of Text Segments.  Each Text Segment has the same color and font.
		*/
		std::vector<TextSegment> getTextSegments();
		/**
		* Returns the width of the Text;
		*/
		float getTextWidth();
		/**
		* Returns the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		float getVerticalLineSpacing();

		/**
		* Sets the color used when adding text without specifying color.
		*/
		void setDefaultColor(const ColourValue& cv);
		/**
		* Sets the font used when adding text without specifying font.
		*/
		void setDefaultFont(const Ogre::String& fontName);
		/**
		* Sets all characters of the text to the specified font.
		*/
		virtual void setFont(const Ogre::String& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setFont(const Ogre::String& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the Horizontal alignment of Text as displayed within the Label area.
		*/
		virtual void setHorizontalTextAlignment(HorizontalTextAlignment a);
		/**
		* Sets the text for this object.
		*/
		virtual void setText(Ogre::UTFString s, Ogre::FontPtr fp, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		virtual void setText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		virtual void setText(Ogre::UTFString s);
		/**
		* Sets the Text using Text Segments.
		*/
		virtual void setText(std::vector<TextSegment> segments);
		/**
		* Defines the amount of width allowed for this text.  Word wrapping occurs
		* when the allotted width is smaller than the width of the text.
		* NOTE: set width to 0 to disable word wrapping.
		*/
		void setTextAllottedWidth(float allottedWidth);
		/**
		* Sets the filtering used when drawing the text.
		*/
		void setTextBrushFilterMode(BrushFilterMode m);
		/**
		* Sets all characters of the text to the specified color.
		*/
		virtual void setTextColor(const ColourValue& cv);
		/**
		* Sets the character at the index given to the specified color.
		*/
		void setTextColor(const ColourValue& cv, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified color.
		*/
		void setTextColor(const ColourValue& cv, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setTextColor(const ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setTextColor(const ColourValue& cv, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		void setVerticalLineSpacing(float distance);

	protected:
		TextUser();
		virtual ~TextUser();

		TextUserDesc* mDesc;
		Text* mText;

		// Pointer to the Widget that owns this interface
		Widget* mOwner;

		/**
		* Called when Text changes, allowing Widgets to easily override and use this notification.
		*/
		virtual void onTextChanged();

		/**
		* Flags the parent window as dirty causing its texture to be updated (redrawn).
		*/
		void Redraw();

	private:
	};
}

#endif
