#ifndef QUICKGUICHARACTER_H
#define QUICKGUICHARACTER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIRect.h"
#include "QuickGUIUVRect.h"
#include "QuickGUIOgreEquivalents.h"
#include "QuickGUIFont.h"
#include "OgreFont.h"
#include "OgreTexture.h"

#include "OgreColourValue.h"
#include "QuickGUIUTFString.h"

namespace QuickGUI
{
	class _QuickGUIExport Character
	{
	public:
		Character(UTFString::code_point cp, const Font& f, ColourValue cv);
		QUICKGUI_DEPRECATED("Use the constructor that takes a Font from Root::getFont.")
		Character(UTFString::code_point cp, Ogre::FontPtr fp, ColourValue cv);		

		/**
		* Returns true if this character is highlighted, false otherwise.
		*/
		bool getHighlighted();
		/**
		* Returns the color of the text background when highlighted.
		*/
		ColourValue getHighlightColor();
		/**
		* Returns the color of the text when highlighted.
		*/
		ColourValue getHighlightedTextColor();

		/**
		* Returns true if this character is a tab, newline, carriage return, space, or other whitespace.
		*/
		bool isWhiteSpace();

		/**
		* Updates fontPtr member variable and recalculates uv coords and dimensions.
		*/
		void setFont(const Font& f);
		/**
		* Sets whether this character is highlighted or not.
		*/
		void setHighlighted(bool highlighted);
		/**
		* Returns the pen position of the character, relative to its TextLine.
		*/
		float getBaseline() const { return dimensions.position.x - mBearing; }
		/**
		* Moves the pen position of the character. The bitmap is drawn at the pen position plus bearing.
		*/
		void setBaseline(float baseline);
		/**
		* Returns the horizontal advance for this glyph.
		*/
		float getAdvance() const { return mAdvance; }
		/// The pen cell of this character, relative to its TextLine: pen position and advance by line height.
		Rect getAdvanceRect() const { return Rect(Point(getBaseline(),dimensions.position.y),Size(mAdvance,dimensions.size.height)); }
		/**
		* Returns the horizontal bearing for this glyph.
		*/
		float getBearing() const { return mBearing; }

		/// Code point of the glyph
		UTFString::code_point codePoint;
		/// Color of the character when drawn.
		ColourValue colorValue;
		/// Position is relative to the parent TextLine
		Rect dimensions;
		/// Font of the glyph being drawn.
		const Font* font;
		/// The Ogre font of the glyph being drawn.
		QUICKGUI_DEPRECATED("Use font.")
		Ogre::FontPtr fontPtr;
		/// The texture of the Ogre font.
		QUICKGUI_DEPRECATED("Use font->getTextureName().")
		Ogre::TexturePtr texturePtr;
		/// UV coords of the glyph bitmap within the font texture.
		UVRect uvCoords;

	protected:
		Character();

		bool mHighlighted;
		float mAdvance;
		float mBearing;
	};
}

#endif
