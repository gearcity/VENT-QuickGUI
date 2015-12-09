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
