#include "QuickGUIStringConverter.h"

namespace QuickGUI
{
	Ogre::String StringConverter::toString(BrushFilterMode m)
	{
		switch(m)
		{
		case BRUSHFILTER_NONE:										return "BRUSHFILTER_NONE";
		case BRUSHFILTER_NEAREST:									return "BRUSHFILTER_NEAREST";
		case BRUSHFILTER_LINEAR:									return "BRUSHFILTER_LINEAR";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"BrushFilterMode type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(CheckBoxEvent e)
	{
		switch(e)
		{
		case CHECKBOX_EVENT_CHECK_CHANGED:							return "CHECKBOX_EVENT_CHECK_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"CheckBoxEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ComboBoxEvent e)
	{
		switch(e)
		{
		case COMBOBOX_EVENT_SELECTION_CHANGED:						return "COMBOBOX_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ComboBoxEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ConsoleLayout l)
	{
		switch(l)
		{
		case CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM:						return "CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM";
		case CONSOLE_LAYOUT_TEXT_INPUT_TOP:							return "CONSOLE_LAYOUT_TEXT_INPUT_TOP";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ConsoleLayout type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(HorizontalAnchor a)
	{
		switch(a)
		{
		case ANCHOR_HORIZONTAL_CENTER:								return "ANCHOR_HORIZONTAL_CENTER";
		case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:						return "ANCHOR_HORIZONTAL_CENTER_DYNAMIC";
		case ANCHOR_HORIZONTAL_LEFT:								return "ANCHOR_HORIZONTAL_LEFT";
		case ANCHOR_HORIZONTAL_RIGHT:								return "ANCHOR_HORIZONTAL_RIGHT";
		case ANCHOR_HORIZONTAL_LEFT_RIGHT:							return "ANCHOR_HORIZONTAL_LEFT_RIGHT";
		case ANCHOR_HORIZONTAL_NONE:								return "ANCHOR_HORIZONTAL_NONE";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"HorizontalAnchor type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(HorizontalTextAlignment a)
	{
		switch(a)
		{
		case TEXT_ALIGNMENT_HORIZONTAL_LEFT:						return "TEXT_ALIGNMENT_HORIZONTAL_LEFT";
		case TEXT_ALIGNMENT_HORIZONTAL_RIGHT:						return "TEXT_ALIGNMENT_HORIZONTAL_RIGHT";
		case TEXT_ALIGNMENT_HORIZONTAL_CENTER:						return "TEXT_ALIGNMENT_HORIZONTAL_CENTER";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"HorizontalTextAlignment type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ListEvent e)
	{
		switch(e)
		{
		case LIST_EVENT_SELECTION_CHANGED:							return "LIST_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ListEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(MouseCursorEvent e)
	{
		switch(e)
		{
		case MOUSE_CUSSOR_EVENT_ENABLED_CHANGED:					return "MOUSE_CUSSOR_EVENT_ENABLED_CHANGED";
		case MOUSE_CURSOR_EVENT_BORDER_ENTER:						return "MOUSE_CURSOR_EVENT_BORDER_ENTER";
		case MOUSE_CURSOR_EVENT_BORDER_LEAVE:						return "MOUSE_CURSOR_EVENT_BORDER_LEAVE";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"MouseCursorEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ProgressBarEvent e)
	{
		switch(e)
		{
		case PROGRESSBAR_EVENT_PROGRESS_CHANGED:					return "PROGRESSBAR_EVENT_PROGRESS_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(VScrollBarButtonLayout l)
	{
		switch(l)
		{
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP:					return "VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP";
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN:				return "VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN";
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:				return "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH";
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP:					return "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP";
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN:				return "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN";
		case VSCROLL_BAR_BUTTON_LAYOUT_NONE:						return "VSCROLL_BAR_BUTTON_LAYOUT_NONE";
		case VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:					return "VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"VScrollBarButtonLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(HScrollBarButtonLayout l)
	{
		switch(l)
		{
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT:				return "HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT";
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT:				return "HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT";
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:				return "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH";
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT:				return "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT";
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT:				return "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT";
		case HSCROLL_BAR_BUTTON_LAYOUT_NONE:						return "HSCROLL_BAR_BUTTON_LAYOUT_NONE";
		case HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:					return "HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"HScrollBarButtonLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ProgressBarFillDirection d)
	{
		switch(d)
		{
		case PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE:				return "PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE";
		case PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE:				return "PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarFillDirection type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ProgressBarLayout l)
	{
		switch(l)
		{
		case PROGRESSBAR_LAYOUT_HORIZONTAL:							return "PROGRESSBAR_LAYOUT_HORIZONTAL";
		case PROGRESSBAR_LAYOUT_VERTICAL:							return "PROGRESSBAR_LAYOUT_VERTICAL";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ProgressBarClippingEdge e)
	{
		switch(e)
		{
		case PROGRESSBAR_CLIP_LEFT_BOTTOM:							return "PROGRESSBAR_CLIP_LEFT_BOTTOM";
		case PROGRESSBAR_CLIP_RIGHT_TOP:							return "PROGRESSBAR_CLIP_RIGHT_TOP";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarClippingEdge type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(PropertyGridEvent e)
	{
		switch(e)
		{
		case PROPERTYGRID_EVENT_SELECTION_CHANGED:					return "PROPERTYGRID_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"PropertyGridEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(RadioButtonEvent e)
	{
		switch(e)
		{
		case RADIOBUTTON_EVENT_SELECTED:							return "RADIOBUTTON_EVENT_SELECTED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"RadioButtonEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ScrollBarEvent e)
	{
		switch(e)
		{
		case SCROLLBAR_EVENT_ON_SCROLLED:							return "SCROLLBAR_EVENT_ON_SCROLLED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ScrollBarEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(TabControlEvent e)
	{
		switch(e)
		{
		case TABCONTROL_EVENT_SELECTION_CHANGED:					return "TABCONTROL_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"TabControlEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(ToolBarItemLayout l)
	{
		switch(l)
		{
		case TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE:				return "TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE";
		case TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE:				return "TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"ToolBarItemLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(TreeViewEvent e)
	{
		switch(e)
		{
		case TREEVIEW_EVENT_SELECTION_CHANGED:						return "TREEVIEW_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"TreeViewEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(TreeViewCheckBoxNodeEvent e)
	{
		switch(e)
		{
		case TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED:				return "TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"TreeViewCheckBoxNodeEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(TreeViewRadioButtonNodeEvent e)
	{
		switch(e)
		{
		case TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED:	return "TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"TreeViewRadioButtonNodeEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(VerticalAnchor a)
	{
		switch(a)
		{
		case ANCHOR_VERTICAL_BOTTOM:								return "ANCHOR_VERTICAL_BOTTOM";
		case ANCHOR_VERTICAL_CENTER:								return "ANCHOR_VERTICAL_CENTER";
		case ANCHOR_VERTICAL_CENTER_DYNAMIC:						return "ANCHOR_VERTICAL_CENTER_DYNAMIC";
		case ANCHOR_VERTICAL_NONE:									return "ANCHOR_VERTICAL_NONE";
		case ANCHOR_VERTICAL_TOP:									return "ANCHOR_VERTICAL_TOP";
		case ANCHOR_VERTICAL_TOP_BOTTOM:							return "ANCHOR_VERTICAL_TOP_BOTTOM";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"VerticalAnchor type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(VerticalTextAlignment a)
	{
		switch(a)
		{
		case TEXT_ALIGNMENT_VERTICAL_BOTTOM:						return "TEXT_ALIGNMENT_VERTICAL_BOTTOM";
		case TEXT_ALIGNMENT_VERTICAL_CENTER:						return "TEXT_ALIGNMENT_VERTICAL_CENTER";
		case TEXT_ALIGNMENT_VERTICAL_TOP:							return "TEXT_ALIGNMENT_VERTICAL_TOP";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"VerticalTextAlignment type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(WidgetEvent e)
	{
		switch(e)
		{
		case WIDGET_EVENT_CLIENTSIZE_CHANGED:						return "WIDGET_EVENT_CLIENTSIZE_CHANGED";
		case WIDGET_EVENT_ENABLED_CHANGED:							return "WIDGET_EVENT_ENABLED_CHANGED";
		case WIDGET_EVENT_POSITION_CHANGED:							return "WIDGET_EVENT_POSITION_CHANGED";
		case WIDGET_EVENT_SCROLL_CHANGED:							return "WIDGET_EVENT_SCROLL_CHANGED";
		case WIDGET_EVENT_SIZE_CHANGED:								return "WIDGET_EVENT_SIZE_CHANGED";
		case WIDGET_EVENT_SKIN_CHANGED:								return "WIDGET_EVENT_SKIN_CHANGED";
		case WIDGET_EVENT_VISIBLE_CHANGED:							return "WIDGET_EVENT_VISIBLE_CHANGED";
		case WIDGET_EVENT_CHARACTER_KEY:							return "WIDGET_EVENT_CHARACTER_KEY";
		case WIDGET_EVENT_DRAGGED:									return "WIDGET_EVENT_DRAGGED";
		case WIDGET_EVENT_DROPPED:									return "WIDGET_EVENT_DROPPED";
		case WIDGET_EVENT_KEY_DOWN:									return "WIDGET_EVENT_KEY_DOWN";
		case WIDGET_EVENT_KEY_UP:									return "WIDGET_EVENT_KEY_UP";
		case WIDGET_EVENT_KEYBOARD_INPUT_GAIN:						return "WIDGET_EVENT_KEYBOARD_INPUT_GAIN";
		case WIDGET_EVENT_KEYBOARD_INPUT_LOSE:						return "WIDGET_EVENT_KEYBOARD_INPUT_LOSE";
		case WIDGET_EVENT_MOUSE_BUTTON_DOWN:						return "WIDGET_EVENT_MOUSE_BUTTON_DOWN";
		case WIDGET_EVENT_MOUSE_BUTTON_UP:							return "WIDGET_EVENT_MOUSE_BUTTON_UP";
		case WIDGET_EVENT_MOUSE_CLICK:								return "WIDGET_EVENT_MOUSE_CLICK";
		case WIDGET_EVENT_MOUSE_CLICK_DOUBLE:						return "WIDGET_EVENT_MOUSE_CLICK_DOUBLE";
		case WIDGET_EVENT_MOUSE_CLICK_TRIPLE:						return "WIDGET_EVENT_MOUSE_CLICK_TRIPLE";
		case WIDGET_EVENT_MOUSE_ENTER:								return "WIDGET_EVENT_MOUSE_ENTER";
		case WIDGET_EVENT_MOUSE_LEAVE:								return "WIDGET_EVENT_MOUSE_LEAVE";
		case WIDGET_EVENT_MOUSE_MOVE:								return "WIDGET_EVENT_MOUSE_MOVE";
		case WIDGET_EVENT_MOUSE_WHEEL:								return "WIDGET_EVENT_MOUSE_WHEEL";
		case WIDGET_EVENT_ON_HOVER:									return "WIDGET_EVENT_ON_HOVER";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"Widget Event does not have a string equivalent!  Time to update!","StringConverter::toString");
		}
	}

	Ogre::String StringConverter::toString(WindowEvent e)
	{
		switch(e)
		{
		case WINDOW_EVENT_DRAWN:									return "WINDOW_EVENT_DRAWN";
		case WINDOW_EVENT_FOCUS_GAINED:								return "WINDOW_EVENT_FOCUS_GAINED";
		case WINDOW_EVENT_FOCUS_LOST:								return "WINDOW_EVENT_FOCUS_LOST";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"Window Event does not have a string equivalent!  Time to update!","StringConverter::toString");
		}
	}

	BrushFilterMode StringConverter::parseBrushFilterMode(const Ogre::String& s)
	{
		if(s == "BRUSHFILTER_NONE")									return BRUSHFILTER_NONE;
		else if(s == "BRUSHFILTER_NEAREST")							return BRUSHFILTER_NEAREST;
		else if(s == "BRUSHFILTER_LINEAR")							return BRUSHFILTER_LINEAR;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized BrushFilterMode type! (Possible need to update?)","StringConverter::parseBrushFilterMode");
	}

	CheckBoxEvent StringConverter::parseCheckBoxEvent(const Ogre::String& s)
	{
		if(s == "CHECKBOX_EVENT_CHECK_CHANGED")						return CHECKBOX_EVENT_CHECK_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized CheckBoxEvent type! (Possible need to update?)","StringConverter::parseCheckBoxEvent");
	}

	ComboBoxEvent StringConverter::parseComboBoxEvent(const Ogre::String& s)
	{
		if(s == "COMBOBOX_EVENT_SELECTION_CHANGED")					return COMBOBOX_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ComboBoxEvent type! (Possible need to update?)","StringConverter::parseComboBoxEvent");
	}

	ConsoleLayout StringConverter::parseConsoleLayout(const Ogre::String& s)
	{
		if(s == "CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM")					return CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM;
		else if(s == "CONSOLE_LAYOUT_TEXT_INPUT_TOP")				return CONSOLE_LAYOUT_TEXT_INPUT_TOP;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ConsoleLayout type! (Possible need to update?)","StringConverter::parseConsoleLayout");
	}

	HorizontalAnchor StringConverter::parseHorizontalAnchor(const Ogre::String& s)
	{
		if(s == "ANCHOR_HORIZONTAL_CENTER")							return ANCHOR_HORIZONTAL_CENTER;
		else if(s == "ANCHOR_HORIZONTAL_CENTER_DYNAMIC")			return ANCHOR_HORIZONTAL_CENTER_DYNAMIC;
		else if(s == "ANCHOR_HORIZONTAL_LEFT")						return ANCHOR_HORIZONTAL_LEFT;
		else if(s == "ANCHOR_HORIZONTAL_RIGHT")						return ANCHOR_HORIZONTAL_RIGHT;
		else if(s == "ANCHOR_HORIZONTAL_LEFT_RIGHT")				return ANCHOR_HORIZONTAL_LEFT_RIGHT;
		else if(s == "ANCHOR_HORIZONTAL_NONE")						return ANCHOR_HORIZONTAL_NONE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized HorizontalAnchor type! (Possible need to update?)","StringConverter::parseHorizontalAnchor");
	}

	HorizontalTextAlignment StringConverter::parseHorizontalTextAlignment(const Ogre::String& s)
	{
		if(s == "TEXT_ALIGNMENT_HORIZONTAL_LEFT")					return TEXT_ALIGNMENT_HORIZONTAL_LEFT;
		else if(s == "TEXT_ALIGNMENT_HORIZONTAL_RIGHT")				return TEXT_ALIGNMENT_HORIZONTAL_RIGHT;
		else if(s == "TEXT_ALIGNMENT_HORIZONTAL_CENTER")			return TEXT_ALIGNMENT_HORIZONTAL_CENTER;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized HorizontalTextAlignment type! (Possible need to update?)","StringConverter::parseHorizontalTextAlignment");
	}

	VScrollBarButtonLayout StringConverter::parseVScrollBarButtonLayout(const Ogre::String& s)
	{
		if(s == "VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP")			return VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN")		return VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH")		return VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP")		return VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN")		return VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_NONE")				return VSCROLL_BAR_BUTTON_LAYOUT_NONE;
		else if(s == "VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE")			return VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized parseVScrollBarButtonLayout type! (Possible need to update?)","StringConverter::parseVScrollBarButtonLayout");
	}

	HScrollBarButtonLayout StringConverter::parseHScrollBarButtonLayout(const Ogre::String& s)
	{
		if(s == "HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT")			return HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT")	return HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH")		return HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT")		return HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT")	return HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_NONE")				return HSCROLL_BAR_BUTTON_LAYOUT_NONE;
		else if(s == "HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE")			return HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized parseHScrollBarButtonLayout type! (Possible need to update?)","StringConverter::parseHScrollBarButtonLayout");
	}

	ListEvent StringConverter::parseListEvent(const Ogre::String& s)
	{
		if(s == "LIST_EVENT_SELECTION_CHANGED")						return LIST_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ListEvent type! (Possible need to update?)","StringConverter::parseListEvent");
	}

	MouseCursorEvent StringConverter::parseMouseCursorEvent(const Ogre::String& s)
	{
		if(s == "MOUSE_CUSSOR_EVENT_ENABLED_CHANGED")				return MOUSE_CUSSOR_EVENT_ENABLED_CHANGED;
		else if(s == "MOUSE_CURSOR_EVENT_BORDER_ENTER")				return MOUSE_CURSOR_EVENT_BORDER_ENTER;
		else if(s == "MOUSE_CURSOR_EVENT_BORDER_LEAVE")				return MOUSE_CURSOR_EVENT_BORDER_LEAVE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized MouseCursorEvent type! (Possible need to update?)","StringConverter::parseMouseCursorEvent");
	}

	ProgressBarEvent StringConverter::parseProgressBarEvent(const Ogre::String& s)
	{
		if(s == "PROGRESSBAR_EVENT_PROGRESS_CHANGED")				return PROGRESSBAR_EVENT_PROGRESS_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarEvent type! (Possible need to update?)","StringConverter::parseProgressBarEvent");
	}

	ProgressBarFillDirection StringConverter::parseProgressBarFillDirection(const Ogre::String& s)
	{
		if(s == "PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE")			return PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE;
		else if(s == "PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE")		return PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarFillDirection type! (Possible need to update?)","StringConverter::parseProgressBarFillDirection");
	}
	
	ProgressBarLayout StringConverter::parseProgressBarLayout(const Ogre::String& s)
	{
		if(s == "PROGRESSBAR_LAYOUT_HORIZONTAL")					return PROGRESSBAR_LAYOUT_HORIZONTAL;
		else if(s == "PROGRESSBAR_LAYOUT_VERTICAL")					return PROGRESSBAR_LAYOUT_VERTICAL;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarLayout type! (Possible need to update?)","StringConverter::parseProgressBarLayout");
	}

	ProgressBarClippingEdge StringConverter::parseProgressBarClippingEdge(const Ogre::String& s)
	{
		if(s == "PROGRESSBAR_CLIP_LEFT_BOTTOM")						return PROGRESSBAR_CLIP_LEFT_BOTTOM;
		else if(s == "PROGRESSBAR_CLIP_RIGHT_TOP")					return PROGRESSBAR_CLIP_RIGHT_TOP;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarClippingEdge type! (Possible need to update?)","StringConverter::parseProgressBarClippingEdge");
	}

	PropertyGridEvent StringConverter::parsePropertyGridEvent(const Ogre::String& s)
	{
		if(s == "PROPERTYGRID_EVENT_SELECTION_CHANGED")				return PROPERTYGRID_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized PropertyGridEvent type! (Possible need to update?)","StringConverter::parsePropertyGridEvent");
	}

	RadioButtonEvent StringConverter::parseRadioButtonEvent(const Ogre::String& s)
	{
		if(s == "RADIOBUTTON_EVENT_SELECTED")						return RADIOBUTTON_EVENT_SELECTED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized RadioButtonEvent type! (Possible need to update?)","StringConverter::parseRadioButtonEvent");
	}

	ScrollBarEvent StringConverter::parseScrollBarEvent(const Ogre::String& s)
	{
		if(s == "SCROLLBAR_EVENT_ON_SCROLLED")						return SCROLLBAR_EVENT_ON_SCROLLED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ScrollBarEvent type! (Possible need to update?)","StringConverter::parseScrollBarEvent");
	}

	TabControlEvent StringConverter::parseTabControlEvent(const Ogre::String& s)
	{
		if(s == "TABCONTROL_EVENT_SELECTION_CHANGED")				return TABCONTROL_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TabControlEvent type! (Possible need to update?)","StringConverter::parseTabControlEvent");
	}

	ToolBarItemLayout StringConverter::parseToolBarItemLayout(const Ogre::String& s)
	{
		if(s == "TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE")			return TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE;
		else if(s == "TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE")	return TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ToolBarItemLayout type! (Possible need to update?)","StringConverter::parseToolBarItemLayout");
	}

	TreeViewEvent StringConverter::parseTreeViewEvent(const Ogre::String& s)
	{
		if(s == "TREEVIEW_EVENT_SELECTION_CHANGED")					return TREEVIEW_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TreeViewEvent type! (Possible need to update?)","StringConverter::parseTreeViewEvent");
	}

	TreeViewCheckBoxNodeEvent StringConverter::parseTreeViewCheckBoxNodeEvent(const Ogre::String& s)
	{
		if(s == "TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED")			return TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TreeViewCheckBoxNodeEvent type! (Possible need to update?)","StringConverter::parseTreeViewCheckBoxNodeEvent");
	}

	TreeViewRadioButtonNodeEvent StringConverter::parseTreeViewRadioButtonNodeEvent(const Ogre::String& s)
	{
		if(s == "TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED") return TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TreeViewRadioButtonNodeEvent type! (Possible need to update?)","StringConverter::parseTreeViewRadioButtonNodeEvent");
	}

	VerticalAnchor StringConverter::parseVerticalAnchor(const Ogre::String& s)
	{
		if(s == "ANCHOR_VERTICAL_BOTTOM")							return ANCHOR_VERTICAL_BOTTOM;
		else if(s == "ANCHOR_VERTICAL_CENTER")						return ANCHOR_VERTICAL_CENTER;
		else if(s == "ANCHOR_VERTICAL_CENTER_DYNAMIC")				return ANCHOR_VERTICAL_CENTER_DYNAMIC;
		else if(s == "ANCHOR_VERTICAL_NONE")						return ANCHOR_VERTICAL_NONE;
		else if(s == "ANCHOR_VERTICAL_TOP")							return ANCHOR_VERTICAL_TOP;
		else if(s == "ANCHOR_VERTICAL_TOP_BOTTOM")					return ANCHOR_VERTICAL_TOP_BOTTOM;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized VerticalAnchor type! (Possible need to update?)","StringConverter::parseVerticalAnchor");
	}

	VerticalTextAlignment StringConverter::parseVerticalTextAlignment(const Ogre::String& s)
	{
		if(s == "TEXT_ALIGNMENT_VERTICAL_BOTTOM")					return TEXT_ALIGNMENT_VERTICAL_BOTTOM;
		else if(s == "TEXT_ALIGNMENT_VERTICAL_CENTER")				return TEXT_ALIGNMENT_VERTICAL_CENTER;
		else if(s == "TEXT_ALIGNMENT_VERTICAL_TOP")					return TEXT_ALIGNMENT_VERTICAL_TOP;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized VerticalTextAlignment type! (Possible need to update?)","StringConverter::parseVerticalTextAlignment");
	}

	WidgetEvent StringConverter::parseWidgetEvent(const Ogre::String& s)
	{
		if(s == "WIDGET_EVENT_CLIENTSIZE_CHANGED")					return WIDGET_EVENT_CLIENTSIZE_CHANGED;
		else if(s == "WIDGET_EVENT_ENABLED_CHANGED")				return WIDGET_EVENT_ENABLED_CHANGED;
		else if(s == "WIDGET_EVENT_POSITION_CHANGED")				return WIDGET_EVENT_POSITION_CHANGED;
		else if(s == "WIDGET_EVENT_SCROLL_CHANGED")					return WIDGET_EVENT_SCROLL_CHANGED;
		else if(s == "WIDGET_EVENT_SIZE_CHANGED")					return WIDGET_EVENT_SIZE_CHANGED;
		else if(s == "WIDGET_EVENT_SKIN_CHANGED")					return WIDGET_EVENT_SKIN_CHANGED;
		else if(s == "WIDGET_EVENT_VISIBLE_CHANGED")				return WIDGET_EVENT_VISIBLE_CHANGED;
		else if(s == "WIDGET_EVENT_CHARACTER_KEY")					return WIDGET_EVENT_CHARACTER_KEY;
		else if(s == "WIDGET_EVENT_DRAGGED")						return WIDGET_EVENT_DRAGGED;
		else if(s == "WIDGET_EVENT_DROPPED")						return WIDGET_EVENT_DROPPED;
		else if(s == "WIDGET_EVENT_KEY_DOWN")						return WIDGET_EVENT_KEY_DOWN;
		else if(s == "WIDGET_EVENT_KEY_UP")							return WIDGET_EVENT_KEY_UP;
		else if(s == "WIDGET_EVENT_KEYBOARD_INPUT_GAIN")			return WIDGET_EVENT_KEYBOARD_INPUT_GAIN;
		else if(s == "WIDGET_EVENT_KEYBOARD_INPUT_LOSE")			return WIDGET_EVENT_KEYBOARD_INPUT_LOSE;
		else if(s == "WIDGET_EVENT_MOUSE_BUTTON_DOWN")				return WIDGET_EVENT_MOUSE_BUTTON_DOWN;
		else if(s == "WIDGET_EVENT_MOUSE_BUTTON_UP")				return WIDGET_EVENT_MOUSE_BUTTON_UP;
		else if(s == "WIDGET_EVENT_MOUSE_CLICK")					return WIDGET_EVENT_MOUSE_CLICK;
		else if(s == "WIDGET_EVENT_MOUSE_CLICK_DOUBLE")				return WIDGET_EVENT_MOUSE_CLICK_DOUBLE;
		else if(s == "WIDGET_EVENT_MOUSE_CLICK_TRIPLE")				return WIDGET_EVENT_MOUSE_CLICK_TRIPLE;
		else if(s == "WIDGET_EVENT_MOUSE_ENTER")					return WIDGET_EVENT_MOUSE_ENTER;
		else if(s == "WIDGET_EVENT_MOUSE_LEAVE")					return WIDGET_EVENT_MOUSE_LEAVE;
		else if(s == "WIDGET_EVENT_MOUSE_MOVE")						return WIDGET_EVENT_MOUSE_MOVE;
		else if(s == "WIDGET_EVENT_MOUSE_WHEEL")					return WIDGET_EVENT_MOUSE_WHEEL;
		else if(s == "WIDGET_EVENT_ON_HOVER")						return WIDGET_EVENT_ON_HOVER;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized Widget Event! (Possible need to update?)","StringConverter::parseWidgetEvent");
	}

	WindowEvent StringConverter::parseWindowEvent(const Ogre::String& s)
	{
		if(s == "WINDOW_EVENT_DRAWN")								return WINDOW_EVENT_DRAWN;
		else if(s == "WINDOW_EVENT_FOCUS_GAINED")					return WINDOW_EVENT_FOCUS_GAINED;
		else if(s == "WINDOW_EVENT_FOCUS_LOST")						return WINDOW_EVENT_FOCUS_LOST;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized Winhdow Event! (Possible need to update?)","StringConverter::parseWindowEvent");
	}
}
