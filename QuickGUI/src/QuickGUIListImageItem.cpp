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

#include "QuickGUIListImageItem.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	void ListImageItem::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ListImageItem");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ListImageItem",d);
	}

	ListImageItemDesc::ListImageItemDesc() :
		ListItemDesc()
	{
		resetToDefault();
	}

	void ListImageItemDesc::resetToDefault()
	{
		ListItemDesc::resetToDefault();

		listimageitem_imageName = "";
		listimageitem_tileImage = true;
	}

	void ListImageItemDesc::serialize(SerialBase* b)
	{
		ListItemDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		ListImageItemDesc* defaultValues = DescManager::getSingleton().createDesc<ListImageItemDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("ImageName", &listimageitem_imageName, defaultValues->listimageitem_imageName);
		b->IO("TileImage", &listimageitem_tileImage, defaultValues->listimageitem_tileImage);

		DescManager::getSingleton().destroyDesc(defaultValues);
	}

	ListImageItem::ListImageItem(const Ogre::String& name) :
		ListItem(name)
	{
	}

	ListImageItem::~ListImageItem()
	{
	}

	void ListImageItem::_initialize(WidgetDesc* d)
	{
		ListItem::_initialize(d);

		mDesc = dynamic_cast<ListImageItemDesc*>(mWidgetDesc);

		ListImageItemDesc* ltid = dynamic_cast<ListImageItemDesc*>(d);

		setSkinType(ltid->widget_skinTypeName);
		setImage(ltid->listimageitem_imageName);
		setTileImage(ltid->listimageitem_tileImage);
	}

	Ogre::String ListImageItem::getClass()
	{
		return "ListImageItem";
	}

	Ogre::String ListImageItem::getImageName()
	{
		return mDesc->listimageitem_imageName;
	}

	bool ListImageItem::getTileImage()
	{
		return mDesc->listimageitem_tileImage;
	}

	void ListImageItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		if(mDesc->listimageitem_imageName != "")
		{
			ColourValue prevColor = brush->getColour();
			Rect prevClipRegion = brush->getClipRegion();

			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			brush->setTexture(mDesc->listimageitem_imageName);
			if(mDesc->listimageitem_tileImage)
				brush->drawTiledRectangle(clipRegion,UVRect(0,0,1,1));
			else
				brush->drawRectangle(clipRegion,UVRect(0,0,1,1));

			brush->setClipRegion(prevClipRegion);
			Brush::getSingleton().setColor(prevColor);
		}
	}

	void ListImageItem::setImage(const Ogre::String& name)
	{
		mDesc->listimageitem_imageName = name;

		if(mDesc->listimageitem_imageName != "")
		{
			// If texture not loaded, load it!
			if(!Ogre::TextureManager::getSingleton().resourceExists(mDesc->listimageitem_imageName))
			{
				Ogre::Image i;
				i.load(mDesc->listimageitem_imageName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
		}

		redraw();
	}

	void ListImageItem::setTileImage(bool tile)
	{
		mDesc->listimageitem_tileImage = tile;

		redraw();
	}
}
