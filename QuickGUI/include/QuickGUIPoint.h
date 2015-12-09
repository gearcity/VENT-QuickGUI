/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

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
            return ( static_cast<int>(x) == static_cast<int>(p.x) &&
                static_cast<int>(y) == static_cast<int>(p.y) );
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
