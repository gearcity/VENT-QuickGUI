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

#include "QuickGUITextUser.h"
#include "QuickGUIWidget.h"
#include "QuickGUIRoot.h"

namespace QuickGUI
{
	TextUserDesc::TextUserDesc()
	{
		resetToDefault();
	}

	void TextUserDesc::resetToDefault()
	{
		textDesc.resetToDefault();
		text_defaultColor = Root::getSingleton().getDefaultColor();
		text_defaultFontName = Root::getSingleton().getDefaultFontName();
	}

	void TextUserDesc::serialize(SerialBase* b)
	{
		b->IO("DefaultColor",&text_defaultColor,ColourValue::White);
		b->IO("DefaultFontName",&text_defaultFontName,Root::getSingleton().getDefaultFontName());

		textDesc.serialize(b);
	}

	TextUser::TextUser() :
		mOwner(NULL),
		mText(NULL)
	{
	}

	TextUser::~TextUser()
	{
		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);
	}

	void TextUser::_initialize(Widget* owner, WidgetDesc* d)
	{
		mOwner = owner;
		mDesc = dynamic_cast<TextUserDesc*>(d);

		if(mDesc == NULL)
			throw Exception(Exception::ERR_TEXT,"Supplied WidgetDesc does not inherit from TextUserDesc","TextUser::_initialize");

		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);
	}

	void TextUser::addText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mText->addText(s,fp,cv);

		Redraw();
	}

	void TextUser::addText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		addText(s,Text::getFont(fontName),cv);
	}

	void TextUser::addText(Ogre::UTFString s)
	{
		addText(s,mDesc->text_defaultFontName,mDesc->text_defaultColor);
	}

	void TextUser::addText(std::vector<TextSegment> segments)
	{
		mText->addText(segments);

		Redraw();
	}

	void TextUser::clearText()
	{
		mText->clearText();

		Redraw();
	}

	ColourValue TextUser::getDefaultColor()
	{
		return mDesc->text_defaultColor;
	}

	Ogre::String TextUser::getDefaultFont()
	{
		return mDesc->text_defaultFontName;
	}

	HorizontalTextAlignment TextUser::getHorizontalTextAlignment()
	{
		return mText->getHorizontalTextAlignment();
	}

	Ogre::UTFString TextUser::getText()
	{
		return mText->getText();
	}

	float TextUser::getTextAllottedHeight()
	{
		return mText->getAllottedHeight();
	}

	Size TextUser::getTextAllottedSize()
	{
		return mText->getAllottedSize();
	}

	float TextUser::getTextAllottedWidth()
	{
		return mText->getAllottedWidth();
	}

	BrushFilterMode TextUser::getTextBrushFilterMode()
	{
		return mText->getBrushFilterMode();
	}

	float TextUser::getTextHeight()
	{
		return mText->getTextHeight();
	}

	std::vector<TextSegment> TextUser::getTextSegments()
	{
		return mText->getTextSegments();
	}

	float TextUser::getTextWidth()
	{
		return mText->getTextWidth();
	}

	float TextUser::getVerticalLineSpacing()
	{
		return mText->getVerticalLineSpacing();
	}

	VerticalTextAlignment TextUser::getVerticalTextAlignment()
	{
		return mText->getVerticalTextAlignment();
	}

	void TextUser::onTextChanged()
	{
		// Do nothing by default, this function is meant to be overridden.
	}

	void TextUser::Redraw()
	{
		if(mOwner != NULL)
			mOwner->redraw();
	}

	void TextUser::setDefaultColor(const ColourValue& cv)
	{
		mDesc->text_defaultColor = cv;
	}

	void TextUser::setDefaultFont(const Ogre::String& fontName)
	{
		mDesc->text_defaultFontName = fontName;
	}

	void TextUser::setFont(const Ogre::String& fontName)
	{
		mDesc->text_defaultFontName = fontName;

		mText->setFont(fontName);

		onTextChanged();

		Redraw();
	}

	void TextUser::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		onTextChanged();

		Redraw();
	}

	void TextUser::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		onTextChanged();

		Redraw();
	}

	void TextUser::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		onTextChanged();

		Redraw();
	}

	void TextUser::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		onTextChanged();

		Redraw();
	}

	void TextUser::setHorizontalTextAlignment(HorizontalTextAlignment a)
	{
		mText->setHorizontalTextAlignment(a);

		onTextChanged();

		Redraw();
	}

	void TextUser::setText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		onTextChanged();

		Redraw();
	}

	void TextUser::setText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		setText(s,Text::getFont(fontName),cv);
	}

	void TextUser::setText(Ogre::UTFString s, const ColourValue& cv)
	{
		setText(s,mDesc->text_defaultFontName,cv);
	}

	void TextUser::setText(Ogre::UTFString s)
	{
		setText(s,mDesc->text_defaultFontName,mDesc->text_defaultColor);
	}

	void TextUser::setText(std::vector<TextSegment> segments)
	{
		mText->setText(segments);

		onTextChanged();

		Redraw();
	}

	void TextUser::setTextAllottedHeight(float allottedHeight)
	{
		mText->setAllottedHeight(allottedHeight);

		onTextChanged();

		Redraw();
	}

	void TextUser::setTextAllottedSize(Size allottedSize)
	{
		mText->setAllottedSize(allottedSize);

		onTextChanged();

		Redraw();
	}

	void TextUser::setTextAllottedWidth(float allottedWidth)
	{
		mText->setAllottedWidth(allottedWidth);

		onTextChanged();

		Redraw();
	}

	void TextUser::setTextBrushFilterMode(BrushFilterMode m)
	{
		mText->setBrushFilterMode(m);
	}

	void TextUser::setTextColor(const ColourValue& cv)
	{
		mDesc->text_defaultColor = cv;

		mText->setColor(cv);

		onTextChanged();

		Redraw();
	}

	void TextUser::setTextColor(const ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		onTextChanged();

		Redraw();
	}

	void TextUser::setTextColor(const ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		onTextChanged();

		Redraw();
	}

	void TextUser::setTextColor(const ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		onTextChanged();

		Redraw();
	}

	void TextUser::setTextColor(const ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		onTextChanged();

		Redraw();
	}

	void TextUser::setVerticalLineSpacing(float distance)
	{
		if(distance == mText->getVerticalLineSpacing())
			return;

		mText->setVerticalLineSpacing(distance);

		onTextChanged();

		Redraw();
	}

	void TextUser::setVerticalTextAlignment(VerticalTextAlignment a)
	{
		mText->setVerticalTextAlignment(a);

		onTextChanged();

		Redraw();
	}
}
