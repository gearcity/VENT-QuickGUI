#include "QuickGUIRect.h"

#include <algorithm>

namespace QuickGUI
{
	Rect::Rect() :
		position(0,0),
		size(0,0)
	{
	}

	Rect::Rect(float X, float Y, float Width, float Height) :
		position(X,Y),
		size(Width,Height)
	{
	}

	Rect::Rect(Point p, Size s) :
		position(p),
		size(s)
	{
	}

	Rect Rect::getIntersection( const Rect& r ) const
	{
		Rect retVal = Rect::ZERO;

		if(intersectsRect(r))
		{
			retVal.position.x = std::max(position.x,r.position.x);
			retVal.position.y = std::max(position.y,r.position.y);
			retVal.size.width = std::min(position.x + size.width, r.position.x + r.size.width) - retVal.position.x;
			retVal.size.height = std::min(position.y + size.height, r.position.y + r.size.height) - retVal.position.y;
		}

		return retVal;
	}

	bool Rect::inside(const Rect& r) const
	{
		if( (position.x >= r.position.x) &&
			(position.y >= r.position.y) &&
			((position.x + size.width) <= (r.position.x + r.size.width)) &&
			((position.y + size.height) <= (r.position.y + r.size.height)) )
			return true;

		return false;
	}

	bool Rect::intersectsRect(const Rect& r) const
	{
		// if our left side is greater than r's right side, or our right side is less than r's left side, intersection is not possible.
		if( (position.x > (r.position.x + r.size.width)) || ((position.x + size.width) < r.position.x) )
			return false;

		// if our top is greater than r's bottom, or our bottom is less than r's top, intersection is not possible.
		if( (position.y > (r.position.y + r.size.height)) || ((position.y + size.height) < r.position.y) )
			return false;

		// If the above conditions are not met, than there must be overlap between our dimensions and r's dimensions.
		return true;
	}

	bool Rect::isPointWithinBounds(const Point& pixelPosition) const
	{
		float xPos = pixelPosition.x;
		float yPos = pixelPosition.y;

		if( (xPos < position.x) || (xPos > (position.x + size.width)) )
			return false;

		if( (yPos < position.y) || (yPos > (position.y + size.height)) )
			return false;

		return true;
	}

	void Rect::translate(const Point& p)
	{
		position.x += p.x;
		position.y += p.y;
	}

	const Rect Rect::ZERO( 0, 0, 0, 0 );
}
