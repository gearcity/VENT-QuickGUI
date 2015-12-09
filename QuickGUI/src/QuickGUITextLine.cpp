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
		float maxBaseLine = (mLargestFont->getTrueTypeMaxBearingY() >> 6);

		for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		{
			Ogre::TexturePtr tp = Text::getFontTexture((*it)->fontPtr);
			float baseLine = ((*it)->fontPtr->getTrueTypeMaxBearingY() >> 6);

			(*it)->dimensions.position.y = maxBaseLine - baseLine;
			(*it)->dimensions.position.roundUp();
		}
	}

	void TextLine::addCharacter(Character* c)
	{
		// Check if character can fit within the allotted width for this TextLine
		float xCoord = 0;
		if(!mCharacters.empty())
		{
			Character* lastChar = mCharacters.back();
			xCoord = lastChar->dimensions.position.x + lastChar->dimensions.size.width;
		}
		
		// Add the character and set its position
		mCharacters.push_back(c);

		c->dimensions.position.x += xCoord;

		// Adjust Y position to match largest baseline
		if(mLargestFont.isNull() || Text::getFontHeight(c->fontPtr) > Text::getFontHeight(mLargestFont))
		{
			mLargestFont = c->fontPtr;
			_verticallyAlignCharacters();
		}
		else
		{
			Ogre::TexturePtr largestFontTexture = Text::getFontTexture(mLargestFont);
			float maxBaseLine = (mLargestFont->getTrueTypeMaxBearingY() >> 6);
			float baseLine = (c->fontPtr->getTrueTypeMaxBearingY() >> 6);

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
				brush->queueRect(Rect((*it)->dimensions.position + mPosition,(*it)->dimensions.size),(*it)->uvCoords);
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
			brush->setColor(mCharacters.front()->colorValue);

			brush->beginRectQueue();

			// Get the glyph's UV Coords
			UVRect uvCoords = Text::getGlyphUVCoords(mLargestFont,mMaskSymbol);
			// Use UV Coords to Determine character dimensions
			Size s(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));

			Point p = mCharacters.front()->dimensions.position;
			p.roundUp();

			for(std::vector<Character*>::iterator it = mCharacters.begin(); it != --(mCharacters.end()); ++it)
			{
				brush->queueRect(Rect(p + mPosition,s),uvCoords);
				p.x += s.width;
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
			Ogre::TexturePtr texturePtr = Text::getFontTexture(mLargestFont);

			// Get the glyph's UV Coords
			UVRect uvCoords = Text::getGlyphUVCoords(mLargestFont,mMaskSymbol);
			// Use UV Coords to Determine character dimensions
			Size s(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));

			// Simulated position - text is masked
			float charPos = 0;

			for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			{
				// Determine if the position is to the left of a given character
				if(p.x < charPos)
				{
					// If we make it here we know the position is to the left of this cursor.
					// If there is previous character and the position is closer to the previous character, return its index.
					if((index > 0) &&  (p.x < (charPos - (s.width / 2.0))))
						return (index - 1);
					else
						return index;
				}

				charPos += s.width;
				++index;
			}
		}
		else
		{
			for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			{
				// Determine if the position is to the left of a given character
				if(p.x < (*it)->dimensions.position.x)
				{
					// If we make it here we know the position is to the left of this cursor.
					// If there is previous character and the position is closer to the previous character, return its index.
					if((index > 0) &&  (p.x < ((*it)->dimensions.position.x - (mCharacters[index - 1]->dimensions.size.width / 2.0))))
						return (index - 1);
					else
						return index;
				}

				++index;
			}

			index -= 1;
		}

		return index;
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
		{
			Ogre::TexturePtr texturePtr = Text::getFontTexture(mLargestFont);

			// Get the glyph's UV Coords
			UVRect uvCoords = Text::getGlyphUVCoords(mLargestFont,mMaskSymbol);
			// Use UV Coords to Determine character dimensions
			Size s(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));

			p.x = index * s.width;
		}
		else
		{
			p.x = mCharacters[index]->dimensions.position.x;
		}

		return p;
	}

	float TextLine::getWidth()
	{
		if(mCharacters.empty())
			return 0;

		float width = 0;

		if(mMaskText)
		{
			Ogre::TexturePtr texturePtr = Text::getFontTexture(mLargestFont);
		
			// Get the glyph's UV Coords
			UVRect uvCoords = Text::getGlyphUVCoords(mLargestFont,mMaskSymbol);
			// Use UV Coords to Determine character dimensions
			Size s(((uvCoords.right - uvCoords.left) * texturePtr->getWidth()),((uvCoords.bottom - uvCoords.top) * texturePtr->getHeight()));

			for(std::vector<Character*>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
			{
				if(Text::isNullCharacter((*it)->codePoint))
					continue;

				width += s.width;
			}
		}
		else
		{
			width = ((mCharacters.back()->dimensions.position.x + mCharacters.back()->dimensions.size.width) - mCharacters.front()->dimensions.position.x);
		}

		return width;
	}

	void TextLine::setMaskText(bool mask, Ogre::UTFString::code_point maskSymbol)
	{
		mMaskText = mask;
		mMaskSymbol = maskSymbol;
	}
}
