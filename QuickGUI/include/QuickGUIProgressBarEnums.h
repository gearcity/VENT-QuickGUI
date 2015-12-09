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

#ifndef QUICKGUIPROGRESSBARENUMS_H
#define QUICKGUIPROGRESSBARENUMS_H

namespace QuickGUI
{
		/**
		* Allows setting the direction the progressbar fills.
		* FILLS_NEGATIVE_TO_POSITIVE: 
		*	For Vertical Layouts, bar moves bottom to top. For Horizontal, bar moves left to right.
		* FILLS_POSITIVE_TO_NEGATIVE: 
		*	For Vertical Layouts, bar moves top to bottom. For Horizontal, bar moves right to left.
		*/
		enum ProgressBarFillDirection
		{
			PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE	=  0,
			PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE
		};

		enum ProgressBarLayout
		{
			PROGRESSBAR_LAYOUT_HORIZONTAL			=  0,
			PROGRESSBAR_LAYOUT_VERTICAL
		};

		/**
		* Dictates what side of the texture to *chop*.
		* PROGRESSBAR_CLIP_LEFT_BOTTOM: 
		*	For Vertical Layouts, texture chopped at the bottom. For Horizontal, texture chopped at the left.
		* PROGRESSBAR_CLIP_RIGHT_TOP: 
		*	For Vertical Layouts, texture chopped at the top. For Horizontal, texture chopped at the right.
		*/
		enum ProgressBarClippingEdge
		{
			PROGRESSBAR_CLIP_LEFT_BOTTOM			=  0,
			PROGRESSBAR_CLIP_RIGHT_TOP
		};
}

#endif
