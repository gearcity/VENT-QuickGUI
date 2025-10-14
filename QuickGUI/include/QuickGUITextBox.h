#ifndef QUICKGUITEXTBOX_H
#define QUICKGUITEXTBOX_H

#include "QuickGUIText.h"
#include "QuickGUITextCursor.h"
#include "QuickGUITextInputValidator.h"
#include "QuickGUITimerManager.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport TextBoxDesc :
			public WidgetDesc
	{
	public:
		// Console has a TextBox Desc as a member
		friend class ConsoleDesc;
		friend class DescFactory;
	protected:
		TextBoxDesc();
		virtual ~TextBoxDesc() {}
	public:

		/// Amount of time until a cursor changes from visible to not visible, or vice versa.
		float textbox_cursorBlinkTime;
		ColourValue textbox_defaultColor;
		Ogre::String textbox_defaultFontName;
		/// Horizontal alignment of text within this widget's client area.
		HorizontalTextAlignment textbox_horizontalTextAlignment;
		/// Pixel Padding added to left/right side of client area, to allow drawing of text cursor
		float textbox_horizontalPadding;
		/// Amount of time a key must be held down before it starts repeating.
		float textbox_keyDownTime;
		/// Amount of time a key must be held down to repeat its input.
		float textbox_keyRepeatTime;
		bool textbox_maskText;
		Ogre::UTFString::code_point textbox_maskSymbol;
		unsigned int textbox_maxCharacters;
		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment textbox_verticalTextAlignment;
		Ogre::String textbox_textCursorDefaultSkinTypeName;
		Point textbox_textPosition;

		

		/// Describes the Text used in this TextBox
		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TextBoxDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TextBox"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TextBox :
		public Widget
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
		* Removes any TextInputValidator this TextBox might have been using.
		*/
		void clearTextInputValidator();
		/**
		* Clears the Text of this widget.
		*/
		void clearText();

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the color used for all newly added characters.
		*/
		ColourValue getDefaultColor();
		/**
		* Returns the font used for all newly added characters.
		*/
		Ogre::String getDefaultFont();
		/**
		* Returns the number of pixels padded on the left and right side of the TextBox, allowing
		* the Cursor to be drawn. (Otherwise the cursor may get partially clipped)
		*/
		float getHorizontalPadding();
		/**
		* Gets the Horizontal alignment of Text as displayed within the TextBox area.
		*/
		HorizontalTextAlignment getHorizontalTextAlignment();
		/**
		* Returns the symbol used to mask text, if it is to be masked.
		*/
		Ogre::UTFString::code_point getMaskSymbol();
		/**
		* Returns true if the text is masked, false otherwise.
		*/
		bool getMaskText();
		/**
		* Returns the max number of characters this TextBox supports.
		*/
		int getMaxCharacters();
		/**
		* Gets the area of the TextBox that is drawn on screen
		*/
		Rect getScreenRect();
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();
		/**
		* Returns the filtering used when drawing the text of this widget.
		*/
		BrushFilterMode getTextBrushFilterMode();
		/**
		* Returns a list of Text Segments.  Each Text Segment has the same color and font.
		*/
		std::vector<TextSegment> getTextSegments();
		/**
		* Gets the "type" of the Text Cursor used.
		*/
		Ogre::String getTextCursorSkinType();

		void onWindowDrawn(const EventArgs& args);

		/**
		* Removes a character from the text at the index given, and
		* positions the text cursor before that character.
		* NOTE: If the cursor index is -1 it will stay -1.
		*/
		void removeCharacter(int index);

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
		* Enabled Widgets receive mouse and keyboard events via injections to the GUIManager.
		* Disabled Widgets can only receive these events if they are manually fired.
		*/
		virtual void setEnabled(bool enabled);
		/**
		* Sets all characters of the text to the specified font.
		*/
		void setFont(const Ogre::String& fontName);
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
		* Sets the number of pixels padded on the left and right side of the TextBox, allowing
		* the Cursor to be drawn. (Otherwise the cursor may get partially clipped)
		*/
		void setHorizontalPadding(float padding);
		/**
		* Sets the Horizontal alignment of Text as displayed within the TextBox area.
		*/
		void setHorizontalTextAlignment(HorizontalTextAlignment a);
		/**
		* Sets whether or not the Text will be masked. (password box)
		*/
		void setMaskText(bool mask, Ogre::UTFString::code_point textbox_maskSymbol);
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
		void setText(Ogre::UTFString s);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, Ogre::FontPtr fp, const ColourValue& cv);
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
		void setTextColor(const ColourValue& cv);
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
		* Sets the Vertical alignment of Text as displayed within the TextBox area.
		*/
		void setVerticalTextAlignment(VerticalTextAlignment a);
		/**
		* Sets the "type" of the Text Cursor used.
		*/
		void setTextCursorSkinType(const Ogre::String type);
		/** Sets the input validator used to determine whether input is accepted.
            @param
                function member function assigned to process input.  Given in the form of myClass::myFunction.
				Function must return bool, and take a code point, index, and UTFString as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				setTextInputValidator(myClass::myFunction,this);
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::TextBox::setTextInputValidator' : function does not take x arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns bool, and takes parameters "Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText".
        */
		template<typename T> void setTextInputValidator(bool (T::*TextInputValidator)(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText), T* obj)
		{
			if(mTextInputValidatorSlot != NULL)
				OGRE_DELETE_T(mTextInputValidatorSlot,TextInputValidatorSlot,Ogre::MEMCATEGORY_GENERAL);

			mTextInputValidatorSlot = OGRE_NEW_T(TextInputValidatorPointer<T>,Ogre::MEMCATEGORY_GENERAL)(TextInputValidator,obj);
		}

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

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
		TextBox(const Ogre::String& name);
		virtual ~TextBox();

		Text* mText;

		TextCursor* mTextCursor;
		int mCursorIndex;
		Point mCursorPosition;

		TextInputValidatorSlot* mTextInputValidatorSlot;

		/// Record is last key down was a function button or character input.
		bool mFunctionKeyDownLast;
		/// Store the last key that went down
		KeyEventArgs mLastKnownInput;
		/// Timer that repeats keys
		Timer* mKeyRepeatTimer;
		/**
		* Callback that repetitively inputs the last held down key.
		*/
		void keyRepeatTimerCallback();
		/// Timer that starts repeat timer
		Timer* mKeyDownTimer;
		/**
		* Callback that starts the key repeat timer.
		*/
		void keyDownTimerCallback();

		/// Storing reference to font for quick use.
		Ogre::FontPtr mCurrentFont;

		/// Timer that toggles cursor on and off.
		Timer* mBlinkTimer;
		/**
		* Toggles blinker on and off.
		*/
		void blinkTimerCallback();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		TextBoxDesc* mDesc;

		// Store the clip region so the cursor can be clipped properly.
		Rect mTextBoxClipRegion;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onCharEntered(const EventArgs& args);
		void onKeyDown(const EventArgs& args);
		void onKeyUp(const EventArgs& args);
		void onKeyboardInputGain(const EventArgs& args);
		void onKeyboardInputLose(const EventArgs& args);
		void onMouseButtonDown(const EventArgs& args);
		void onTripleClick(const EventArgs& args);
		void onVisibleChanged(const EventArgs& args);

	private:
	};
}

#endif
