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
