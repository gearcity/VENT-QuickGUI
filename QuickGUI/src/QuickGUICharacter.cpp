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

#include "QuickGUICharacter.h"
#include "QuickGUIText.h"

#include "OgreFont.h"
#include "OgreTexture.h"

namespace QuickGUI
{
	Character::Character(Ogre::UTFString::code_point cp, Ogre::Font* fp, ColourValue cv) :
		codePoint(cp), 
		fontPtr(fp),
		colorValue(cv.r,cv.g,cv.b,cv.a),
		mHighlighted(false)
	{
		texturePtr = Text::getFontTexture(fp);

		if(Text::isNewLine(cp))
		{
			mWhiteSpace = true;
			dimensions.size.height = Text::getGlyphHeight(fp,'0');
			dimensions.size.width = 0;
		}
		else if(Text::isSpace(cp))
		{
			mWhiteSpace = true;
			dimensions.size = Text::getGlyphSize(fp,'0');
		}
		else if(Text::isTab(cp))
		{
			mWhiteSpace = true;
			dimensions.size = Text::getGlyphSize(fp,'0');
			dimensions.size.width *= 4;
		}
		else
		{
			mWhiteSpace = false;

			// Get the glyph's UV Coords
			UVRect uvCoords = Text::getGlyphUVCoords(fp,cp);
			this->uvCoords = uvCoords;

			// Use UV Coords to Determine character dimensions
			dimensions.size = Size(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));
		}

		dimensions.size.roundUp();
	}

	bool Character::getHighlighted()
	{
		return mHighlighted;
	}

	ColourValue Character::getHighlightColor()
	{
		return colorValue;
	}

	ColourValue Character::getHighlightedTextColor()
	{
		return ColourValue(1.0 - colorValue.r,1.0 - colorValue.g,1.0 - colorValue.b,1.0);
	}

	bool Character::isWhiteSpace()
	{
		return mWhiteSpace;
	}

	void Character::setFont(Ogre::Font* fp)
	{
		fontPtr = fp;
		texturePtr = Text::getFontTexture(fp);

		if(Text::isSpace(codePoint))
		{
			dimensions.size = Text::getGlyphSize(fp,'0');
		}
		else if(Text::isTab(codePoint))
		{
			dimensions.size = Text::getGlyphSize(fp,'0');
			dimensions.size.width *= 4;
		}
		else
		{
			// Get the glyph's UV Coords
			UVRect uvCoords = Text::getGlyphUVCoords(fp,codePoint);
			this->uvCoords = uvCoords;

			// Use UV Coords to Determine character dimensions
			dimensions.size = Size(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));
		}
	}

	void Character::setHighlighted(bool highlighted)
	{
		mHighlighted = highlighted;
	}
}
