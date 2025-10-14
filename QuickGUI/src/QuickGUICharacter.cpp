#include "QuickGUICharacter.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
        Character::Character(Ogre::UTFString::code_point cp, Ogre::FontPtr fp, ColourValue cv) :
                codePoint(cp),
                fontPtr(fp),
                colorValue(cv.r,cv.g,cv.b,cv.a),
                mHighlighted(false),
                mWhiteSpace(false),
                mAdvance(0),
                mBearing(0),
                mBaseline(0)
        {
                updateMetrics();
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
                updateMetrics();
                setBaseline(mBaseline);
        }

        void Character::setHighlighted(bool highlighted)
        {
                mHighlighted = highlighted;
        }

        void Character::setBaseline(float baseline)
        {
                mBaseline = baseline;
                dimensions.position.x = mBaseline + mBearing;
        }

        void Character::updateMetrics()
        {
                texturePtr = Text::getFontTexture(fontPtr);

                mAdvance = 0;
                mBearing = 0;
                mWhiteSpace = false;

                if(Text::isNewLine(codePoint))
                {
                        mWhiteSpace = true;
                        dimensions.size.height = Text::getGlyphHeight(fontPtr,'0');
                        dimensions.size.width = 0;
                }
                else if(Text::isSpace(codePoint))
                {
                        mWhiteSpace = true;
                        mAdvance = Text::getGlyphAdvance(fontPtr, Text::UNICODE_SPACE);
                        if(mAdvance <= 0)
                                mAdvance = Text::getGlyphAdvance(fontPtr, Text::UNICODE_ZERO);
                        if(mAdvance <= 0)
                                mAdvance = Text::getGlyphHeight(fontPtr,'0');
                        float height = Text::getGlyphHeight(fontPtr,'0');
                        dimensions.size = Size(mAdvance,height);
                }
                else if(Text::isTab(codePoint))
                {
                        mWhiteSpace = true;
                        float spaceAdvance = Text::getGlyphAdvance(fontPtr, Text::UNICODE_SPACE);
                        if(spaceAdvance <= 0)
                                spaceAdvance = Text::getGlyphAdvance(fontPtr, Text::UNICODE_ZERO);
                        if(spaceAdvance <= 0)
                                spaceAdvance = Text::getGlyphHeight(fontPtr,'0');
                        mAdvance = (spaceAdvance * 4.0f);
                        float height = Text::getGlyphHeight(fontPtr,'0');
                        dimensions.size = Size(mAdvance,height);
                }
                else
                {
                        // Get the glyph's UV Coords
                        uvCoords = Text::getGlyphUVCoords(fontPtr,codePoint);

                        // Use UV Coords to Determine character dimensions
                        dimensions.size = Size(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));

                        mAdvance = Text::getGlyphAdvance(fontPtr, codePoint);
                        mBearing = Text::getGlyphBearing(fontPtr, codePoint);
                        if(mAdvance <= 0)
                                mAdvance = dimensions.size.width;
                }

                dimensions.size.roundUp();

                setBaseline(mBaseline);
        }
}
