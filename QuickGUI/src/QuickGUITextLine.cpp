#include "QuickGUITextLine.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	TextLine::TextLine()
	{
		mLargestFont = NULL;
		mMaskText = false;
	}

	TextLine::~TextLine()
	{
	}

	void TextLine::_verticallyAlignCharacters()
	{
		float maxBaseLine = mLargestFont->getMaxBearingY();

		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			float baseLine = (*it)->font->getMaxBearingY();

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

		if((mLargestFont == NULL) || (c->font->getHeight() > mLargestFont->getHeight()))
		{
			mLargestFont = c->font;
			_verticallyAlignCharacters();
		}
		else
		{
			float maxBaseLine = mLargestFont->getMaxBearingY();
			float baseLine = c->font->getMaxBearingY();
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
			brush->setTexture(mLargestFont->getTextureName());
			if(!mCharacters.empty())
				brush->setColor(mCharacters.front()->colorValue);

			brush->beginRectQueue();

			// Get the glyph's UV Coords
			UVRect uvCoords = mLargestFont->getGlyphUVCoords(mMaskSymbol);
			Size s = mLargestFont->getGlyphBitmapSize(mMaskSymbol);
			float maskAdvance = mLargestFont->getGlyphAdvance(mMaskSymbol);
			float maskBearing = mLargestFont->getGlyphBearing(mMaskSymbol);

			// An asterisk is drawn near the top of its bitmap, so center what the glyph shows in the line.
			float maskShift = (mLargestFont->getHeight() / 2.0f) - mLargestFont->getGlyphInkCenter(mMaskSymbol);

			Point glyphPosition(maskBearing, mCharacters.front()->dimensions.position.y + maskShift);
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

				if( currentTexture != (*it)->font->getTextureName() )
				{
					brush->endRectQueue();

					currentTexture = (*it)->font->getTextureName();
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
			float advance = mLargestFont->getGlyphAdvance(mMaskSymbol);
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
		if(mLargestFont == NULL)
		{
			return 0;
		}

		return mLargestFont->getHeight();
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
			p.x = index * mLargestFont->getGlyphAdvance(mMaskSymbol);
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
			float advance = mLargestFont->getGlyphAdvance(mMaskSymbol);

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
