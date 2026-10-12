#ifndef QUICKGUIFONT_H
#define QUICKGUIFONT_H

#include "QuickGUIDeprecated.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISize.h"
#include "QuickGUIUVRect.h"

#include "OgreString.h"
#include "QuickGUIUTFString.h"

namespace QuickGUI
{
	/// A font with the glyph metrics QuickGUI lays text out with. An implementation supplies the raw metrics.
	class _QuickGUIExport Font
	{
	public:
		virtual ~Font() {}

		/// Returns the name the font was registered with.
		virtual std::string getName() const = 0;
		/// Returns the distance in pixels from the top of the tallest glyph to the baseline.
		virtual float getMaxBearingY() const = 0;
		/// Returns the name of the texture that holds the glyph bitmaps.
		virtual std::string getTextureName() const = 0;
		/// Returns the pixel height of the bitmap of a particular code point.
		virtual float getGlyphHeight(UTFString::code_point cp) const = 0;
		/// Returns the UV coordinates of the bitmap of a particular code point.
		virtual UVRect getGlyphUVCoords(UTFString::code_point cp) const = 0;
		/// Returns the pixel size of the bitmap of a particular code point.
		virtual Size getGlyphBitmapSize(UTFString::code_point cp) const = 0;
		/// Returns the horizontal offset in pixels from the pen position to the left of the bitmap of a particular code point.
		virtual float getGlyphBearing(UTFString::code_point cp) const = 0;
		/// Returns the pen advance in pixels the font gives a code point, before the whitespace rules of getGlyphAdvance.
		virtual float getGlyphDesignAdvance(UTFString::code_point cp) const = 0;
		/// Saves the texture that holds the glyph bitmaps to file.
		virtual void saveTextureToFile(const std::string& fileName) const = 0;

		/// Returns the height in pixels of any glyphs within the font.
		float getHeight() const;
		/// Returns the pen advance in pixels after a code point. A space is the width of the digit zero, a tab is four spaces and a newline is zero.
		float getGlyphAdvance(UTFString::code_point cp) const;
		/// Returns the pixel width of a particular string of text.
		float getTextWidth(const UTFString& s) const;
	};
}

#endif
