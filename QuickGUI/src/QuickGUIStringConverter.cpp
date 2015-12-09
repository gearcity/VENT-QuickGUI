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

#include "QuickGUIStringConverter.h"

#include "OgreCommon.h"
#include "OgreStringConverter.h"

#if OGRE_VERSION_MINOR < 7
#include <OgreMemorySTLAllocator.h>
#include <OgreMemoryAllocatorConfig.h>
template <typename T, typename A = Ogre::STLAllocator<T, Ogre::GeneralAllocPolicy> > 
	struct vector 
	{ 
#if OGRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
		typedef typename std::vector<T, A> type;    
#else
		typedef typename std::vector<T> type;    
#endif // OGRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
	}; 
#else // OGRE_VERSION_MINOR < 7
using Ogre::vector;
#endif // OGRE_VERSION_MINOR < 7

namespace QuickGUI
{
	std::string StringConverter::toString(int i)
	{
		return Ogre::StringConverter::toString(i);
	}

	std::string StringConverter::toString(BrushFilterMode m)
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

	std::string StringConverter::toString(CheckBoxEvent e)
	{
		switch(e)
		{
		case CHECKBOX_EVENT_CHECK_CHANGED:							return "CHECKBOX_EVENT_CHECK_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"CheckBoxEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(ColourValue c)
	{
		return (Ogre::StringConverter::toString(c.a) + " " + 
			Ogre::StringConverter::toString(c.b) + " " +
			Ogre::StringConverter::toString(c.g) + " " +
			Ogre::StringConverter::toString(c.r)
			);
	}

	std::string StringConverter::toString(ComboBoxEvent e)
	{
		switch(e)
		{
		case COMBOBOX_EVENT_SELECTION_CHANGED:						return "COMBOBOX_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ComboBoxEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(ConsoleLayout l)
	{
		switch(l)
		{
		case CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM:						return "CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM";
		case CONSOLE_LAYOUT_TEXT_INPUT_TOP:							return "CONSOLE_LAYOUT_TEXT_INPUT_TOP";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ConsoleLayout type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(HorizontalAnchor a)
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

	std::string StringConverter::toString(HorizontalTextAlignment a)
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

	std::string StringConverter::toString(ListEvent e)
	{
		switch(e)
		{
		case LIST_EVENT_SELECTION_CHANGED:							return "LIST_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ListEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(MouseEvent e)
	{
		switch(e)
		{
		case MOUSE_EVENT_BUTTON_CLICK:								return "MOUSE_EVENT_BUTTON_CLICK";
		case MOUSE_EVENT_BUTTON_DOUBLE_CLICK:						return "MOUSE_EVENT_BUTTON_DOUBLE_CLICK";
		case MOUSE_EVENT_BUTTON_DOWN:								return "MOUSE_EVENT_BUTTON_DOWN";
		case MOUSE_EVENT_BUTTON_TRIPLE_CLICK:						return "MOUSE_EVENT_BUTTON_TRIPLE_CLICK";
		case MOUSE_EVENT_BUTTON_UP:									return "MOUSE_EVENT_BUTTON_UP";
		case MOUSE_EVENT_MOVE:										return "MOUSE_EVENT_MOVE";
		case MOUSE_EVENT_SCROLL:									return "MOUSE_EVENT_SCROLL";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"MouseEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(Point p)
	{
		return (Ogre::StringConverter::toString(p.x) + " " + Ogre::StringConverter::toString(p.y));
	}

	std::string StringConverter::toString(ProgressBarEvent e)
	{
		switch(e)
		{
		case PROGRESSBAR_EVENT_PROGRESS_CHANGED:					return "PROGRESSBAR_EVENT_PROGRESS_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(VScrollBarButtonLayout l)
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

	std::string StringConverter::toString(HScrollBarButtonLayout l)
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

	std::string StringConverter::toString(ProgressBarFillDirection d)
	{
		switch(d)
		{
		case PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE:				return "PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE";
		case PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE:				return "PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarFillDirection type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(ProgressBarLayout l)
	{
		switch(l)
		{
		case PROGRESSBAR_LAYOUT_HORIZONTAL:							return "PROGRESSBAR_LAYOUT_HORIZONTAL";
		case PROGRESSBAR_LAYOUT_VERTICAL:							return "PROGRESSBAR_LAYOUT_VERTICAL";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(ProgressBarClippingEdge e)
	{
		switch(e)
		{
		case PROGRESSBAR_CLIP_LEFT_BOTTOM:							return "PROGRESSBAR_CLIP_LEFT_BOTTOM";
		case PROGRESSBAR_CLIP_RIGHT_TOP:							return "PROGRESSBAR_CLIP_RIGHT_TOP";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ProgressBarClippingEdge type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(PropertyGridEvent e)
	{
		switch(e)
		{
		case PROPERTYGRID_EVENT_SELECTION_CHANGED:					return "PROPERTYGRID_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"PropertyGridEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(RadioButtonEvent e)
	{
		switch(e)
		{
		case RADIOBUTTON_EVENT_SELECTED:							return "RADIOBUTTON_EVENT_SELECTED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"RadioButtonEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(Rect r)
	{
		return (Ogre::StringConverter::toString(r.position.x) + " " + 
			Ogre::StringConverter::toString(r.position.y) + " " +
			Ogre::StringConverter::toString(r.size.width) + " " +
			Ogre::StringConverter::toString(r.size.height)
			);
	}

	std::string StringConverter::toString(ScrollBarEvent e)
	{
		switch(e)
		{
		case SCROLLBAR_EVENT_ON_SCROLLED:							return "SCROLLBAR_EVENT_ON_SCROLLED";
		case SCROLLBAR_EVENT_SLIDER_DRAGGED:						return "SCROLLBAR_EVENT_SLIDER_DRAGGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"ScrollBarEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(SheetEvent e)
	{
		switch(e)
		{
		case SHEET_EVENT_MOUSE_CURSOR_ENTER_SHEET_BORDER:			return "SHEET_EVENT_MOUSE_CURSOR_ENTER_SHEET_BORDER";
		case SHEET_EVENT_MOUSE_CURSOR_LEAVE_SHEET_BORDER:			return "SHEET_EVENT_MOUSE_CURSOR_LEAVE_SHEET_BORDER";
		case SHEET_EVENT_MOUSE_CURSOR_SKIN_CHANGED:					return "SHEET_EVENT_MOUSE_CURSOR_SKIN_CHANGED";
		case SHEET_EVENT_MOUSE_CURSOR_VISIBILE_CHANGED:				return "SHEET_EVENT_MOUSE_CURSOR_VISIBILE_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"SheetEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(Size s)
	{
		return (Ogre::StringConverter::toString(s.width) + " " + Ogre::StringConverter::toString(s.height));
	}

	std::string StringConverter::toString(TabControlEvent e)
	{
		switch(e)
		{
		case TABCONTROL_EVENT_SELECTION_CHANGED:					return "TABCONTROL_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"TabControlEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(ToolBarItemLayout l)
	{
		switch(l)
		{
		case TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE:				return "TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE";
		case TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE:				return "TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE";
		default: 
			throw Exception(Exception::ERR_SERIALIZATION,"ToolBarItemLayout type does not have a string equivalent!  Check if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(TreeViewEvent e)
	{
		switch(e)
		{
		case TREEVIEW_EVENT_SELECTION_CHANGED:						return "TREEVIEW_EVENT_SELECTION_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"TreeViewEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(TreeViewCheckBoxNodeEvent e)
	{
		switch(e)
		{
		case TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED:				return "TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"TreeViewCheckBoxNodeEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(TreeViewRadioButtonNodeEvent e)
	{
		switch(e)
		{
		case TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED:	return "TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED";
		default:
			throw Exception(Exception::ERR_SERIALIZATION,"TreeViewRadioButtonNodeEvent type does not have a string equivalent! Chick if update is needed!","StringConverter::toString");
		}
	}

	std::string StringConverter::toString(VerticalAnchor a)
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

	std::string StringConverter::toString(VerticalTextAlignment a)
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

	std::string StringConverter::toString(WidgetEvent e)
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

	std::string StringConverter::toString(WindowEvent e)
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

	BrushFilterMode StringConverter::parseBrushFilterMode(const std::string& s)
	{
		if(s == "BRUSHFILTER_NONE")									return BRUSHFILTER_NONE;
		else if(s == "BRUSHFILTER_NEAREST")							return BRUSHFILTER_NEAREST;
		else if(s == "BRUSHFILTER_LINEAR")							return BRUSHFILTER_LINEAR;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized BrushFilterMode type! (Possible need to update?)","StringConverter::parseBrushFilterMode");
	}

	CheckBoxEvent StringConverter::parseCheckBoxEvent(const std::string& s)
	{
		if(s == "CHECKBOX_EVENT_CHECK_CHANGED")						return CHECKBOX_EVENT_CHECK_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized CheckBoxEvent type! (Possible need to update?)","StringConverter::parseCheckBoxEvent");
	}

	ColourValue StringConverter::parseColourValue(const std::string& s)
	{
		Ogre::String ogreString(s);
		vector<Ogre::String>::type vs = Ogre::StringUtil::split(ogreString," ");

		ColourValue cv;

		int vsSize = static_cast<int>(vs.size());
		
		if(vsSize > 0)
			cv.a = Ogre::StringConverter::parseReal(vs[0]);
		if(vsSize > 1)
			cv.b = Ogre::StringConverter::parseReal(vs[1]);
		if(vsSize > 2)
			cv.g = Ogre::StringConverter::parseReal(vs[2]);
		if(vsSize > 3)
			cv.r = Ogre::StringConverter::parseReal(vs[3]);

		return cv;
	}

	ComboBoxEvent StringConverter::parseComboBoxEvent(const std::string& s)
	{
		if(s == "COMBOBOX_EVENT_SELECTION_CHANGED")					return COMBOBOX_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ComboBoxEvent type! (Possible need to update?)","StringConverter::parseComboBoxEvent");
	}

	ConsoleLayout StringConverter::parseConsoleLayout(const std::string& s)
	{
		if(s == "CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM")					return CONSOLE_LAYOUT_TEXT_INPUT_BOTTOM;
		else if(s == "CONSOLE_LAYOUT_TEXT_INPUT_TOP")				return CONSOLE_LAYOUT_TEXT_INPUT_TOP;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ConsoleLayout type! (Possible need to update?)","StringConverter::parseConsoleLayout");
	}

	HorizontalAnchor StringConverter::parseHorizontalAnchor(const std::string& s)
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

	HorizontalTextAlignment StringConverter::parseHorizontalTextAlignment(const std::string& s)
	{
		if(s == "TEXT_ALIGNMENT_HORIZONTAL_LEFT")					return TEXT_ALIGNMENT_HORIZONTAL_LEFT;
		else if(s == "TEXT_ALIGNMENT_HORIZONTAL_RIGHT")				return TEXT_ALIGNMENT_HORIZONTAL_RIGHT;
		else if(s == "TEXT_ALIGNMENT_HORIZONTAL_CENTER")			return TEXT_ALIGNMENT_HORIZONTAL_CENTER;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized HorizontalTextAlignment type! (Possible need to update?)","StringConverter::parseHorizontalTextAlignment");
	}

	int StringConverter::parseInt(const std::string& s)
	{
		return Ogre::StringConverter::parseInt(s);
	}

	VScrollBarButtonLayout StringConverter::parseVScrollBarButtonLayout(const std::string& s)
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

	HScrollBarButtonLayout StringConverter::parseHScrollBarButtonLayout(const std::string& s)
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

	ListEvent StringConverter::parseListEvent(const std::string& s)
	{
		if(s == "LIST_EVENT_SELECTION_CHANGED")						return LIST_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ListEvent type! (Possible need to update?)","StringConverter::parseListEvent");
	}

	MouseEvent StringConverter::parseMouseEvent(const std::string& s)
	{
		if(s == "MOUSE_EVENT_BUTTON_CLICK")							return MOUSE_EVENT_BUTTON_CLICK;
		else if(s == "MOUSE_EVENT_BUTTON_DOUBLE_CLICK")				return MOUSE_EVENT_BUTTON_DOUBLE_CLICK;
		else if(s == "MOUSE_EVENT_BUTTON_DOWN")						return MOUSE_EVENT_BUTTON_DOWN;
		else if(s == "MOUSE_EVENT_BUTTON_TRIPLE_CLICK")				return MOUSE_EVENT_BUTTON_TRIPLE_CLICK;
		else if(s == "MOUSE_EVENT_BUTTON_UP")						return MOUSE_EVENT_BUTTON_UP;
		else if(s == "MOUSE_EVENT_MOVE")							return MOUSE_EVENT_MOVE;
		else if(s == "MOUSE_EVENT_SCROLL")							return MOUSE_EVENT_SCROLL;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized MouseEvent type! (Possible need to update?)","StringConverter::parseMouseEvent");
	}

	Point StringConverter::parsePoint(const std::string& s)
	{
		Ogre::String ogreString(s);
		vector<Ogre::String>::type vs = Ogre::StringUtil::split(ogreString," ");

		Point p;

		int vsSize = static_cast<int>(vs.size());

		if(vsSize > 0)
			p.x = Ogre::StringConverter::parseReal(vs[0]);
		if(vsSize > 1)
			p.y = Ogre::StringConverter::parseReal(vs[1]);

		return p;
	}

	ProgressBarEvent StringConverter::parseProgressBarEvent(const std::string& s)
	{
		if(s == "PROGRESSBAR_EVENT_PROGRESS_CHANGED")				return PROGRESSBAR_EVENT_PROGRESS_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarEvent type! (Possible need to update?)","StringConverter::parseProgressBarEvent");
	}

	ProgressBarFillDirection StringConverter::parseProgressBarFillDirection(const std::string& s)
	{
		if(s == "PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE")			return PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE;
		else if(s == "PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE")		return PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarFillDirection type! (Possible need to update?)","StringConverter::parseProgressBarFillDirection");
	}
	
	ProgressBarLayout StringConverter::parseProgressBarLayout(const std::string& s)
	{
		if(s == "PROGRESSBAR_LAYOUT_HORIZONTAL")					return PROGRESSBAR_LAYOUT_HORIZONTAL;
		else if(s == "PROGRESSBAR_LAYOUT_VERTICAL")					return PROGRESSBAR_LAYOUT_VERTICAL;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarLayout type! (Possible need to update?)","StringConverter::parseProgressBarLayout");
	}

	ProgressBarClippingEdge StringConverter::parseProgressBarClippingEdge(const std::string& s)
	{
		if(s == "PROGRESSBAR_CLIP_LEFT_BOTTOM")						return PROGRESSBAR_CLIP_LEFT_BOTTOM;
		else if(s == "PROGRESSBAR_CLIP_RIGHT_TOP")					return PROGRESSBAR_CLIP_RIGHT_TOP;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ProgressBarClippingEdge type! (Possible need to update?)","StringConverter::parseProgressBarClippingEdge");
	}

	PropertyGridEvent StringConverter::parsePropertyGridEvent(const std::string& s)
	{
		if(s == "PROPERTYGRID_EVENT_SELECTION_CHANGED")				return PROPERTYGRID_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized PropertyGridEvent type! (Possible need to update?)","StringConverter::parsePropertyGridEvent");
	}

	RadioButtonEvent StringConverter::parseRadioButtonEvent(const std::string& s)
	{
		if(s == "RADIOBUTTON_EVENT_SELECTED")						return RADIOBUTTON_EVENT_SELECTED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized RadioButtonEvent type! (Possible need to update?)","StringConverter::parseRadioButtonEvent");
	}

	Rect StringConverter::parseRect(const std::string& s)
	{
		Ogre::String ogreString(s);
		vector<Ogre::String>::type vs = Ogre::StringUtil::split(ogreString," ");

		Rect r;

		int vsSize = static_cast<int>(vs.size());
		
		if(vsSize > 0)
			r.position.x = Ogre::StringConverter::parseReal(vs[0]);
		if(vsSize > 1)
			r.position.y = Ogre::StringConverter::parseReal(vs[1]);
		if(vsSize > 2)
			r.size.width = Ogre::StringConverter::parseReal(vs[2]);
		if(vsSize > 3)
			r.size.height = Ogre::StringConverter::parseReal(vs[3]);

		return r;
	}

	ScrollBarEvent StringConverter::parseScrollBarEvent(const std::string& s)
	{
		if(s == "SCROLLBAR_EVENT_ON_SCROLLED")						return SCROLLBAR_EVENT_ON_SCROLLED;
		else if(s == "SCROLLBAR_EVENT_SLIDER_DRAGGED")				return SCROLLBAR_EVENT_SLIDER_DRAGGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ScrollBarEvent type! (Possible need to update?)","StringConverter::parseScrollBarEvent");
	}

	SheetEvent StringConverter::parseSheetEvent(const std::string& s)
	{
		if(s == "SHEET_EVENT_MOUSE_CURSOR_ENTER_SHEET_BORDER")		return SHEET_EVENT_MOUSE_CURSOR_ENTER_SHEET_BORDER;
		else if(s == "SHEET_EVENT_MOUSE_CURSOR_LEAVE_SHEET_BORDER")	return SHEET_EVENT_MOUSE_CURSOR_LEAVE_SHEET_BORDER;
		else if(s == "SHEET_EVENT_MOUSE_CURSOR_SKIN_CHANGED")		return SHEET_EVENT_MOUSE_CURSOR_SKIN_CHANGED;
		else if(s == "SHEET_EVENT_MOUSE_CURSOR_VISIBILE_CHANGED")	return SHEET_EVENT_MOUSE_CURSOR_VISIBILE_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized SheetEvent type! (Possible need to update?)","StringConverter::parseSheetEvent");
	}

	Size StringConverter::parseSize(const std::string& s)
	{
		Ogre::String ogreString(s);
		vector<Ogre::String>::type vs = Ogre::StringUtil::split(ogreString," ");

		Size size;

		int vsSize = static_cast<int>(vs.size());

		if(vsSize > 0)
			size.width = Ogre::StringConverter::parseReal(vs[0]);
		if(vsSize > 1)
			size.height = Ogre::StringConverter::parseReal(vs[1]);

		return size;
	}

	TabControlEvent StringConverter::parseTabControlEvent(const std::string& s)
	{
		if(s == "TABCONTROL_EVENT_SELECTION_CHANGED")				return TABCONTROL_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TabControlEvent type! (Possible need to update?)","StringConverter::parseTabControlEvent");
	}

	ToolBarItemLayout StringConverter::parseToolBarItemLayout(const std::string& s)
	{
		if(s == "TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE")			return TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE;
		else if(s == "TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE")	return TOOLBAR_ITEM_LAYOUT_POSITIVE_TO_NEGATIVE;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized ToolBarItemLayout type! (Possible need to update?)","StringConverter::parseToolBarItemLayout");
	}

	TreeViewEvent StringConverter::parseTreeViewEvent(const std::string& s)
	{
		if(s == "TREEVIEW_EVENT_SELECTION_CHANGED")					return TREEVIEW_EVENT_SELECTION_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TreeViewEvent type! (Possible need to update?)","StringConverter::parseTreeViewEvent");
	}

	TreeViewCheckBoxNodeEvent StringConverter::parseTreeViewCheckBoxNodeEvent(const std::string& s)
	{
		if(s == "TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED")			return TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TreeViewCheckBoxNodeEvent type! (Possible need to update?)","StringConverter::parseTreeViewCheckBoxNodeEvent");
	}

	TreeViewRadioButtonNodeEvent StringConverter::parseTreeViewRadioButtonNodeEvent(const std::string& s)
	{
		if(s == "TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED") return TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized TreeViewRadioButtonNodeEvent type! (Possible need to update?)","StringConverter::parseTreeViewRadioButtonNodeEvent");
	}

	VerticalAnchor StringConverter::parseVerticalAnchor(const std::string& s)
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

	VerticalTextAlignment StringConverter::parseVerticalTextAlignment(const std::string& s)
	{
		if(s == "TEXT_ALIGNMENT_VERTICAL_BOTTOM")					return TEXT_ALIGNMENT_VERTICAL_BOTTOM;
		else if(s == "TEXT_ALIGNMENT_VERTICAL_CENTER")				return TEXT_ALIGNMENT_VERTICAL_CENTER;
		else if(s == "TEXT_ALIGNMENT_VERTICAL_TOP")					return TEXT_ALIGNMENT_VERTICAL_TOP;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized VerticalTextAlignment type! (Possible need to update?)","StringConverter::parseVerticalTextAlignment");
	}

	WidgetEvent StringConverter::parseWidgetEvent(const std::string& s)
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

	WindowEvent StringConverter::parseWindowEvent(const std::string& s)
	{
		if(s == "WINDOW_EVENT_DRAWN")								return WINDOW_EVENT_DRAWN;
		else if(s == "WINDOW_EVENT_FOCUS_GAINED")					return WINDOW_EVENT_FOCUS_GAINED;
		else if(s == "WINDOW_EVENT_FOCUS_LOST")						return WINDOW_EVENT_FOCUS_LOST;
		else
			throw Exception(Exception::ERR_SERIALIZATION,"\"" + s + "\" is not a recognized Winhdow Event! (Possible need to update?)","StringConverter::parseWindowEvent");
	}
}
