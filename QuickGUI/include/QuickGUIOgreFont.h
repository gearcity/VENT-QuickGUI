#ifndef QUICKGUIOGREFONT_H
#define QUICKGUIOGREFONT_H

#include "QuickGUIFont.h"

#include <OgreFont.h>
#include <OgreTexture.h>

namespace QuickGUI
{
	/// A font that has been loaded by Ogre.
	class _QuickGUIExport OgreFont :
		public Font
	{
	public:
		explicit OgreFont(Ogre::FontPtr fp);

		std::string getName() const;
		float getMaxBearingY() const;
		std::string getTextureName() const;
		float getGlyphHeight(UTFString::code_point cp) const;
		UVRect getGlyphUVCoords(UTFString::code_point cp) const;
		Size getGlyphBitmapSize(UTFString::code_point cp) const;
		float getGlyphBearing(UTFString::code_point cp) const;
		float getGlyphDesignAdvance(UTFString::code_point cp) const;
		void saveTextureToFile(const std::string& fileName) const;

	protected:
		Ogre::TexturePtr getTexture() const;
		/// Pixel height that the bearing and advance Ogre reports, as fractions of the glyph height, are scaled by.
		float getGlyphMetricScale(UTFString::code_point cp) const;

		Ogre::FontPtr mFont;
	};
}

#endif
