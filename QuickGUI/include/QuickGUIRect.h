#ifndef QUICKGUIRECT_H
#define QUICKGUIRECT_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIPoint.h"
#include "QuickGUISize.h"

namespace QuickGUI
{
	class _QuickGUIExport Rect
	{
	public:
		Rect();
		Rect(float x, float y, float width, float height);
		Rect(Point p, Size s);

		inline Rect( const Rect& r )
			: position( r.position ), size( r.size )
        {
        }

		inline Rect& operator = ( const Rect& r )
        {
			position = r.position;
			size = r.size;

            return *this;
        }

		inline bool operator != ( const Rect& r ) const
        {
			return ( position != r.position ||
				size != r.size );
        }

		bool operator == ( const Rect& r ) const
        {
			return ( position == r.position &&
				size == r.size );
        }

		inline Rect& operator /= ( float f )
        {
			position /= f;
			size /= f;

			return *this;
        }


		Rect getIntersection( const Rect& r ) const;

		bool inside(const Rect& r) const;
		bool intersectsRect(const Rect& r) const;
		bool isPointWithinBounds(const Point& pixelPosition) const;

		void translate(const Point& p);

		Point position;
		Size size;

		static const Rect ZERO;
	};
}

#endif
