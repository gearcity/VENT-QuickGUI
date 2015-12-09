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

#ifndef QUICKGUISIZE_H
#define QUICKGUISIZE_H

#include "QuickGUIExportDLL.h"

namespace QuickGUI
{
	class _QuickGUIExport Size
	{
	public:
		Size();
		Size(float WidthHeight);
		Size(float Width, float Height);

		inline Size( const Size& s )
            : width( s.width ), height( s.height )
        {
        }

		inline Size& operator = ( const Size& s )
        {
            width = s.width;
            height = s.height;

            return *this;
        }

		inline bool operator != ( const Size& s ) const
        {
            return ( width != s.width ||
                height != s.height );
        }

		inline bool operator == ( const Size& s ) const
        {
            return ( static_cast<int>(width) == static_cast<int>(s.width) &&
                static_cast<int>(height) == static_cast<int>(s.height) );
        }

		inline Size& operator /= ( float f )
        {
            width /= f;
            height /= f;

            return *this;
        }

		inline Size operator * ( const float& r ) const
        {
            return Size(width * r,height * r);
        }

		inline Size& operator *= ( const float& r )
        {
            width *= r;
            height *= r;

            return *this;
        }

		inline Size operator * ( const Size& s ) const
        {
			return Size(width * s.width,height * s.height);
        }

		inline Size operator / ( const Size& s ) const
        {
            return Size(width / s.width,height / s.height);
        }

		inline Size operator + ( const Size& s ) const
        {
            return Size(width + s.width,height + s.height);
        }

		inline Size operator - ( const Size& s ) const
        {
            return Size(width - s.width,height - s.height);
        }

		Size& roundUp();
		Size& roundDown();
		Size& round();

		float width;
		float height;

		static const Size ZERO;
	};
}

#endif
