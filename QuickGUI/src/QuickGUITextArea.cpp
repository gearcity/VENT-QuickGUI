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

#include "QuickGUITextArea.h"
#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUISheet.h"
#include "QuickGUICharacter.h"
#include "QuickGUITextLine.h"
#include "QuickGUIVScrollBar.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIDescManager.h"

#include "OgreFont.h"

namespace QuickGUI
{
	const Ogre::String TextArea::BACKGROUND = "background";
	const Ogre::String TextArea::TEXTOVERLAY = "textoverlay";

	void TextArea::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TextArea");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(TEXTOVERLAY);
		d->defineSkinReference(HSCROLLBAR,"HScrollBar");
		d->defineSkinReference(VSCROLLBAR,"VScrollBar");
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TextArea",d);
	}

	TextAreaDesc::TextAreaDesc() :
		ContainerWidgetDesc()
	{
		resetToDefault();
	}

	void TextAreaDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(125,100);

		textarea_cursorBlinkTime = 0.5;
		textarea_defaultColor = Root::getSingleton().getDefaultColor();
		textarea_defaultFontName = Root::getSingleton().getDefaultFontName();
		textarea_horizontalPadding = 2;
		textarea_maxCharacters = 255;
		textarea_readOnly = false;
		textarea_textCursorDefaultSkinTypeName = "default";

		textDesc.resetToDefault();
	}

	void TextAreaDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		TextAreaDesc* defaultValues = DescManager::getSingleton().createDesc<TextAreaDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("CursorBlinkTime",				&textarea_cursorBlinkTime,					defaultValues->textarea_cursorBlinkTime);
		b->IO("DefaultColor",					&textarea_defaultColor,						defaultValues->textarea_defaultColor);
		b->IO("DefaultFontName",				&textarea_defaultFontName,					defaultValues->textarea_defaultFontName);
		b->IO("MaxCharacters",					&textarea_maxCharacters,					defaultValues->textarea_maxCharacters);
		b->IO("TextCursorDefaultSkinTypeName",	&textarea_textCursorDefaultSkinTypeName,	defaultValues->textarea_textCursorDefaultSkinTypeName);
		b->IO("TBHorizontalPadding",			&textarea_horizontalPadding,				defaultValues->textarea_horizontalPadding);

		DescManager::getSingleton().destroyDesc(defaultValues);

		textDesc.serialize(b);
	}

	TextArea::TextArea(const Ogre::String& name) :
		ContainerWidget(name),
		mText(NULL),
		mDesc(NULL),
		mTextCursor(NULL),
		mCursorIndex(0),
		mBlinkTimer(NULL)
	{
		addWidgetEventHandler(WIDGET_EVENT_CHARACTER_KEY,&TextArea::onCharEntered,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&TextArea::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_GAIN,&TextArea::onKeyboardInputGain,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_LOSE,&TextArea::onKeyboardInputLose,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TextArea::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_TRIPLE,&TextArea::onTripleClick,this);
		addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&TextArea::onVisibleChanged,this);

		mTextCursor = OGRE_NEW_T(TextCursor,Ogre::MEMCATEGORY_GENERAL)(this);
	}

	TextArea::~TextArea()
	{
		if(mWindow != NULL)
			mWindow->removeWindowEventHandler(WINDOW_EVENT_DRAWN,this);

		TimerManager::getSingleton().destroyTimer(mBlinkTimer);

		OGRE_DELETE_T(mTextCursor,TextCursor,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);
	}

	Point TextArea::_convertScreenToTextCoordinates(const Point& p)
	{
		// Convert position to coordinates relative to TextBox position
		Point relativePosition;
		relativePosition.x = p.x - (mTexturePosition.x + mWindow->getPosition().x);
		relativePosition.y = p.y - (mTexturePosition.y + mWindow->getPosition().y);

		// Convert relative TextBox position to coordinates relative to client area
		relativePosition -= mClientDimensions.position;

		// Convert relative client position to coordinates relative to Text position
		Point relativeTextPosition;
		relativeTextPosition.x = relativePosition.x - mTextPosition.x;
		relativeTextPosition.y = relativePosition.y - mTextPosition.y;

		return relativeTextPosition;
	}

	void TextArea::_initialize(WidgetDesc* d)
	{
		TextAreaDesc* td = dynamic_cast<TextAreaDesc*>(d);

		// Create Timers before calling ContainerWidget::_initialize.  Some functions like setEnabled affect timers.
		TimerDesc timerDesc;
		timerDesc.repeat = true;
		timerDesc.timePeriod = td->textarea_cursorBlinkTime;
		mBlinkTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mBlinkTimer->setCallback(&TextArea::blinkTimerCallback,this);

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<TextAreaDesc*>(mWidgetDesc);

		mTextCursor->setHeight(Text::getFontHeight(td->textarea_defaultFontName));
		mTextCursor->setSkinType(td->textarea_textCursorDefaultSkinTypeName);

		mDesc->widget_consumeKeyboardEvents = true;
		mCursorIndex = -1;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		setDefaultFont(td->textarea_defaultFontName);
		setDefaultColor(td->textarea_defaultColor);
		mDesc->textarea_maxCharacters = td->textarea_maxCharacters;

		mDesc->textarea_cursorBlinkTime = td->textarea_cursorBlinkTime;
		mDesc->textarea_readOnly = td->textarea_readOnly;
		mDesc->textarea_horizontalPadding = td->textarea_horizontalPadding;

		setSkinType(d->widget_skinTypeName);

		mDesc->textDesc.allottedSize.width = td->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);

		setHorizontalAlignment(mDesc->textDesc.horizontalTextAlignment);
		// Make sure the text cursor is not visible, as setting the alignment will reposition the text cursor and make it visible.
		mTextCursor->setVisible(false);
	}

	void TextArea::_setScrollY(float percentage)
	{
		if(!mDesc->containerwidget_supportScrollBars)
			return;

		// Cap percentage within bounds: [0.0,1.0]
		if(percentage <= 0.0)
			percentage = 0.0;
		else if(percentage > 1.0)
			percentage = 1.0;
		else // Caps percentage to thousandths place
			percentage = Ogre::Math::Floor(Ogre::Math::Ceil(percentage * 1000.0)) / 1000.0;

		// calculate the total distance the View can be offset, ie how far will the view offset be at 100%? (Answer: Virtual height minus actual height)
		float maxViewDisplacement = (mVirtualSize.height - mClientDimensions.size.height);

		// Offset vertical view according to percentage:
		//   At 0.0, vertical view is not offset at all.
		//   At 1.0, vertical view is offset to maximum.
		mDesc->containerwidget_yScrollOffset = static_cast<unsigned int>(percentage * maxViewDisplacement);
		// In the TextArea widget we're offsetting the Text position to achieve the effect of offsetting the view.
		mTextPosition.y = -(percentage * maxViewDisplacement);
		mTextPosition.roundUp();

		// Make sure TextCursor scrolls with the text
		if(mText != NULL)
		{
			// Position Cursor
			mCursorPosition = mTextPosition + mText->getPositionAtCharacterIndex(mCursorIndex);

			Point p = getScreenPosition();
			p.translate(mClientDimensions.position);
			p.translate(mCursorPosition);
			mTextCursor->setPosition(p);
		}

		for(std::list<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->setScrollY(static_cast<unsigned int>(mDesc->containerwidget_yScrollOffset));
	}

	void TextArea::addCharacter(Ogre::UTFString::code_point cp)
	{
		mText->addCharacter(OGRE_NEW_T(Character,Ogre::MEMCATEGORY_GENERAL)(cp,mCurrentFont,mDesc->textarea_defaultColor),mCursorIndex);

		updateVirtualDimensions();
		
		setCursorIndex(mCursorIndex+1);
	}

	void TextArea::addText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mText->addText(s,fp,cv);

		updateVirtualDimensions();

		scrollToBottom();

		redraw();
	}

	void TextArea::addText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		addText(s,Text::getFont(fontName),cv);
	}

	void TextArea::addText(Ogre::UTFString s)
	{
		addText(s,mDesc->textarea_defaultFontName,mDesc->textarea_defaultColor);
	}

	void TextArea::addText(std::vector<TextSegment> segments)
	{
		mText->addText(segments);

		updateVirtualDimensions();

		scrollToBottom();

		redraw();
	}

	void TextArea::addTextLine(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mText->addTextLine(s,fp,cv);

		updateVirtualDimensions();

		scrollToBottom();

		redraw();
	}

	void TextArea::addTextLine(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		addTextLine(s,Text::getFont(fontName),cv);
	}

	void TextArea::addTextLine(Ogre::UTFString s)
	{
		addTextLine(s,mDesc->textarea_defaultFontName,mDesc->textarea_defaultColor);
	}

	void TextArea::addTextLine(std::vector<TextSegment> segments)
	{
		mText->addTextLine(segments);

		updateVirtualDimensions();

		scrollToBottom();

		redraw();
	}

	void TextArea::blinkTimerCallback()
	{
		mTextCursor->setVisible(!mTextCursor->getVisible());
	}

	void TextArea::clearText()
	{
		mText->clearText();

		updateVirtualDimensions();
	}

	Ogre::String TextArea::getClass()
	{
		return "TextArea";
	}

	Point TextArea::getCursorIndexPosition(int index)
	{
		Point charPos = mText->getCharacterPosition(index);
		charPos += mTextPosition;

		return charPos;
	}

	ColourValue TextArea::getDefaultColor()
	{
		return mDesc->textarea_defaultColor;
	}

	Ogre::String TextArea::getDefaultFont()
	{
		return mDesc->textarea_defaultFontName;
	}

	HorizontalTextAlignment TextArea::getHorizontalAlignment()
	{
		return mText->getHorizontalTextAlignment();
	}

	float TextArea::getHorizontalPadding()
	{
		return mDesc->textarea_horizontalPadding;
	}

	bool TextArea::getReadOnly()
	{
		return mDesc->textarea_readOnly;
	}

	Rect TextArea::getScreenRect()
	{
		Rect r = mTextBoxClipRegion;

		if((mDesc->sheet != NULL) && (mDesc->sheet != mWindow))
			r.translate(mWindow->getPosition());

		return r;
	}

	Ogre::UTFString TextArea::getText()
	{
		return mText->getText();
	}

	BrushFilterMode TextArea::getTextBrushFilterMode()
	{
		return mText->getBrushFilterMode();
	}

	Ogre::String TextArea::getTextCursorSkinType()
	{
		return mDesc->textarea_textCursorDefaultSkinTypeName;
	}

	std::vector<TextSegment> TextArea::getTextSegments()
	{
		return mText->getTextSegments();
	}

	float TextArea::getVerticalLineSpacing()
	{
		return mText->getVerticalLineSpacing();
	}

	bool TextArea::isCursorIndexAboveViewableText(int index)
	{
		if(static_cast<int>(index) > mText->getLength())
			index = mText->getLength();
		else if(index < 0)
			index = 0;

		// Get characters position within text
		float charYPos = mText->getCharacterYPosition(index);
		charYPos += mTextPosition.y;

		if(charYPos < 0)
			return true;
		else
			return false;
	}

	bool TextArea::isCursorIndexBelowViewableText(int index)
	{
		if(static_cast<int>(index) > mText->getLength())
			index = mText->getLength();
		else if(index < 0)
			index = 0;

		// Get characters position within text
		float charYPos = mText->getCharacterYPosition(index);
		charYPos += mTextPosition.y;
		//charYPos -= mClientDimensions.position.y;
		// Make sure to include bottom of character when querying if cursor is within view
		Character* c = mText->getCharacter(index);
		charYPos += c->dimensions.size.height;
		// Add a 2 pixel buffer
		charYPos += 2;

		if(charYPos >= mClientDimensions.size.height)
			return true;
		else
			return false;
	}

	bool TextArea::isCursorIndexVisible(int index)
	{
		if(isCursorIndexAboveViewableText(index))
			return false;
		else if(isCursorIndexBelowViewableText(index))
			return false;
		else
			return true;
	}

	void TextArea::moveCursorDown()
	{
		Point globalPos = mTextCursor->getPosition();
		globalPos.x += (mTextCursor->getSize().width / 2.0);
		// Position is at the top left of cursor, we want the position that is below the cursor.
		// Increment y by cursor height, and then increment again by half cursor height. (1.5)
		globalPos.y += (mTextCursor->getSize().height * 1.5);

		setCursorIndex(mText->getCursorIndexAtPosition(_convertScreenToTextCoordinates(globalPos)));
	}

	void TextArea::moveCursorLeft()
	{
		setCursorIndex(mCursorIndex - 1);
	}

	void TextArea::moveCursorRight()
	{
		setCursorIndex(mCursorIndex + 1);
	}

	void TextArea::moveCursorUp()
	{
		Point globalPos = mTextCursor->getPosition();
		globalPos.x += (mTextCursor->getSize().width / 2.0);
		globalPos.y -= (mTextCursor->getSize().height / 2.0);

		setCursorIndex(mText->getCursorIndexAtPosition(_convertScreenToTextCoordinates(globalPos)));
	}

	void TextArea::onCharEntered(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);

		addCharacter(kea.codepoint);
	}

	void TextArea::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		// Draw Background

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect unpaddedClientArea = mClientDimensions;
		unpaddedClientArea.position.x -= mDesc->textarea_horizontalPadding;
		unpaddedClientArea.size.width += (mDesc->textarea_horizontalPadding * 2);

		Rect textArea = Rect(mTexturePosition,unpaddedClientArea.size);
		textArea.translate(unpaddedClientArea.position);

		mTextBoxClipRegion = prevClipRegion.getIntersection(textArea);
		brush->setClipRegion(mTextBoxClipRegion);

		textArea.translate(mTextPosition);

		// Draw text
		mText->draw(textArea.position);

		// Draw Text Overlay SkinElement

		brush->drawSkinElement(mTextBoxClipRegion,mSkinType->getSkinElement(TEXTOVERLAY));

		// Restore clipping

		brush->setClipRegion(prevClipRegion);

		brush->setColor(prevColor);
	}

	void TextArea::onKeyDown(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);

		switch(kea.scancode)
		{
		case KC_BACK:
			removeCharacter(mCursorIndex - 1);
			break;
		case KC_DELETE:
			removeCharacter(mCursorIndex);
			break;
		case KC_DOWN:
			moveCursorDown();
			break;
		case KC_END:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(mText->getLength());
			else
				setCursorIndex(mText->getIndexOfTextLineEnd(mCursorIndex));
			break;
		case KC_LEFT:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(mText->getIndexOfPreviousWord(mCursorIndex));
			else
				setCursorIndex(mCursorIndex - 1);
			break;
		case KC_HOME:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(0);
			else
				setCursorIndex(mText->getIndexOfTextLineBegin(mCursorIndex));
			break;
		case KC_RETURN:
			addCharacter(Text::UNICODE_CR);
			break;
		case KC_RIGHT:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(mText->getIndexOfNextWord(mCursorIndex));
			else
				setCursorIndex(mCursorIndex + 1);
			break;
		case KC_UP:
			moveCursorUp();
			break;
		case KC_LSHIFT:
		case KC_RSHIFT:
		case KC_LCONTROL:
		case KC_RCONTROL:
			break;
		default:
			break;
		}
	}

	void TextArea::onKeyboardInputGain(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		mTextCursor->setVisible(true);
		mBlinkTimer->start();
	}

	void TextArea::onKeyboardInputLose(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		mBlinkTimer->stop();
		mTextCursor->setVisible(false);
	}

	void TextArea::onMouseButtonDown(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mea.button == MB_Left)
		{
			mText->clearHighlights();
			setCursorIndex(mText->getCursorIndexAtPosition(_convertScreenToTextCoordinates(mea.position)));
		}
	}

	void TextArea::onTripleClick(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mea.button == MB_Left)
		{
			mText->highlight();
			redraw();
		}
	}

	void TextArea::onVisibleChanged(const EventArgs& args)
	{
		if(!mWidgetDesc->widget_visible)
		{
			mBlinkTimer->stop();
			mTextCursor->setVisible(false);
		}
		else
		{
			if((mWidgetDesc->sheet != NULL) && (mWidgetDesc->sheet->getKeyboardListener() == this))
			{
				mBlinkTimer->start();
				mTextCursor->setVisible(true);
			}
		}
	}

	void TextArea::onWindowDrawn(const EventArgs& args)
	{
		mTextCursor->onDraw();
	}

	void TextArea::removeCharacter(int index)
	{
		if(index < 0)
			return;

		if(index >= mText->getLength())
			return;

		mText->removeCharacter(index);

		updateVirtualDimensions();

		// Update index
		setCursorIndex(index);
	}

	void TextArea::scrollToBottom()
	{
		setCursorIndex(mText->getLength());
	}

	void TextArea::scrollToTop()
	{
		setCursorIndex(0);
	}

	void TextArea::setCursorIndex(int index)
	{
		if(static_cast<int>(index) > mText->getLength())
			index = mText->getLength();
		else if(index < 0)
			index = 0;

		// Reset cursor blinking every time we move the cursor
		if(!mDesc->textarea_readOnly && (mDesc->sheet->getKeyboardListener() == this))
		{
			mBlinkTimer->reset();
			mTextCursor->setVisible(true);
		}

		// Update Cursor Height to match the font of the character to the left of cursor
		if(index == 0)
			mTextCursor->setHeight(Text::getFontHeight(mDesc->textarea_defaultFontName));
		else
			mTextCursor->setHeight(Text::getFontHeight(mText->getCharacter(static_cast<unsigned int>(index - 1))->fontPtr));

		// Update CursorIndex
		mCursorIndex = index;

		// See if Scrolling of Text needs to occur
		if(isCursorIndexAboveViewableText(mCursorIndex))
		{
			if(mVScrollBar != NULL)
				mVScrollBar->setPercentage(mText->getCharacterYPosition(mCursorIndex) / mVirtualSize.height);
		}
		else if(isCursorIndexBelowViewableText(mCursorIndex))
		{
			if(mVScrollBar != NULL)
			{
				TextLine* tl = mText->getTextLineFromIndex(mCursorIndex);
				// I throw in a buffer of 2 pixels, so floating point rounding won't be so bad
				mVScrollBar->setPercentage((mText->getCharacterYPosition(mCursorIndex) + tl->getHeight()) / mVirtualSize.height);
			}
		}

		// Position Cursor
		mCursorPosition = mTextPosition + mText->getPositionAtCharacterIndex(mCursorIndex);

		Point p = getScreenPosition();
		p.translate(mClientDimensions.position);
		p.translate(mCursorPosition);
		mTextCursor->setPosition(p);
		redraw();
	}

	void TextArea::setDefaultColor(const ColourValue& cv)
	{
		mDesc->textarea_defaultColor = cv;
	}

	void TextArea::setDefaultFont(const Ogre::String& fontName)
	{
		mDesc->textarea_defaultFontName = fontName;
		mCurrentFont = Text::getFont(fontName);
	}

	void TextArea::setFont(const Ogre::String& fontName)
	{
		mDesc->textarea_defaultFontName = fontName;

		mText->setFont(fontName);

		redraw();
	}

	void TextArea::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		redraw();
	}

	void TextArea::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		redraw();
	}

	void TextArea::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		redraw();
	}

	void TextArea::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		redraw();
	}

	void TextArea::setHorizontalAlignment(HorizontalTextAlignment a)
	{
		mText->setHorizontalTextAlignment(a);

		redraw();
	}

	void TextArea::setHorizontalPadding(float padding)
	{
		mDesc->textarea_horizontalPadding = padding;

		updateClientDimensions();

		redraw();
	}

	void TextArea::setMaxCharacters(unsigned int max)
	{
		if(max == mDesc->textarea_maxCharacters)
			return;
		
		if(max < mDesc->textarea_maxCharacters)
		{
			throw Exception(Exception::ERR_TEXT,"Cannot set max Characters when text exceeds max! (Data loss)","TextBox::setMaxCharacters");
		}

		mDesc->textarea_maxCharacters = max;
	}

	void TextArea::setParent(Widget* parent)
	{
		if(mParentWidget != NULL)
		{
			// Remove event handler from previous window
			if(mParentWidget->getWindow() != NULL)
				mParentWidget->getWindow()->removeWindowEventHandler(WINDOW_EVENT_DRAWN,this);
		}

		mParentWidget = parent;

		if(mParentWidget != NULL)
		{
			mWindow = mParentWidget->getWindow();
			mWidgetDesc->guiManager = mParentWidget->getGUIManager();
			mWidgetDesc->sheet = mParentWidget->getSheet();

			// Check if widget should be centered in Parent's client area.
			setHorizontalAnchor(mWidgetDesc->widget_horizontalAnchor);
			setVerticalAnchor(mWidgetDesc->widget_verticalAnchor);

			// Add event handler to new window
			if(mWindow != NULL)
				mWindow->addWindowEventHandler(WINDOW_EVENT_DRAWN,&TextArea::onWindowDrawn,this);
		}

		// Update screen rectangle

		updateTexturePosition();

		// Update components window reference via setParent
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			(*it).second->setParent(this);
		}

		// Update children's window reference via setParent
		for(std::list<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setParent(this);
		}
	}

	void TextArea::setReadOnly(bool readOnly)
	{
		mDesc->textarea_readOnly = readOnly;

		if(mDesc->textarea_readOnly)
		{
			mTextCursor->setVisible(false);
			mBlinkTimer->stop();
		}
	}

	void TextArea::setTextCursorSkinType(const Ogre::String& skinTypeName)
	{
		mDesc->textarea_textCursorDefaultSkinTypeName = skinTypeName;

		mTextCursor->setSkinType(skinTypeName);
	}

	void TextArea::setText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		updateVirtualDimensions();

		scrollToBottom();

		redraw();
	}

	void TextArea::setText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		setText(s,Text::getFont(fontName),cv);
	}

	void TextArea::setText(Ogre::UTFString s)
	{
		setText(s,mDesc->textarea_defaultFontName,mDesc->textarea_defaultColor);
	}

	void TextArea::setText(std::vector<TextSegment> segments)
	{
		mText->setText(segments);

		updateVirtualDimensions();

		scrollToBottom();

		redraw();
	}

	void TextArea::setTextBrushFilterMode(BrushFilterMode m)
	{
		mText->setBrushFilterMode(m);
	}

	void TextArea::setTextColor(const ColourValue& cv)
	{
		mDesc->textarea_defaultColor = cv;

		mText->setColor(cv);

		redraw();
	}

	void TextArea::setTextColor(const ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void TextArea::setTextColor(const ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void TextArea::setTextColor(const ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void TextArea::setTextColor(const ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void TextArea::setVerticalLineSpacing(float distance)
	{
		if(distance == mText->getVerticalLineSpacing())
			return;

		mText->setVerticalLineSpacing(distance);

		redraw();
	}

	void TextArea::updateClientDimensions()
	{
		ContainerWidget::updateClientDimensions();

		// Shrink client area horizontally (on both left and right side) according to padding amount
		if(mDesc != NULL)
		{
			mClientDimensions.position.x += mDesc->textarea_horizontalPadding;
			mClientDimensions.size.width -= (mDesc->textarea_horizontalPadding * 2);
		}

		if(mText != NULL)
			mText->setAllottedWidth(mClientDimensions.size.width);

		redraw();
	}

	void TextArea::updateTexturePosition()
	{
		Widget::updateTexturePosition();

		if(mDesc != NULL)
		{
			Point p = getScreenPosition();
			p.translate(Point(0,0));
			p.translate(mCursorPosition);
			mTextCursor->setPosition(p);
		}
	}

	void TextArea::updateVirtualDimensions()
	{
		if(mDesc == NULL)
			return;

		if(!mDesc->containerwidget_supportScrollBars)
			return;

		mVirtualSize = mClientDimensions.size;

		if(mText != NULL)
		{
			float textHeight = mText->getTextHeight();

			if(textHeight > mVirtualSize.height)
				mVirtualSize.height = textHeight;

			mAmountToScrollOnWheel = (mText->getAverageTextLineHeight() / (mVirtualSize.height - mClientDimensions.size.height));
		}
		else
			mAmountToScrollOnWheel = 0;

		// Set Slider width/height.
		mHScrollBar->setSliderWidth((mClientDimensions.size.width / mVirtualSize.width) * mHScrollBar->getSliderBounds().size.width);
		mVScrollBar->setSliderHeight((mClientDimensions.size.height / mVirtualSize.height) * mVScrollBar->getSliderBounds().size.height);

		// Now that sliders have changed in size, adjust scroll according to slider positions
		_setScrollX(mHScrollBar->getPercentage());
		_setScrollY(mVScrollBar->getPercentage());

		_determineScrollBarVisibility();
	}
}
