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
