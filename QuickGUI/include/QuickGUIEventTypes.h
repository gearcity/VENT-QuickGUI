#ifndef QUICKGUIEVENTTYPES_H
#define QUICKGUIEVENTTYPES_H

namespace QuickGUI
{
	enum CheckBoxEvent
	{
		// Property type events
		// Input type events
		// Other type events
		CHECKBOX_EVENT_CHECK_CHANGED	=  0,
		CHECKBOX_EVENT_COUNT
	};

	/**
	* List Events
	*/
	enum ComboBoxEvent
	{
		// Property type events
		// Input type events
		// Other type events
		COMBOBOX_EVENT_SELECTION_CHANGED	=  0,
		COMBOBOX_EVENT_COUNT
	};

	/**
	* List Events
	*/
	enum ListEvent
	{
		// Property type events
		// Input type events
		// Other type events
		LIST_EVENT_SELECTION_CHANGED	=  0,
		LIST_EVENT_COUNT
	};

	/**
	* Mouse Cursor Events
	*/
	enum MouseCursorEvent
	{
		MOUSE_CUSSOR_EVENT_ENABLED_CHANGED	 = 0,
		MOUSE_CURSOR_EVENT_BORDER_ENTER			,
		MOUSE_CURSOR_EVENT_BORDER_LEAVE			,
		MOUSE_CURSOR_EVENT_COUNT
	};

	/**
	* ProgressBar Events
	*/
	enum ProgressBarEvent
	{
		// Property type events
		PROGRESSBAR_EVENT_PROGRESS_CHANGED	 = 0,
		// Input type events
		// Other type events
		PROGRESSBAR_EVENT_COUNT
	};

	/**
	* PropertyGrid Events
	*/
	enum PropertyGridEvent
	{
		// Property type events
		// Input type events
		// Other type events
		PROPERTYGRID_EVENT_SELECTION_CHANGED	,
		PROPERTYGRID_EVENT_COUNT
	};

	enum RadioButtonEvent
	{
		// Property type events
		// Input type events
		// Other type events
		RADIOBUTTON_EVENT_SELECTED	=  0,
		RADIOBUTTON_EVENT_COUNT
	};

	/**
	* ScrollBar Events
	*/
	enum ScrollBarEvent
	{
		// Property type events
		// Input type events
		// Other type events
		SCROLLBAR_EVENT_ON_SCROLLED			,
		SCROLLBAR_EVENT_COUNT
	};

	/**
	* TabControl Events
	*/
	enum TabControlEvent
	{
		// Property type events
		// Input type events
		// Other type events
		TABCONTROL_EVENT_SELECTION_CHANGED	=  0,
		TABCONTROL_EVENT_COUNT
	};

	/**
	* TreeView Events
	*/
	enum TreeViewEvent
	{
		// Property type events
		// Input type events
		// Other type events
		TREEVIEW_EVENT_SELECTION_CHANGED	,
		TREEVIEW_EVENT_COUNT
	};

	/**
	* TreeViewCheckBoxNode Events
	*/
	enum TreeViewCheckBoxNodeEvent
	{
		// Property type events
		// Input type events
		// Other type events
		TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED	,
		TREEVIEWCHECKBOXNODE_EVENT_COUNT
	};

	/**
	* TreeViewCheckBoxNode Events
	*/
	enum TreeViewRadioButtonNodeEvent
	{
		// Property type events
		// Input type events
		// Other type events
		TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED	,
		TREEVIEWRADIOBUTTONNODE_EVENT_COUNT
	};

	/**
	* All widgets must support these events
	*/
	enum WidgetEvent
	{
		// Property type events
		WIDGET_EVENT_CLIENTSIZE_CHANGED =  0,
		WIDGET_EVENT_ENABLED_CHANGED		,
		WIDGET_EVENT_POSITION_CHANGED		,
		WIDGET_EVENT_SCROLL_CHANGED			,
		WIDGET_EVENT_SIZE_CHANGED			,
		WIDGET_EVENT_SKIN_CHANGED			,
		WIDGET_EVENT_VISIBLE_CHANGED		,
		// Input type events
		WIDGET_EVENT_CHARACTER_KEY			,
		WIDGET_EVENT_DRAGGED				,
		WIDGET_EVENT_DROPPED				,
		WIDGET_EVENT_KEY_DOWN				,
		WIDGET_EVENT_KEY_UP					,
		WIDGET_EVENT_KEYBOARD_INPUT_GAIN	,
		WIDGET_EVENT_KEYBOARD_INPUT_LOSE	,
		WIDGET_EVENT_MOUSE_BUTTON_DOWN		,
		WIDGET_EVENT_MOUSE_BUTTON_UP		,
		WIDGET_EVENT_MOUSE_CLICK			,
		WIDGET_EVENT_MOUSE_CLICK_DOUBLE		,
		WIDGET_EVENT_MOUSE_CLICK_TRIPLE		,
		WIDGET_EVENT_MOUSE_ENTER			,
		WIDGET_EVENT_MOUSE_LEAVE			,
		WIDGET_EVENT_MOUSE_MOVE				,
		WIDGET_EVENT_MOUSE_WHEEL			,
		WIDGET_EVENT_ON_HOVER				,
		// Other type events
		WIDGET_EVENT_COUNT
	};

	/**
	* Window Events
	*/
	enum WindowEvent
	{
		// Property type events
		// Input type events
		// Other type events
		WINDOW_EVENT_DRAWN					,
		WINDOW_EVENT_FOCUS_LOST				,
		WINDOW_EVENT_FOCUS_GAINED			,
		WINDOW_EVENT_COUNT
	};
}

#endif
