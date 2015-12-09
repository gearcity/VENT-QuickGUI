#include "QuickGUICharacter.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	Character::Character(Ogre::UTFString::code_point cp, Ogre::FontPtr fp, ColourValue cv) :
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

	void Character::setFont(Ogre::FontPtr fp)
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
