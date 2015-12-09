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

#include "QuickGUIOgreEquivalents.h"

namespace QuickGUI
{
	const Vector2 Vector2::ZERO( 0, 0 );

	const Vector3 Vector3::ZERO( 0, 0, 0 );

	bool ColourValue::operator==(const ColourValue& rhs) const
    {
        return (r == rhs.r &&
            g == rhs.g &&
            b == rhs.b &&
            a == rhs.a);
    }
    //---------------------------------------------------------------------
    bool ColourValue::operator!=(const ColourValue& rhs) const
    {
        return !(*this == rhs);
    }

	const ColourValue ColourValue::ZERO = ColourValue(0.0,0.0,0.0,0.0);
    const ColourValue ColourValue::Black = ColourValue(0.0,0.0,0.0);
    const ColourValue ColourValue::White = ColourValue(1.0,1.0,1.0);
    const ColourValue ColourValue::Red = ColourValue(1.0,0.0,0.0);
    const ColourValue ColourValue::Green = ColourValue(0.0,1.0,0.0);
    const ColourValue ColourValue::Blue = ColourValue(0.0,0.0,1.0);
}
