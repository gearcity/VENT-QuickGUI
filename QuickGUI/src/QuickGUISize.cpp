#include "QuickGUISize.h"

#include "OgreMath.h"

namespace QuickGUI
{
	Size::Size() :
		width(0),
		height(0)
	{
	}

	Size::Size(float WidthHeight) :
		width(WidthHeight),
		height(WidthHeight)
	{
	}

	Size::Size(float Width, float Height) :
		width(Width),
		height(Height)
	{
	}

	Size& Size::roundUp()
	{
		width = Ogre::Math::Ceil(width);
		height = Ogre::Math::Ceil(height);

		return *this;
	}

	Size& Size::roundDown()
	{
		width = Ogre::Math::Floor(width);
		height = Ogre::Math::Floor(height);

		return *this;
	}

	Size& Size::round()
	{
		width = Ogre::Math::Floor(width + 0.5);
		height = Ogre::Math::Floor(height + 0.5);

		return *this;
	}

	const Size Size::ZERO( 0, 0 );
}
