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

#ifndef QUICKGUITOOLBARENUMS_H
#define QUICKGUITOOLBARENUMS_H

namespace QuickGUI
{
	/**
	* Specifies whether ToolBar items are arranged vertically
	* or horizontally.
	*/
	enum ToolBarOrientation
	{
		TOOLBAR_ORIENTATION_HORIZONTAL				=  0,
		TOOLBAR_ORIENTATION_VERTICAL
	};

	/**
	* Defines ToolBar States.
	*/
	enum ToolBarState
	{
		TOOLBAR_STATE_OPEN_MENUS				=  0,
		TOOLBAR_STATE_DEFAULT
	};

	/**
	* Defines whether items are vertically arranged top to bottom or bottom to top,
	* or horizontally arranged left to right or right to left.
	*/
	enum ToolBarItemLayout
	{
		TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE				=  0,
		TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE
	};
}

#endif
