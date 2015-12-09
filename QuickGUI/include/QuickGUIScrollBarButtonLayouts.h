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

#ifndef QUICKGUISCROLLBARBUTTONTYPES_H
#define QUICKGUISCROLLBARBUTTONTYPES_H

namespace QuickGUI
{
	enum VScrollBarButtonLayout
	{
		VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP		=  0,
		VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN			,
		VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH			,
		VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP			,
		VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN			,
		VSCROLL_BAR_BUTTON_LAYOUT_NONE					,
		VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE			
	};

	enum HScrollBarButtonLayout
	{
		HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT		=  0,
		HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT		,
		HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH			,
		HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT			,
		HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT		,
		HSCROLL_BAR_BUTTON_LAYOUT_NONE					,
		HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE			
	};
}

#endif
