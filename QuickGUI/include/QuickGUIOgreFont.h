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

		Ogre::String getName() const;
		float getMaxBearingY() const;
		Ogre::String getTextureName() const;
		float getGlyphHeight(Ogre::UTFString::code_point cp) const;
		UVRect getGlyphUVCoords(Ogre::UTFString::code_point cp) const;
		Size getGlyphBitmapSize(Ogre::UTFString::code_point cp) const;
		float getGlyphBearing(Ogre::UTFString::code_point cp) const;
		float getGlyphDesignAdvance(Ogre::UTFString::code_point cp) const;
		float getGlyphInkCenter(Ogre::UTFString::code_point cp) const;
		void saveTextureToFile(const Ogre::String& fileName) const;

	protected:
		Ogre::TexturePtr getTexture() const;
		/// Pixel height that the bearing and advance Ogre reports, as fractions of the glyph height, are scaled by.
		float getGlyphMetricScale(Ogre::UTFString::code_point cp) const;

		Ogre::FontPtr mFont;
	};
}

#endif
