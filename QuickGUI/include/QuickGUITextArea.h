/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

#ifndef QUICKGUITEXTAREA_H
#define QUICKGUITEXTAREA_H

#include "QuickGUIText.h"
#include "QuickGUITextCursor.h"
#include "QuickGUITimerManager.h"
#include "QuickGUIContainerWidget.h"

namespace Ogre
{
	// forward declarations
	class Font;
}

namespace QuickGUI
{
	class _QuickGUIExport TextAreaDesc :
		public ContainerWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		TextAreaDesc();
		virtual ~TextAreaDesc() {}
	public:

		/// Amount of time until a cursor changes from visible to not visible, or vice versa.
		float textarea_cursorBlinkTime;
		ColourValue textarea_defaultColor;
		Ogre::String textarea_defaultFontName;
		/// Pixel Padding added to left/right side of client area, to allow drawing of text cursor
		float textarea_horizontalPadding;
		unsigned int textarea_maxCharacters;
		/// Sets the text to read only
		bool textarea_readOnly;
		Ogre::String textarea_textCursorDefaultSkinTypeName;

		/// Describes the Text used in this TextBox
		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TextAreaDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TextArea"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TextArea :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String TEXTOVERLAY;
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
		* Adds a character in front of the TextCursor, and increments the TextCursor
		* position.
		*/
		void addCharacter(Ogre::UTFString::code_point cp);
		/**
		* Adds text to this object.
		*/
		void addText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv);
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
		* Adds text to this object.
		*/
		void addTextLine(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addTextLine(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addTextLine(Ogre::UTFString s);
		/**
		* Adds Text using Text Segments.
		*/
		void addTextLine(std::vector<TextSegment> segments);

		/**
		* Clears the Text of this widget.
		*/
		void clearText();

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the position of the index relative to the widget.
		*/
		Point getCursorIndexPosition(int index);
		/**
		* Gets the color used when adding characters to the TextBox.
		*/
		ColourValue getDefaultColor();
		/**
		* Gets the font used when adding character to the TextBox.
		*/
		Ogre::String getDefaultFont();
		/**
		* Gets the horizontal alignment of text.
		*/
		HorizontalTextAlignment getHorizontalAlignment();
		/**
		* Returns the number of pixels padded on the left and right side of the TextArea, allowing
		* the Cursor to be drawn. (Otherwise the cursor may get partially clipped)
		*/
		float getHorizontalPadding();
		/**
		* Returns true if the text cannot be manipulated via input, false otherwise.
		*/
		bool getReadOnly();
		/**
		* Gets the area of the TextBox that is drawn on screen
		*/
		Rect getScreenRect();
		/**
		* Returns the filtering used when drawing the text of this widget.
		*/
		BrushFilterMode getTextBrushFilterMode();
		/**
		* Gets the TextCursor's skin type.
		*/
		Ogre::String getTextCursorSkinType();
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();
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
		* Returns true if the index is above the currently shown portion of text, false otherwise.
		*/
		bool isCursorIndexAboveViewableText(int index);
		/**
		* Returns true if the index is below the currently shown portion of text, false otherwise.
		*/
		bool isCursorIndexBelowViewableText(int index);
		/**
		* Returns true if the index is visible in the currently shown portion of text, false otherwise.
		*/
		bool isCursorIndexVisible(int index);

		/**
		* Moves Text Cursor to the character below the cursor.
		*/
		void moveCursorDown();
		/**
		* Moves Text Cursor to the character to the left of the cursor.
		*/
		void moveCursorLeft();
		/**
		* Moves Text Cursor to the character to the right of the cursor.
		*/
		void moveCursorRight();
		/**
		* Moves Text Cursor to the character above the cursor.
		*/
		void moveCursorUp();

		void onWindowDrawn(const EventArgs& args);

		/**
		* Removes a character from the text at the index given, and
		* positions the text cursor before that character.
		* NOTE: If the cursor index is -1 it will stay -1.
		*/
		void removeCharacter(int index);

		/**
		* Sets the cursor to the bottom of the text, scrolling the text into view.
		*/
		void scrollToBottom();
		/**
		* Sets the cursor to the top of the text, scrolling the text into view.
		*/
		void scrollToTop();
		/**
		* Sets the index of the text cursor. A cursor index represents the position
		* to the left of the character with the same index. -1 represents the right most
		* position.
		*/
		void setCursorIndex(int index);
		/**
		* Sets the color used when adding characters to the TextBox.
		*/
		void setDefaultColor(const ColourValue& cv);
		/**
		* Sets the font used when adding character to the TextBox.
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
		* Sets the horizontal alignment of text.
		*/
		void setHorizontalAlignment(HorizontalTextAlignment a);
		/**
		* Sets the number of pixels padded on the left and right side of the TextBox, allowing
		* the Cursor to be drawn. (Otherwise the cursor may get partially clipped)
		*/
		void setHorizontalPadding(float padding);
		/**
		* Sets the maximum number of characters that this TextBox will support.
		*/
		void setMaxCharacters(unsigned int max);
		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s);
		/**
		* Sets the Text using Text Segments.
		*/
		void setText(std::vector<TextSegment> segments);
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
		* Sets the TextCursor's skin type.
		*/
		void setTextCursorSkinType(const Ogre::String& skinTypeName);
		/**
		* If set, the TextCursor will not display, and text cannot be added or removed using mouse/keyboard input.
		*/
		void setReadOnly(bool readOnly);
		/**
		* Sets the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		void setVerticalLineSpacing(float distance);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();
		/**
		* Recalculate Virtual dimensions, the minimum size required to encapsulate the client area and all Child widgets.
		*/
		virtual void updateVirtualDimensions();

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
		using Widget::setQueryFlags;
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
		TextArea(const Ogre::String& name);
		virtual ~TextArea();

		Text* mText;

		TextCursor* mTextCursor;
		int mCursorIndex;
		Point mCursorPosition;

		Point _convertScreenToTextCoordinates(const Point& p);

		/// Position of Text, used for scrolling support
		Point mTextPosition;

		/// Storing reference to font for quick use.
		Ogre::Font* mCurrentFont;

		/// Timer that toggles cursor on and off.
		Timer* mBlinkTimer;
		/**
		* Toggles blinker on and off.
		*/
		void blinkTimerCallback();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		TextAreaDesc* mDesc;

		// Store the clip region so the cursor can be clipped properly.
		Rect mTextBoxClipRegion;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onCharEntered(const EventArgs& args);
		void onKeyDown(const EventArgs& args);
		void onKeyboardInputGain(const EventArgs& args);
		void onKeyboardInputLose(const EventArgs& args);
		void onMouseButtonDown(const EventArgs& args);
		void onTripleClick(const EventArgs& args);
		void onVisibleChanged(const EventArgs& args);

		virtual void _setScrollY(float percentage);

	private:
	};
}

#endif
