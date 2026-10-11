#include "QuickGUICharacter.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	Character::Character(Ogre::UTFString::code_point cp, Ogre::FontPtr fp, ColourValue cv) :
		codePoint(cp),
		colorValue(cv.r,cv.g,cv.b,cv.a),
		mHighlighted(false),
		mAdvance(0),
		mBearing(0)
	{
		setFont(fp);
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
		return Text::isWhiteSpace(codePoint);
	}

	void Character::setBaseline(float baseline)
	{
		dimensions.position.x = baseline + mBearing;
	}

	void Character::setFont(Ogre::FontPtr fp)
	{
		float baseline = getBaseline();

		fontPtr = fp;
		texturePtr = Text::getFontTexture(fp);
		mAdvance = 0;
		mBearing = 0;

		if(Text::isNewLine(codePoint))
		{
			dimensions.size.height = Text::getGlyphHeight(fp,'0');
			dimensions.size.width = 0;
		}
		else if(Text::isSpace(codePoint) || Text::isTab(codePoint))
		{
			float height = Text::getGlyphHeight(fp,'0');
			mAdvance = Text::getGlyphAdvance(fp,Text::UNICODE_ZERO);
			if(mAdvance <= 0)
				mAdvance = height;
			if(Text::isTab(codePoint))
				mAdvance *= 4;
			dimensions.size = Size(mAdvance,height);
		}
		else
		{
			// Get the glyph's UV Coords
			uvCoords = Text::getGlyphUVCoords(fp,codePoint);

			// Use UV Coords to Determine character dimensions
			dimensions.size = Size(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));

			mAdvance = Text::getGlyphAdvance(fp,codePoint);
			mBearing = Text::getGlyphBearing(fp,codePoint);
			if(mAdvance <= 0)
				mAdvance = dimensions.size.width;
		}

		dimensions.size.roundUp();
		setBaseline(baseline);
	}

	void Character::setHighlighted(bool highlighted)
	{
		mHighlighted = highlighted;
	}
}
