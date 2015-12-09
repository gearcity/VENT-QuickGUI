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

#include "QuickGUISkinElement.h"
#include "QuickGUIScriptDefinition.h"

#include "OgreColourValue.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreImage.h"
#include "OgreTextureManager.h"

namespace QuickGUI
{
	SkinElement::SkinElement(const Ogre::String& elementName) :
		mName(elementName),
		mTextureName(""),
		mDirty(true),
		mTileBackground(true),
		mTileBorders(true)
	{
		mBorderThickness[0] = mBorderThickness[1] = mBorderThickness[2] = mBorderThickness[3] = 0;
	}

	void SkinElement::_updateUVsAndDimensions()
	{
		if(!mDirty)
			return;

		if(mTextureName == "")
			return;

		Ogre::TextureManager* tm = Ogre::TextureManager::getSingletonPtr();
		if(!tm->resourceExists(mTextureName))
			tm->load(mTextureName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 0, 1.0f);

		Ogre::TexturePtr tex = tm->getByName(mTextureName);
		float texWidth = tex->getWidth();
		float texHeight = tex->getHeight();

		mBackgroundUV.left = mBorderThickness[BORDER_LEFT] / texWidth;
		mBackgroundUV.right = (texWidth - mBorderThickness[BORDER_RIGHT]) / texWidth;
		mBackgroundUV.top = mBorderThickness[BORDER_TOP] / texHeight;
		mBackgroundUV.bottom = (texHeight - mBorderThickness[BORDER_BOTTOM]) / texHeight;
		
		// Top Left Corner
		mBorderUV[BORDER_TOP_LEFT].left = 0;
		mBorderUV[BORDER_TOP_LEFT].right = mBorderThickness[BORDER_LEFT] / texWidth;
		mBorderUV[BORDER_TOP_LEFT].top = 0;
		mBorderUV[BORDER_TOP_LEFT].bottom = mBorderThickness[BORDER_TOP] / texHeight;
		// Top Side
		mBorderUV[BORDER_TOP].left = mBorderThickness[BORDER_LEFT] / texWidth;
		mBorderUV[BORDER_TOP].right = (texWidth - mBorderThickness[BORDER_RIGHT]) / texWidth;
		mBorderUV[BORDER_TOP].top = 0;
		mBorderUV[BORDER_TOP].bottom = mBorderThickness[BORDER_TOP] / texHeight;
		// Top Right Corner
		mBorderUV[BORDER_TOP_RIGHT].left = (texWidth - mBorderThickness[BORDER_RIGHT]) / texWidth;
		mBorderUV[BORDER_TOP_RIGHT].right = 1.0;
		mBorderUV[BORDER_TOP_RIGHT].top = 0;
		mBorderUV[BORDER_TOP_RIGHT].bottom = mBorderThickness[BORDER_TOP] / texHeight;
		// Right Side
		mBorderUV[BORDER_RIGHT].left = (texWidth - mBorderThickness[BORDER_RIGHT]) / texWidth;
		mBorderUV[BORDER_RIGHT].right = 1.0;
		mBorderUV[BORDER_RIGHT].top = mBorderThickness[BORDER_TOP] / texHeight;
		mBorderUV[BORDER_RIGHT].bottom = (texHeight - mBorderThickness[BORDER_BOTTOM]) / texHeight;
		// Bottom Right Corner
		mBorderUV[BORDER_BOTTOM_RIGHT].left = (texWidth - mBorderThickness[BORDER_RIGHT]) / texWidth;
		mBorderUV[BORDER_BOTTOM_RIGHT].right = 1.0;
		mBorderUV[BORDER_BOTTOM_RIGHT].top = (texHeight - mBorderThickness[BORDER_BOTTOM]) / texHeight;
		mBorderUV[BORDER_BOTTOM_RIGHT].bottom = 1.0;
		// Bottom Side
		mBorderUV[BORDER_BOTTOM].left = mBorderThickness[BORDER_LEFT] / texWidth;
		mBorderUV[BORDER_BOTTOM].right = (texWidth - mBorderThickness[BORDER_RIGHT]) / texWidth;
		mBorderUV[BORDER_BOTTOM].top = (texHeight - mBorderThickness[BORDER_BOTTOM]) / texHeight;
		mBorderUV[BORDER_BOTTOM].bottom = 1.0;
		// Left Bottom Corner
		mBorderUV[BORDER_BOTTOM_LEFT].left = 0;
		mBorderUV[BORDER_BOTTOM_LEFT].right = mBorderThickness[BORDER_LEFT] / texWidth;
		mBorderUV[BORDER_BOTTOM_LEFT].top = (texHeight - mBorderThickness[BORDER_BOTTOM]) / texHeight;
		mBorderUV[BORDER_BOTTOM_LEFT].bottom = 1.0;
		// Left Side
		mBorderUV[BORDER_LEFT].left = 0;
		mBorderUV[BORDER_LEFT].right = mBorderThickness[BORDER_LEFT] / texWidth;
		mBorderUV[BORDER_LEFT].top = mBorderThickness[BORDER_TOP] / texHeight;
		mBorderUV[BORDER_LEFT].bottom = (texHeight - mBorderThickness[BORDER_BOTTOM]) / texHeight;
		
		_updateTexture();

		mDirty = false;
	}

