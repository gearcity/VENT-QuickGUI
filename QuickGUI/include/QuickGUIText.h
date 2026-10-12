#ifndef QUICKGUITEXT_H
#define QUICKGUITEXT_H

#include "QuickGUIDesc.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISize.h"
#include "QuickGUITextAlignment.h"
#include "QuickGUIUVRect.h"
#include "QuickGUIFont.h"

#include "OgreColourValue.h"
#include <OgreFontManager.h>
#include "OgrePrerequisites.h"
#include "OgreTextureManager.h"
#include "QuickGUIUTFString.h"

#include <list>
#include <vector>

namespace QuickGUI
{
	// forward declarations
	class Character;
	class Text;
	class TextLine;

	class _QuickGUIExport TextSegment
	{
	public:
		TextSegment();
		TextSegment(const std::string FontName, const ColourValue& Color, const UTFString& Text);
		TextSegment(const ColourValue& Color, const UTFString& Text);
		TextSegment(const UTFString& Text);

		// Font of this segment of text
		std::string fontName;
		// Color of this segment of text
		ColourValue color;
		// Array of code points
		UTFString text;
	};

	class _QuickGUIExport TextDesc :
		public Desc
	{
	public:
		TextDesc();

		float allottedWidth;
		BrushFilterMode	brushFilterMode;
		HorizontalTextAlignment horizontalTextAlignment;
		float verticalLineSpacing;
		std::vector<TextSegment> segments;

		// Adds performance boost by letting Desc update its segments before serial writing,
		// Instead of updating segments for every text changing operation.
		Text* text;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual std::string getClass() { return "TextDesc"; }
		/**
		* Returns the height of the Text if it were treated as one line of text.
		*/
		float getTextHeight();
		/**
		* Returns the number of characters.
		*/
		int getTextLength();
		/**
		* Returns the width of the Text if it were treated as one line of text.
		*/
		float getTextWidth();

		/**
		* Restore properties to default values
		*/
		void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Text
	{
	public:
		friend class SerialWriter;
	public:
		/// Define Unicode constants
		enum 
		{
			UNICODE_NEL		= 0x0085,
			UNICODE_CR		= 0x000D,
			UNICODE_LF		= 0x000A,
			UNICODE_TAB		= 0x0009,
			UNICODE_SPACE	= 0x0020,
			UNICODE_ZERO	= 0x0030,
		};

		/**
		* Returns true if the given unicode character is a Tab, false otherwise.
		*/
		static inline bool	isTab			(UTFString::unicode_char c) { return c == UNICODE_TAB; }
		/**
		* Returns true if the given unicode character is a Space, false otherwise.
		*/
		static inline bool	isSpace			(UTFString::unicode_char c) { return c == UNICODE_SPACE; }
		/**
		* Returns true if the given unicode character is a NewLine ('\n' or '\r'), false otherwise.
		*/
		static inline bool	isNewLine		(UTFString::unicode_char c) { return c == UNICODE_CR || c == UNICODE_LF || c == UNICODE_NEL; }
		/**
		* Returns true if the given unicode character is a Null character, false otherwise.
		*/
		static inline bool  isNullCharacter (UTFString::unicode_char c) { return c == UNICODE_NEL; }
		/**
		* Returns true if the given unicode character is a Space, Tab, or NewLine, false otherwise.
		*/
		static inline bool	isWhiteSpace	(UTFString::unicode_char c) { return isTab(c) || isSpace(c) || isNewLine(c); }
	public:
		/**
		* Converts '\n' and '\r' into "\\n", and '\t' into "\\t".
		*/
		static void convertWhitespaceToText(UTFString& s);
		/**
		* Converts "\\n" and "\\r" into '\n', and "\\t" into '\t'.
		*/
		static void convertTextToWhiteSpace(UTFString& s);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static Ogre::FontPtr getFirstAvailableFont();
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static Ogre::FontPtr getFont(const std::string& fontName);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getFontHeight(const std::string& fontName);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getFontHeight(Ogre::FontPtr fp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static Ogre::TexturePtr getFontTexture(const std::string& fontName);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static Ogre::TexturePtr getFontTexture(Ogre::FontPtr fp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static std::string getFontTextureName(const std::string& fontName);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static std::string getFontTextureName(Ogre::FontPtr fp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getGlyphHeight(const std::string& fontName, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getGlyphHeight(Ogre::FontPtr fp, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static UVRect getGlyphUVCoords(const std::string& fontName, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static UVRect getGlyphUVCoords(Ogre::FontPtr fp, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static Size getGlyphSize(const std::string& fontName, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static Size getGlyphSize(Ogre::FontPtr fp, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getGlyphWidth(const std::string& fontName, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getGlyphWidth(Ogre::FontPtr fp, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getGlyphAdvance(const std::string& fontName, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getGlyphAdvance(Ogre::FontPtr fp, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getGlyphBearing(const std::string& fontName, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getGlyphBearing(Ogre::FontPtr fp, UTFString::code_point cp);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getTextWidth(const std::string& fontName, UTFString s);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static float getTextWidth(Ogre::FontPtr fp, UTFString s);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static void saveFontTextureToFile(const std::string& fontName, const std::string& fileName);
		QUICKGUI_DEPRECATED("Use Root::getFont and the Font class.")
		static void saveFontTextureToFile(Ogre::FontPtr fp, const std::string& fileName);
	public:
		Text(TextDesc& d);
		~Text();

		/**
		* Inserts a character at the specified index.
		* NOTE: The Text class will delete characters on destruction.
		*/
		void addCharacter(Character* c, unsigned int index);
		/**
		* Inserts a character to the end of the text.
		* NOTE: The Text class will delete characters on destruction.
		*/
		void addCharacter(Character* c);
		/**
		* Adds text to this object.
		*/
		void addText(UTFString s, const Font& font, const ColourValue& cv);
		QUICKGUI_DEPRECATED("Use the overload that takes a Font from Root::getFont.")
		void addText(UTFString s, Ogre::FontPtr fp, const ColourValue& cv);
		/**
		* Adds Text using Text Segments.
		*/
		void addText(std::vector<TextSegment> segments);
		/**
		* Adds new line of text to this object.
		*/
		void addTextLine(UTFString s, const Font& font, const ColourValue& cv);
		QUICKGUI_DEPRECATED("Use the overload that takes a Font from Root::getFont.")
		void addTextLine(UTFString s, Ogre::FontPtr fp, const ColourValue& cv);
		/**
		* Adds new line of Text using Text Segments.
		*/
		void addTextLine(std::vector<TextSegment> segments);

		/**
		* Removes all highlighting from text.
		*/
		void clearHighlights();

		/**
		* Returns true if any of the text is highlighted.
		*/
		bool getHighlight();

		/**
		* Clears text.
		*/
		void clearText();

		/**
		* Draws the text to the current render target.
		*/
		void draw(Point& position);

		/**
		* Returns true if the text has no characters, false otherwise.
		*/
		bool empty();

		/**
		* Returns the amount of width defined for this text.  Word wrapping occurs
		* when the allotted width is smaller than the width of the text.
		*/
		float getAllottedWidth();
		/**
		* Returns the filtering used when drawing the skin of this widget.
		*/
		BrushFilterMode getBrushFilterMode();
		/**
		* Returns the Character at the given index.
		*/
		Character* getCharacter(unsigned int index);
		/**
		* Returns the position of the character given relative to the Text.
		* NOTE: The position of the Character class is relative to its TextLine.
		*/
		Point getCharacterPosition(unsigned int index);
		/**
		* Returns the y position of the character given relative to the Text.
		* NOTE: The y position of the Character class is relative to its TextLine.
		*/
		float getCharacterYPosition(unsigned int index);
		/**
		* Iterates through all TextLines and returns the index of the character
		* closest to the position given.
		*/
		int getCursorIndexAtPosition(const Point& p);
		/**
		* Returns the horizontal alignment of this text.
		*/
		HorizontalTextAlignment getHorizontalTextAlignment();
		/**
		* Returns the index of the next word after the index given, or -1 if
		* no more words remain.
		*/
		int getIndexOfNextWord(unsigned int index);
		/**
		* Returns the index of the previous word before the index given, or the beginning if
		* no more words are before the current one.
		*/
		int getIndexOfPreviousWord(unsigned int index);
		/**
		* Returns the index of the last character in the TextLine with the index given.
		*/
		int getIndexOfTextLineBegin(unsigned int index);
		/**
		* Returns the index of the first character in the TextLine with the index given.
		*/
		int getIndexOfTextLineEnd(unsigned int index);
		/**
		* Returns the number of characters in this Text.
		*/
		int getLength();
		/**
		* Returns the position of the character at the index specified.
		*/
		Point getPositionAtCharacterIndex(unsigned int index);
		/**
		* Returns the Width and Height of this text.
		*/
		Size getSize();
		/**
		* Returns a string of characters representing the text.
		*/
		UTFString getText();
		/**
		* Returns the desc object representing this Text instance.
		*/
		TextDesc* getTextDesc();
		/**
		* Returns the height of text.
		* NOTE: the allotted width affects the number of TextLines generated, which affects the height.
		*/
		float getTextHeight();
		/**
		* Returns the TextLine from the Character index given.
		*/
		TextLine* getTextLineFromIndex(unsigned int index);
		/**
		* Returns the index of the first character in the TextLine given.
		*/
		int getTextLineBeginIndex(TextLine* textLine);
		/**
		* Returns the index of the last character in the TextLine given.
		*/
		int getTextLineEndIndex(TextLine* textLine);
		/**
		* Returns the width of text as if it were all on one line.
		*/
		float getTextWidth();
		/**
		* Returns a list of Text Segments.  Each Text Segment has the same color and font.
		*/
		std::vector<TextSegment> getTextSegments();
		/**
		* Returns the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		float getVerticalLineSpacing();

		/**
		* Highlights all the text.
		*/
		void highlight();
		/**
		* Highlights the character at the index given.
		*/
		void highlight(unsigned int index);
		/**
		* Highlights all characters within the defined range.
		*/
		void highlight(unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be highlighted, otherwise only the first occurrence is highlighted.
		*/
		void highlight(UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be highlighted, otherwise only the first occurrence is highlighted.
		*/
		void highlight(UTFString s, bool allOccurrences);

		/**
		* Removes a character from the index given.
		*/
		void removeCharacter(unsigned int index);

		/**
		* Defines the amount of width allowed for this text.  Word wrapping occurs
		* when the allotted width is smaller than the width of the text.
		*/
		void setAllottedWidth(float pixelWidth);
		/**
		* Sets the filtering used when drawing the text.
		*/
		void setBrushFilterMode(BrushFilterMode m);
		/**
		* Sets all characters of the text to the specified color.
		*/
		void setColor(const ColourValue& cv);
		/**
		* Sets the character at the index given to the specified color.
		*/
		void setColor(const ColourValue& cv, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified color.
		*/
		void setColor(const ColourValue& cv, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setColor(const ColourValue& cv, UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setColor(const ColourValue& cv, UTFString s, bool allOccurrences);
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
		* Sets whether or not the Text will be masked. (password box)
		*/
		void setMaskText(bool mask, UTFString::code_point maskSymbol);
		/**
		* Sets the text for this object.
		*/
		void setText(UTFString s, const Font& font, const ColourValue& cv);
		QUICKGUI_DEPRECATED("Use the overload that takes a Font from Root::getFont.")
		void setText(UTFString s, Ogre::FontPtr fp, const ColourValue& cv);
		/**
		* Sets the Text using Text Segments.
		*/
		void setText(std::vector<TextSegment> segments);
		/**
		* Sets the Horizontal alignment of Text when drawn.
		*/
		void setHorizontalTextAlignment(HorizontalTextAlignment a);
		/**
		* Sets the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		void setVerticalLineSpacing(float distance);

		/**
		* Forces recalculation of TextLines.
		*/
		void update();

	protected:
		TextDesc* mTextDesc;

		bool mMaskText;
		UTFString::code_point mMaskSymbol;

		UTFString mText;
		// Store information of every character in this Text object
		std::list<Character*> mCharacters;

		std::vector<TextLine*> mTextLines;

		// Store whether TextLines need to be updated or not
		bool mTextLinesDirty;

		/**
		* Checks if TextLines need to be updated.  If so, they are
		* destroyed and recreated.
		*/
		void _checkTextLines();
		/**
		* Create Text Lines and populate them on a word by word basis
		*/
		void _createTextLines();
		/**
		* Destroys Text Lines and restores Character position to default. (0,0)
		*/
		void _destroyTextLines();
		/**
		* Draws Text Lines to current Render Target starting at point p.
		*/
		void _drawTextLines(Point& position);

	private:
	};
}

#endif
