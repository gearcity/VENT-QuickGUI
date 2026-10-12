#include "QuickGUIFont.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	float Font::getHeight() const
	{
		return getGlyphHeight(Text::UNICODE_ZERO);
	}

	float Font::getGlyphAdvance(Ogre::UTFString::code_point cp) const
	{
		if(Text::isNewLine(cp))
		{
			return 0;
		}

		if(Text::isSpace(cp) || Text::isTab(cp))
		{
			float spaceAdvance = getGlyphDesignAdvance(Text::UNICODE_ZERO);
			if(spaceAdvance <= 0)
			{
				spaceAdvance = getGlyphHeight(Text::UNICODE_ZERO);
			}

			if(Text::isTab(cp))
			{
				return (spaceAdvance * 4.0f);
			}

			return spaceAdvance;
		}

		return getGlyphDesignAdvance(cp);
	}

	float Font::getTextWidth(const Ogre::UTFString& s) const
	{
		float width = 0;

		for(Ogre::UTFString::const_iterator it = s.begin(); it != s.end(); ++it)
		{
			width += getGlyphAdvance((*it));
		}

		return width;
	}
}
