#ifndef QUICKGUITEXTALIGNMENT_H
#define QUICKGUITEXTALIGNMENT_H

namespace QuickGUI
{
	/**
	* Specifies horizontal alignment of text within its alloted width.
	*/
	enum HorizontalTextAlignment
	{
		TEXT_ALIGNMENT_HORIZONTAL_CENTER			=  0,
		TEXT_ALIGNMENT_HORIZONTAL_LEFT					,
		TEXT_ALIGNMENT_HORIZONTAL_RIGHT
		
	};

	/**
	* Specifies vertical alignment of text within its alloted width.
	*/
	enum VerticalTextAlignment
	{
		TEXT_ALIGNMENT_VERTICAL_BOTTOM				=  0,
		TEXT_ALIGNMENT_VERTICAL_CENTER					,
		TEXT_ALIGNMENT_VERTICAL_TOP
	};
}

#endif
