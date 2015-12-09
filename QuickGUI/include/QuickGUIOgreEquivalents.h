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

#ifndef QUICKGUIOGREEQUIVALENTS_H
#define QUICKGUIOGREEQUIVALENTS_H

#include "QuickGUIExportDLL.h"

namespace QuickGUI
{
	struct _QuickGUIExport Vector2
	{
		float x;
		float y;

		inline Vector2() :
			x(0),
			y(0)
        {
        }

		inline Vector2(const float fX, const float fY ) : 
			x( fX ), 
			y( fY )
        {
        }

		static const Vector2 ZERO;
	};

	struct _QuickGUIExport Vector3
	{
		float x;
		float y;
		float z;

		inline Vector3() :
			x(0),
			y(0),
			z(0)
        {
        }

		inline Vector3(const float fX, const float fY , const float fZ ) : 
			x( fX ), 
			y( fY ),
			z( fZ )
        {
        }

		static const Vector3 ZERO;
	};

	struct _QuickGUIExport ColourValue
	{
		float r;
		float g;
		float b;
		float a;

		inline ColourValue( float red = 1.0f,float green = 1.0f, float blue = 1.0f, float alpha = 1.0f ) : 
			r(red), 
			g(green), 
			b(blue), 
			a(alpha)
        { 
		}

		bool operator==(const ColourValue& rhs) const;
	    bool operator!=(const ColourValue& rhs) const;

		static const ColourValue ZERO;
        static const ColourValue Black;
        static const ColourValue White;
        static const ColourValue Red;
        static const ColourValue Green;
        static const ColourValue Blue;
	};
}

#endif
