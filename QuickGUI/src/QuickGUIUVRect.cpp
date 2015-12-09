#include "QuickGUIUVRect.h"

namespace QuickGUI
{
	UVRect::UVRect() :
		left(0),
		right(0),
		top(1),
		bottom(1)
	{
	}

	UVRect::UVRect(float Left, float Top, float Right, float Bottom) :
		left(Left),
		top(Top),
		right(Right),
		bottom(Bottom)
	{
	}

	const UVRect UVRect::ZERO( 0, 0, 0, 0 );
}
