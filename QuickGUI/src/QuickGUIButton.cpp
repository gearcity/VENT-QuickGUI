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

#include "QuickGUIButton.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIDescManager.h"

#include "OgreFont.h"

namespace QuickGUI
{
	const Ogre::String Button::DEFAULT = "default";
	const Ogre::String Button::OVER = "over";
	const Ogre::String Button::DOWN = "down";

	void Button::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Button");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(DOWN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Button",d);
	}

	ButtonDesc::ButtonDesc() :
		LabelDesc()
	{
		resetToDefault();
	}

	void ButtonDesc::resetToDefault()
	{
		LabelDesc::resetToDefault();

		widget_dimensions.size = Size(75,25);

		button_imageName = "";
		button_tileImage = false;

		textDesc.horizontalTextAlignment = TEXT_ALIGNMENT_HORIZONTAL_CENTER;
	}

	void ButtonDesc::serialize(SerialBase* b)
	{
		LabelDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		ButtonDesc* defaultValues = DescManager::getSingleton().createDesc<ButtonDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("ImageName", &button_imageName, defaultValues->button_imageName);
		b->IO("TileImage", &button_tileImage, defaultValues->button_tileImage);

		DescManager::getSingleton().destroyDesc(defaultValues);
	}

	Button::Button(const Ogre::String& name) :
		Label(name)
	{
	}

	Button::~Button()
	{
	}

	void Button::_initialize(WidgetDesc* d)
	{
		Label::_initialize(d);

		ButtonDesc* bd = dynamic_cast<ButtonDesc*>(d);

		mDesc = dynamic_cast<ButtonDesc*>(mWidgetDesc);

		setSkinType(d->widget_skinTypeName);
		setImage(bd->button_imageName);
		setTileImage(bd->button_tileImage);

		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&Button::onMouseEnter,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&Button::onMouseLeave,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&Button::onMouseLeftButtonDown,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&Button::onMouseLeftButtonUp,this);
	}

	Ogre::String Button::getClass()
	{
		return "Button";
	}

	Ogre::String Button::getImageName()
	{
		return mDesc->button_imageName;
	}

	bool Button::getTileImage()
	{
		return mDesc->button_tileImage;
	}

	void Button::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		// Draw background
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Draw image
		if(mDesc->button_imageName != "")
		{
			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			brush->setTexture(mDesc->button_imageName);
			if(mDesc->button_tileImage)
				brush->drawTiledRectangle(clipRegion,UVRect(0,0,1,1));
			else
				brush->drawRectangle(clipRegion,UVRect(0,0,1,1));

			brush->setClipRegion(prevClipRegion);
		}

		// Clip to client dimensions
		Rect clipRegion(mClientDimensions);
		clipRegion.translate(mTexturePosition);

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);		

		// Draw text
		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void Button::onMouseEnter(const EventArgs& args)
	{
		mSkinElement = mSkinType->getSkinElement(OVER);

		redraw();
	}

	void Button::onMouseLeave(const EventArgs& args)
	{
		mSkinElement = mSkinType->getSkinElement(DEFAULT);

		redraw();
	}

	void Button::onMouseLeftButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			mSkinElement = mSkinType->getSkinElement(DOWN);
			redraw();
		}
	}

	void Button::onMouseLeftButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			mSkinElement = mSkinType->getSkinElement(OVER);
			redraw();
		}
	}

	void Button::setImage(const Ogre::String& name)
	{
		mDesc->button_imageName = name;

		if(mDesc->button_imageName != "")
		{
			// If texture not loaded, load it!
			if(!Ogre::TextureManager::getSingleton().resourceExists(mDesc->button_imageName))
			{
				Ogre::Image i;
				i.load(mDesc->button_imageName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
		}

		redraw();
	}

	void Button::setText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mText->setAllottedWidth(mClientDimensions.size.width);

		TextUser::setText(s,fp,cv);
	}

	void Button::setText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		mText->setAllottedWidth(mClientDimensions.size.width);

		TextUser::setText(s,fontName,cv);
	}

	void Button::setText(Ogre::UTFString s, const ColourValue& cv)
	{
		mText->setAllottedWidth(mClientDimensions.size.width);

		TextUser::setText(s,cv);
	}

	void Button::setText(Ogre::UTFString s)
	{
		mText->setAllottedWidth(mClientDimensions.size.width);

		TextUser::setText(s);
	}

	void Button::setText(std::vector<TextSegment> segments)
	{
		mText->setAllottedWidth(mClientDimensions.size.width);

		TextUser::setText(segments);
	}

	void Button::setTileImage(bool tile)
	{
		mDesc->button_tileImage = tile;

		redraw();
	}

	void Button::updateClientDimensions()
	{
		Widget::updateClientDimensions();

		// Many buttons have no text.  In this case, we don't need to adjust
		// the Text's allotted width
		if((mText != NULL) && (!mText->empty()))
			mText->setAllottedSize(mClientDimensions.size);

		redraw();
	}

	void Button::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
	}
}
