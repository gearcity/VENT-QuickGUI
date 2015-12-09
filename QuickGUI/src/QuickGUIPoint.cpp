#include "QuickGUIPoint.h"

#include "OgreMath.h"

namespace QuickGUI
{
	Point::Point() :
		x(0),
		y(0)
	{
	}

	Point::Point(float X, float Y) :
		x(X),
		y(Y)
	{
	}

	void Point::translate(const Point& p)
	{
		x += p.x;
		y += p.y;
	}

	Point& Point::roundUp()
	{
		x = Ogre::Math::Ceil(x);
		y = Ogre::Math::Ceil(y);

		return *this;
	}

	Point& Point::roundDown()
	{
		x = Ogre::Math::Floor(x);
		y = Ogre::Math::Floor(y);

		return *this;
	}

	Point& Point::round()
	{
		x = Ogre::Math::Floor(x + 0.5);
		y = Ogre::Math::Floor(y + 0.5);

		return *this;
	}

	const Point Point::ZERO( 0, 0 );
}
