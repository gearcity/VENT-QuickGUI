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
