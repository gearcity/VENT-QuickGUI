#ifndef QUICKGUIPOINT_H
#define QUICKGUIPOINT_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISize.h"

namespace QuickGUI
{
	class _QuickGUIExport Point
	{
	public:
		Point();
		Point(float X, float Y);

		inline Point( const Point& p )
            : x( p.x ), y( p.y )
        {
        }

		inline Point& operator = ( const Point& p )
        {
            x = p.x;
            y = p.y;

            return *this;
        }

		inline bool operator != ( const Point& p ) const
        {
            return ( x != p.x ||
                y != p.y );
        }

		inline bool operator == ( const Point& p ) const
        {
            return ( x == p.x &&
                y == p.y );
        }

		inline Point operator * ( const float r ) const
        {
            return Point(x * r,y * r);
        }

		inline Point operator / ( const Point& p ) const
        {
            return Point(x / p.x,y / p.y);
        }

		inline Point operator / ( const Size& s ) const
        {
            return Point(x / s.width,y / s.height);
        }

		inline Point& operator /= ( float f )
        {
            x /= f;
            y /= f;

            return *this;
        }

		inline Point operator + ( const Point& p ) const
        {
            return Point(x + p.x,y + p.y);
        }

		inline Point operator + ( const Size& s ) const
        {
            return Point(x + s.width,y + s.height);
        }

		inline bool operator < ( const Point& p ) const
		{
			if(x < p.x)
				return true;
			if(y < p.y)
				return true;

			return false;
		}

		inline Point operator += ( const Size& s )
        {
			x += s.width;
			y += s.height;

            return *this;
        }

		inline Point operator += ( const Point& p )
		{
			x += p.x;
			y += p.y;

			return *this;
		}

		inline Point operator - ( const Point& p ) const
        {
            return Point(x - p.x,y - p.y);
        }

		inline Point operator -= ( const Point& p )
        {
			x -= p.x;
			y -= p.y;

			return *this;
        }

		void translate(const Point& p);

		Point& roundUp();
		Point& roundDown();
		Point& round();

		float x;
		float y;

		static const Point ZERO;
	};
}

#endif
