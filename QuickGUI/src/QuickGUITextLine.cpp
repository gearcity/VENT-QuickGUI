#include "QuickGUITextLine.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	TextLine::TextLine()
	{
		mLargestFont.setNull();
		mMaskText = false;
	}

	TextLine::~TextLine()
	{
	}

	void TextLine::_verticallyAlignCharacters()
	{
		Ogre::TexturePtr largestFontTexture = Text::getFontTexture(mLargestFont);
		float maxBaseLine = static_cast<float>(mLargestFont->getTrueTypeMaxBearingY());

		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			Ogre::TexturePtr tp = Text::getFontTexture((*it)->fontPtr);
			float baseLine = static_cast<float>((*it)->fontPtr->getTrueTypeMaxBearingY());

			(*it)->dimensions.position.y = maxBaseLine - baseLine;
			(*it)->dimensions.position.roundUp();
		}
	}

	void TextLine::addCharacter(Character* c)
	{
		float baseline = 0;
		if(!mCharacters.empty())
		{
			Character* lastChar = mCharacters.back();
			baseline = lastChar->getBaseline() + lastChar->getAdvance();
		}
		
		c->setBaseline(baseline);
		mCharacters.push_back(c);

		if(mLargestFont.isNull() || Text::getFontHeight(c->fontPtr) > Text::getFontHeight(mLargestFont))
		{
			mLargestFont = c->fontPtr;
			_verticallyAlignCharacters();
		}
		else
		{
			float maxBaseLine = static_cast<float>(mLargestFont->getTrueTypeMaxBearingY());
			float baseLine = static_cast<float>(c->fontPtr->getTrueTypeMaxBearingY());
			c->dimensions.position.y = maxBaseLine - baseLine;
		}

		c->dimensions.position.roundUp();
	}

	void TextLine::draw(Point p)
	{
		mPosition = p;
		mPosition.roundUp();
		Brush* brush = Brush::getSingletonPtr();
		Ogre::String currentTexture = "TextSelection";

		// Render any quads queued for drawing.
		brush->endRectQueue();

		// Do an initial pass to draw any quads related to text highlighting
		brush->setTexture("TextSelection");

		brush->beginRectQueue();

		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			// Ignore any newline characters
			if(Text::isNewLine((*it)->codePoint) || Text::isNullCharacter((*it)->codePoint))
				continue;

			if((*it)->getHighlighted())
			{
				brush->setColor((*it)->getHighlightColor());
				brush->queueRect(Rect((*it)->getAdvanceRect().position + mPosition,(*it)->getAdvanceRect().size),(*it)->uvCoords);
			}
		}

		// Do a second pass and render the glyphs

		// If the text is masked, we just need to render the mask Symbol.  We will render using the largest font
		if(mMaskText)
		{
			// Render any currently queued quads
			brush->endRectQueue();

			// Set texture to largest font's texture
			Ogre::TexturePtr texturePtr = Text::getFontTexture(mLargestFont);
			brush->setTexture(texturePtr);
			if(!mCharacters.empty())
				brush->setColor(mCharacters.front()->colorValue);

			brush->beginRectQueue();

			// Get the glyph's UV Coords
			UVRect uvCoords = Text::getGlyphUVCoords(mLargestFont,mMaskSymbol);
			// Use UV Coords to Determine character dimensions
			Size s(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));
			float maskAdvance = Text::getGlyphAdvance(mLargestFont,mMaskSymbol);
			float maskBearing = Text::getGlyphBearing(mLargestFont,mMaskSymbol);

			Point glyphPosition(maskBearing, mCharacters.front()->dimensions.position.y);
			glyphPosition.roundUp();

			for(size_t i = 0; (i + 1) < mCharacters.size(); ++i)
			{
				brush->queueRect(Rect(Point(glyphPosition.x + (i * maskAdvance),glyphPosition.y) + mPosition,s),uvCoords);
			}
		}
		else
		{
			for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			{
				if((*it)->isWhiteSpace())
					continue;

				if( currentTexture != (*it)->texturePtr->getName() )
				{
					brush->endRectQueue();

					currentTexture = (*it)->texturePtr->getName();
					brush->setTexture(currentTexture);

					brush->beginRectQueue();
				}

				if((*it)->getHighlighted())
					brush->setColor((*it)->getHighlightedTextColor());
				else
					brush->setColor((*it)->colorValue);

				brush->queueRect(Rect((*it)->dimensions.position + mPosition,(*it)->dimensions.size),(*it)->uvCoords);
			}
		}
	}

	bool TextLine::empty()
	{
		return mCharacters.empty();
	}

	Character* TextLine::getCharacter(int index)
	{
		if(mCharacters.empty())
			return NULL;

		if(index < 0)
			index = 0;
		else if(index >= static_cast<int>(mCharacters.size()))
			index = static_cast<int>(mCharacters.size()) - 1;

		return mCharacters[index];
	}

	int TextLine::getCursorIndexAtPosition(Point p)
	{
		int index = 0;

		if(mMaskText)
		{
			float advance = Text::getGlyphAdvance(mLargestFont,mMaskSymbol);
			float charPos = 0;

			for(size_t i = 0; i < mCharacters.size(); ++i)
			{
				if(p.x < charPos)
				{
					if((index > 0) && (p.x < (charPos - (advance / 2.0f))))
						return (index - 1);
					else
						return index;
				}

				charPos += advance;
				++index;
			}

			return index;
		}

		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			float left = (*it)->getBaseline();

			if(p.x < left)
			{
				if((index > 0) && (p.x < (left - (mCharacters[index - 1]->getAdvance() / 2.0f))))
					return (index - 1);
				else
					return index;
			}

			++index;
		}

		return index - 1;
	}

	float TextLine::getHeight()
	{
		if(mLargestFont.isNull())
			return 0;

		return Text::getFontHeight(mLargestFont);
	}

	int TextLine::getLength()
	{
		return static_cast<int>(mCharacters.size());
	}

	Point TextLine::getPositionAtCharacterIndex(unsigned int index)
	{
		// Force index to be valid for all values given
		if(index >= static_cast<unsigned int>(mCharacters.size()))
			index = static_cast<unsigned int>(mCharacters.size()) - 1;

		Point p;

		if(mMaskText)
			p.x = index * Text::getGlyphAdvance(mLargestFont,mMaskSymbol);
		else
			p.x = mCharacters[index]->getBaseline();

		return p;
	}

	float TextLine::getWidth()
	{
		if(mCharacters.empty())
			return 0;

		if(mMaskText)
		{
			float width = 0;
			float advance = Text::getGlyphAdvance(mLargestFont,mMaskSymbol);

			for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			{
				if(Text::isNullCharacter((*it)->codePoint))
					continue;

				width += advance;
			}

			return width;
		}

		Character* firstCharacter = NULL;
		Character* lastCharacter = NULL;

		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			Character* character = (*it);
			if(Text::isNewLine(character->codePoint) || Text::isNullCharacter(character->codePoint))
				continue;

			if(firstCharacter == NULL)
				firstCharacter = character;

			lastCharacter = character;
		}

		if(firstCharacter == NULL || lastCharacter == NULL)
			return 0;

		return (lastCharacter->getBaseline() + lastCharacter->getAdvance()) - firstCharacter->getBaseline();
	}

	void TextLine::setMaskText(bool mask, Ogre::UTFString::code_point maskSymbol)
	{
		mMaskText = mask;
		mMaskSymbol = maskSymbol;
	}
}
