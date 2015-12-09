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

#include "QuickGUITab.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITabControl.h"
#include "QuickGUISkinDefinition.h"

#include "OgreImage.h"
#include "OgreFont.h"

namespace QuickGUI
{
	const Ogre::String Tab::FRONT = "front";
	const Ogre::String Tab::MAIN = "main";

	void Tab::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Tab");
		d->defineSkinElement(FRONT);
		d->defineSkinElement(MAIN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Tab",d);
	}

	TabDesc::TabDesc() :
		LabelDesc()
	{
		resetToDefault();
	}

	void TabDesc::resetToDefault()
	{
		LabelDesc::resetToDefault();

		tab_frontWidth = 0;
		tab_imageName = "";
	}

	void TabDesc::serialize(SerialBase* b)
	{
		// Tabs never get serialized, their data is duplicated and serialized from TabPage widget
	}

	Tab::Tab(const Ogre::String& name) :
		Label(name),
		mDesc(NULL)
	{
	}

	Tab::~Tab()
	{
	}

	void Tab::_adjustTabWidth()
	{
		_deriveImageSize();

		mDesc->widget_dimensions.size.width = mDesc->tab_frontWidth + mSkinElement->getBorderThickness(BORDER_LEFT) + mImageSize.width + mDesc->textDesc.getTextWidth() + mSkinElement->getBorderThickness(BORDER_RIGHT);

		// If the Tab belongs to a TabPage which belongs to a TabControl, signal TabControl to update all TabPages
		if((mParentWidget != NULL) && (mParentWidget->getParentWidget() != NULL))
			dynamic_cast<TabControl*>(mParentWidget->getParentWidget())->updateTabPositions();
	}

	void Tab::_deriveImageSize()
	{
		if((mDesc == NULL) || (mDesc->tab_imageName == ""))
		{
			mImageSize = Size::ZERO;
			return;
		}

		float height = mClientDimensions.size.height;

		Ogre::Image i;
		i.load(mDesc->tab_imageName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		float ratio = height / static_cast<float>(i.getHeight());
		
		mImageSize.height = static_cast<float>(i.getHeight()) * ratio;
		mImageSize.width = static_cast<float>(i.getWidth()) * ratio;

		mImageSize.roundUp();
	}

	void Tab::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<TabDesc*>(mWidgetDesc);

		TabDesc* td = dynamic_cast<TabDesc*>(d);

		mDesc->tab_imageName = td->tab_imageName;

		setSkinType(td->widget_skinTypeName);

		if(td->tab_frontWidth <= 0)
			mDesc->tab_frontWidth = mSkinType->getSkinElement(FRONT)->getWidth();
		else
			mDesc->tab_frontWidth = td->tab_frontWidth;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;
		
		if(mDesc->textDesc.segments.empty())
			mDesc->widget_dimensions.size.height = mSkinElement->getBorderThickness(BORDER_TOP) + 25 + mSkinElement->getBorderThickness(BORDER_BOTTOM);
		else
			mDesc->widget_dimensions.size.height = mSkinElement->getBorderThickness(BORDER_TOP) + mDesc->textDesc.getTextHeight() + mSkinElement->getBorderThickness(BORDER_BOTTOM);

		_deriveImageSize();

		mDesc->widget_dimensions.size.width = mDesc->tab_frontWidth + mSkinElement->getBorderThickness(BORDER_LEFT) + mImageSize.width + mDesc->textDesc.getTextWidth() + mSkinElement->getBorderThickness(BORDER_RIGHT);
		
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);
	}

	BorderSide Tab::getBorderSide(Point p)
	{
		return BORDER_NONE;
	}

	Ogre::String Tab::getClass()
	{
		return "Tab";
	}

	void Tab::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// set clip region to dimensions
		brush->setClipRegion(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).getIntersection(prevClipRegion));

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		}

		Rect tabRect(mTexturePosition,mWidgetDesc->widget_dimensions.size);
		
		// Draw the front part of the tab
		tabRect.size.width = mDesc->tab_frontWidth;
		brush->drawSkinElement(tabRect,st->getSkinElement(FRONT));

		// Draw the main part of the tab
		tabRect.position.x += mDesc->tab_frontWidth;
		tabRect.size.width = mWidgetDesc->widget_dimensions.size.width - mDesc->tab_frontWidth;
		brush->drawSkinElement(tabRect,st->getSkinElement(MAIN));

		// Draw the icon on the main part of the tab
		Rect iconRect(tabRect);
		iconRect.size = mImageSize;
		if(mDesc->tab_imageName != "")
		{
			brush->setTexture(mDesc->tab_imageName);
			brush->drawRectangle(iconRect,UVRect(0,0,1,1));
		}

		// Draw text

		float textHeight = mText->getTextHeight();
		float yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);

		Rect clipRegion;
		clipRegion.size = 
			Size(
				tabRect.size.width - iconRect.size.width - st->getSkinElement(MAIN)->getBorderThickness(BORDER_RIGHT) - st->getSkinElement(MAIN)->getBorderThickness(BORDER_LEFT),
				tabRect.size.height - st->getSkinElement(MAIN)->getBorderThickness(BORDER_BOTTOM) - st->getSkinElement(MAIN)->getBorderThickness(BORDER_TOP));
		clipRegion.position = tabRect.position;
		clipRegion.translate(Point(st->getSkinElement(MAIN)->getBorderThickness(BORDER_LEFT),st->getSkinElement(MAIN)->getBorderThickness(BORDER_TOP)));
		clipRegion.position.x += iconRect.size.width;

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		clipRegion.position.y += yPos;
		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	bool Tab::overBorderSide(Point p)
	{
		return false;
	}

	void Tab::setHeight(float pixelHeight)
	{
		Label::setHeight(pixelHeight);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName)
	{
		Label::setFont(fontName);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName, unsigned int index)
	{
		Label::setFont(fontName,index);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		Label::setFont(fontName,startIndex,endIndex);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		Label::setFont(fontName,c,allOccurrences);

		_adjustTabWidth();
	}

	void Tab::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		Label::setFont(fontName,s,allOccurrences);

		_adjustTabWidth();
	}

	void Tab::setText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		Label::setText(s,fp,cv);

		_adjustTabWidth();
	}

	void Tab::setText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		Label::setText(s,fontName,cv);

		_adjustTabWidth();
	}

	void Tab::setText(Ogre::UTFString s)
	{
		Label::setText(s);

		_adjustTabWidth();
	}

	void Tab::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);
	}

	void Tab::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(MAIN);
	}
}
