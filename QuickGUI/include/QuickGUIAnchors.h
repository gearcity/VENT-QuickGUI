#ifndef QUICKGUIANCHORS_H
#define QUICKGUIANCHORS_H

namespace QuickGUI
{
	/**
	* Specifies horizontal position/size relative to parent resizing.
	*/
	enum HorizontalAnchor
	{
		ANCHOR_HORIZONTAL_CENTER			=  0,
		ANCHOR_HORIZONTAL_CENTER_DYNAMIC		,
		ANCHOR_HORIZONTAL_LEFT					,
		ANCHOR_HORIZONTAL_RIGHT					,
		ANCHOR_HORIZONTAL_LEFT_RIGHT			,
		ANCHOR_HORIZONTAL_NONE
	};
	/**
	* Specifies vertical position/size relative to parent resizing.
	*/
	enum VerticalAnchor
	{
		ANCHOR_VERTICAL_CENTER			=  0,
		ANCHOR_VERTICAL_CENTER_DYNAMIC		,
		ANCHOR_VERTICAL_TOP					,
		ANCHOR_VERTICAL_BOTTOM				,
		ANCHOR_VERTICAL_TOP_BOTTOM			,
		ANCHOR_VERTICAL_NONE
	};
}

#endif