	void SkinElement::_updateTexture()
	{
		if(mTextureName != "")
			mTexture.load(mTextureName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}

	UVRect SkinElement::getBackgroundUVCoords()
	{
		_updateUVsAndDimensions();

		return mBackgroundUV;
	}

	float SkinElement::getBorderThickness(BorderSide s)
	{
		switch(s)
		{
		case BORDER_LEFT:
			return mBorderThickness[BORDER_LEFT];
		case BORDER_RIGHT:
			return mBorderThickness[BORDER_RIGHT];
		case BORDER_TOP:
			return mBorderThickness[BORDER_TOP];
		case BORDER_BOTTOM:
			return mBorderThickness[BORDER_BOTTOM];
		case BORDER_BOTTOM_LEFT:
		case BORDER_BOTTOM_RIGHT:
			return mBorderThickness[BORDER_BOTTOM];
		case BORDER_TOP_LEFT:
		case BORDER_TOP_RIGHT:
			return mBorderThickness[BORDER_TOP];
		default:
			return -1;
		}
	}

	UVRect SkinElement::getBorderUVCoords(BorderSide s)
	{
		if(s == BORDER_NONE)
			return UVRect(0,0,0,0);

		_updateUVsAndDimensions();

		return mBorderUV[s];
	}

	int SkinElement::getHeight()
	{
		_updateUVsAndDimensions();

		return static_cast<int>(mTexture.getHeight());
	}

	Ogre::String SkinElement::getName()
	{
		return mName;
	}

	const Ogre::Image& SkinElement::getTextureImage()
	{
		_updateUVsAndDimensions();

		return mTexture;
	}

	Ogre::String SkinElement::getTextureName()
	{
		return mTextureName;
	}

	bool SkinElement::getTileBackground()
	{
		return mTileBackground;
	}

	bool SkinElement::getTileBorders()
	{
		return mTileBorders;
	}

	int SkinElement::getWidth()
	{
		_updateUVsAndDimensions();

		return static_cast<int>(mTexture.getWidth());
	}

	bool SkinElement::transparentPixel(Point p)
	{
		if(mTextureName == "")
			return false;

		if(p.x < 0 || p.y < 0 || p.x >= mTexture.getWidth() || p.y >= mTexture.getHeight())
			return false;
/*
		Point temp = p;

		// Convert coordintates into percentages, and then to a pixel location on the image.
		p.x = (p.x / drawnSize.width * (mTexture.getWidth() - 1));
		p.y = (p.y / drawnSize.height * (mTexture.getHeight() - 1));

		int x = static_cast<int>(p.x);
		int y = static_cast<int>(p.y);
*/
		return (mTexture.getColourAt(static_cast<int>(p.x),static_cast<int>(p.y),0).a < 0.01);
/*
		ColourValue cv = mTexture.getColourAt(static_cast<int>(p.x),static_cast<int>(p.y),0);
		float alpha = cv.a;

		if(alpha <= 0.01)
			return true;
		else
			return false;

		return (alpha <= 0);
*/	}

	void SkinElement::serialize(SerialBase* b)
	{
		b->begin("SkinElement",mName);

		// Only write if texture name is valid, and only read if ScriptDefinition is not empty
		if((b->isSerialWriter() && (mTextureName != "")) || (b->isSerialReader() && !(b->getCurrentDefinition()->isEmpty())))
		{
			b->IO("Texture",&mTextureName,"");
			b->IO("TileBackground",&mTileBackground,false);
			b->IO("Border_Bottom",&(mBorderThickness[BORDER_BOTTOM]),0.0);
			b->IO("Border_Left",&(mBorderThickness[BORDER_LEFT]),0.0);
			b->IO("Border_Right",&(mBorderThickness[BORDER_RIGHT]),0.0);
			b->IO("Border_Top",&(mBorderThickness[BORDER_TOP]),0.0);
			b->IO("TileBorders",&mTileBorders,false);
		}

		b->end();
	}

	void SkinElement::setBorderThickness(BorderSide s, float thickness)
	{
		switch(s)
		{
		case BORDER_LEFT:
			mBorderThickness[BORDER_LEFT] = thickness;
		case BORDER_RIGHT:
			mBorderThickness[BORDER_RIGHT] = thickness;
		case BORDER_TOP:
			mBorderThickness[BORDER_TOP] = thickness;
		case BORDER_BOTTOM:
			mBorderThickness[BORDER_BOTTOM] = thickness;
		case BORDER_BOTTOM_LEFT:
			mBorderThickness[BORDER_BOTTOM] = thickness;
			mBorderThickness[BORDER_LEFT] = thickness;
			break;
		case BORDER_BOTTOM_RIGHT:
			mBorderThickness[BORDER_BOTTOM] = thickness;
			mBorderThickness[BORDER_RIGHT] = thickness;
			break;
		case BORDER_TOP_LEFT:
			mBorderThickness[BORDER_TOP] = thickness;
			mBorderThickness[BORDER_LEFT] = thickness;
			break;
		case BORDER_TOP_RIGHT:
			mBorderThickness[BORDER_TOP] = thickness;
			mBorderThickness[BORDER_RIGHT] = thickness;
			break;
		default:
			break;
		}

		mDirty = true;
	}

	void SkinElement::setTextureName(const Ogre::String& textureName)
	{
		mTextureName = textureName;

		mDirty = true;
	}

	void SkinElement::setTileBackground(bool tile)
	{
		mTileBackground = tile;
	}

	void SkinElement::setTileBorders(bool tile)
	{
		mTileBorders = tile;
	}
}
