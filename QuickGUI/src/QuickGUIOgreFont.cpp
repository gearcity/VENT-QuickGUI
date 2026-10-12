#include "QuickGUIOgreFont.h"
#include "QuickGUIText.h"

#include "OgreHardwarePixelBuffer.h"
#include "OgreImage.h"
#include "OgreMaterial.h"
#include "OgreTechnique.h"
#include "OgreTextureManager.h"

#include <vector>

namespace QuickGUI
{
	OgreFont::OgreFont(Ogre::FontPtr fp) :
		mFont(fp)
	{
		mFont->load();
	}

	Ogre::String OgreFont::getName() const
	{
		return mFont->getName();
	}

	float OgreFont::getMaxBearingY() const
	{
		return static_cast<float>(mFont->getTrueTypeMaxBearingY());
	}

	Ogre::String OgreFont::getTextureName() const
	{
		return mFont->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
	}

	Ogre::TexturePtr OgreFont::getTexture() const
	{
		return static_cast<Ogre::TexturePtr>(Ogre::TextureManager::getSingleton().getByName(getTextureName()));
	}

	float OgreFont::getGlyphHeight(Ogre::UTFString::code_point cp) const
	{
		Ogre::Font::UVRect uvRect = mFont->getGlyphTexCoords(cp);

		return ((uvRect.bottom - uvRect.top) * getTexture()->getHeight());
	}

	UVRect OgreFont::getGlyphUVCoords(Ogre::UTFString::code_point cp) const
	{
		Ogre::Font::UVRect uvRect = mFont->getGlyphTexCoords(cp);

		UVRect retVal;
		retVal.left = uvRect.left;
		retVal.top = uvRect.top;
		retVal.right = uvRect.right;
		retVal.bottom = uvRect.bottom;

		return retVal;
	}

	Size OgreFont::getGlyphBitmapSize(Ogre::UTFString::code_point cp) const
	{
		Ogre::TexturePtr tp = getTexture();
		Ogre::Font::UVRect uvRect = mFont->getGlyphTexCoords(cp);

		return Size(((uvRect.right - uvRect.left) * tp->getWidth()),((uvRect.bottom - uvRect.top) * tp->getHeight()));
	}

	float OgreFont::getGlyphMetricScale(Ogre::UTFString::code_point cp) const
	{
		const Ogre::GlyphInfo& glyph = mFont->getGlyphInfo(cp);

		float scale = (glyph.uvRect.bottom - glyph.uvRect.top) * getTexture()->getHeight();
		if(scale == 0)
		{
			scale = getGlyphHeight(Text::UNICODE_ZERO);
		}

		return scale;
	}

	float OgreFont::getGlyphBearing(Ogre::UTFString::code_point cp) const
	{
		const Ogre::GlyphInfo& glyph = mFont->getGlyphInfo(cp);

		return Ogre::Math::Floor((glyph.bearing * getGlyphMetricScale(cp)) + 0.5f);
	}

	float OgreFont::getGlyphDesignAdvance(Ogre::UTFString::code_point cp) const
	{
		const Ogre::GlyphInfo& glyph = mFont->getGlyphInfo(cp);

		float advance = Ogre::Math::Floor((glyph.advance * getGlyphMetricScale(cp)) + 0.5f);
		if(advance <= 0)
		{
			float width = (glyph.uvRect.right - glyph.uvRect.left) * getTexture()->getWidth();
			if(width > advance)
			{
				advance = width;
			}
		}

		return advance;
	}

	float OgreFont::getGlyphInkCenter(Ogre::UTFString::code_point cp) const
	{
		Ogre::TexturePtr tp = getTexture();
		UVRect uv = getGlyphUVCoords(cp);

		const size_t left = static_cast<size_t>((uv.left * tp->getWidth()) + 0.5f);
		const size_t top = static_cast<size_t>((uv.top * tp->getHeight()) + 0.5f);
		const size_t width = static_cast<size_t>(((uv.right - uv.left) * tp->getWidth()) + 0.5f);
		const size_t height = static_cast<size_t>(((uv.bottom - uv.top) * tp->getHeight()) + 0.5f);

		if((width == 0) || (height == 0))
		{
			return height / 2.0f;
		}

		std::vector<unsigned char> pixels(width * height * 4);
		Ogre::PixelBox box(width, height, 1, Ogre::PF_BYTE_RGBA, &pixels[0]);
		tp->getBuffer()->blitToMemory(Ogre::Box(left, top, left + width, top + height), box);

		size_t firstRow = height;
		size_t lastRow = 0;
		for(size_t y = 0; y < height; ++y)
		{
			for(size_t x = 0; x < width; ++x)
			{
				if(pixels[((y * width) + x) * 4 + 3] != 0)
				{
					if(y < firstRow)
					{
						firstRow = y;
					}
					lastRow = y;
				}
			}
		}

		if(firstRow == height)
		{
			return height / 2.0f;
		}

		return (firstRow + lastRow + 1) / 2.0f;
	}

	void OgreFont::saveTextureToFile(const Ogre::String& fileName) const
	{
		Ogre::TexturePtr tp = getTexture();

		// Declare buffer
		const size_t buffSize = (tp->getWidth() * tp->getHeight() * 4);
		unsigned char *data = OGRE_ALLOC_T(unsigned char,buffSize,Ogre::MEMCATEGORY_GENERAL);

		// Clear buffer
		memset(data, 0, buffSize);

		// Setup Image with correct settings
		Ogre::Image i;
		i.loadDynamicImage(data, tp->getWidth(), tp->getHeight(), 1, Ogre::PF_R8G8B8A8, true);

		// Copy Texture buffer contents to image buffer
		Ogre::HardwarePixelBufferSharedPtr buf = tp->getBuffer();
		const Ogre::PixelBox destBox = i.getPixelBox();
		buf->blitToMemory(destBox);

		// Save to disk!
		i.save(fileName);
	}
}
