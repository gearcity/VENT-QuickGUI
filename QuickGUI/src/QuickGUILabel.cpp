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

#include "QuickGUILabel.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"

namespace QuickGUI
{
	const Ogre::String Label::BACKGROUND = "background";

	void Label::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Label");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Label",d);
	}

	LabelDesc::LabelDesc() :
		WidgetDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void LabelDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		TextUserDesc::resetToDefault();

		widget_dimensions.size = Size::ZERO;
		widget_minSize = Size::ZERO;
	}

	void LabelDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		TextUserDesc::serialize(b);
	}

	Label::Label(const Ogre::String& name) :
		Widget(name),
		TextUser()
	{
	}

	Label::~Label()
	{
	}

	void Label::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<LabelDesc*>(mWidgetDesc);

		LabelDesc* ld = dynamic_cast<LabelDesc*>(d);

		mDesc->text_defaultColor = ld->text_defaultColor;
		mDesc->text_defaultFontName = ld->text_defaultFontName;

		setSkinType(d->widget_skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = ld->textDesc;

		if(mDesc->widget_dimensions.size.width == 1)
		{
			if(mDesc->textDesc.segments.empty())
				mDesc->widget_dimensions.size.width = 50;
			else
				mDesc->widget_dimensions.size.width = mSkinElement->getBorderThickness(BORDER_LEFT) + mDesc->textDesc.getTextWidth() + mSkinElement->getBorderThickness(BORDER_RIGHT);
		}
		if(mDesc->widget_dimensions.size.height == 1)
		{
			if(mDesc->textDesc.segments.empty())
				mDesc->widget_dimensions.size.height = 20;
			else
				mDesc->widget_dimensions.size.height = mSkinElement->getBorderThickness(BORDER_TOP) + mDesc->textDesc.getTextHeight() + mSkinElement->getBorderThickness(BORDER_BOTTOM);
		}
		// Now that dimensions may have changed, update client dimensions
		updateClientDimensions();

		mDesc->textDesc.allottedSize.height = mDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));
		mDesc->textDesc.allottedSize.width = mDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
		TextUser::_initialize(this,mDesc);
	}

	Ogre::String Label::getClass()
	{
		return "Label";
	}

	void Label::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Clip to client dimensions
		Rect clipRegion(mClientDimensions);
		clipRegion.translate(mTexturePosition);

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);		

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void Label::updateClientDimensions()
	{
		Widget::updateClientDimensions();
		if(mText != NULL)
			mText->setAllottedSize(mClientDimensions.size);

		redraw();
	}
}
