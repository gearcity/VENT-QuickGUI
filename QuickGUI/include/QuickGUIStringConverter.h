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

#ifndef QUICKGUISTRINGCONVERTER_H
#define QUICKGUISTRINGCONVERTER_H

#include "QuickGUIAnchors.h"
#include "QuickGUIBrushEnums.h"
#include "QuickGUIConsoleEnums.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIProgressBarEnums.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIScrollBarButtonLayouts.h"
#include "QuickGUITextAlignment.h"
#include "QuickGUIToolBarEnums.h"
#include "QuickGUIPoint.h"
#include "QuickGUISize.h"
#include "QuickGUIRect.h"
#include "QuickGUIOgreEquivalents.h"

namespace QuickGUI
{
	class _QuickGUIExport StringConverter
	{
	public:
		static std::string toString(int i);
		static std::string toString(BrushFilterMode m);
		static std::string toString(CheckBoxEvent e);
		static std::string toString(ColourValue c);
		static std::string toString(ComboBoxEvent e);
		static std::string toString(ConsoleLayout l);
		static std::string toString(HorizontalAnchor a);
		static std::string toString(HorizontalTextAlignment a);
		static std::string toString(HScrollBarButtonLayout l);
		static std::string toString(ListEvent e);
		static std::string toString(MouseEvent e);
		static std::string toString(Point p);
		static std::string toString(ProgressBarEvent e);
		static std::string toString(ProgressBarFillDirection d);
		static std::string toString(ProgressBarLayout l);
		static std::string toString(ProgressBarClippingEdge e);
		static std::string toString(PropertyGridEvent e);
		static std::string toString(RadioButtonEvent e);
		static std::string toString(Rect r);
		static std::string toString(ScrollBarEvent e);
		static std::string toString(SheetEvent e);
		static std::string toString(Size s);
		static std::string toString(TabControlEvent e);
		static std::string toString(ToolBarItemLayout l);
		static std::string toString(TreeViewEvent e);
		static std::string toString(TreeViewCheckBoxNodeEvent e);
		static std::string toString(TreeViewRadioButtonNodeEvent e);
		static std::string toString(VerticalAnchor a);
		static std::string toString(VerticalTextAlignment a);
		static std::string toString(VScrollBarButtonLayout l);
		static std::string toString(WidgetEvent e);
		static std::string toString(WindowEvent e);

		static BrushFilterMode parseBrushFilterMode(const std::string& s);
		static CheckBoxEvent parseCheckBoxEvent(const std::string& s);
		static ColourValue parseColourValue(const std::string& s);
		static ComboBoxEvent parseComboBoxEvent(const std::string& s);
		static ConsoleLayout parseConsoleLayout(const std::string& s);
		static HorizontalAnchor parseHorizontalAnchor(const std::string& s);
		static HorizontalTextAlignment parseHorizontalTextAlignment(const std::string& s);
		static HScrollBarButtonLayout parseHScrollBarButtonLayout(const std::string& s);
		static int parseInt(const std::string& s);
		static ListEvent parseListEvent(const std::string& s);
		static MouseEvent parseMouseEvent(const std::string& s);
		static Point parsePoint(const std::string& s);
		static ProgressBarEvent parseProgressBarEvent(const std::string& s);
		static ProgressBarFillDirection parseProgressBarFillDirection(const std::string& s);
		static ProgressBarLayout parseProgressBarLayout(const std::string& s);
		static ProgressBarClippingEdge parseProgressBarClippingEdge(const std::string& s);
		static PropertyGridEvent parsePropertyGridEvent(const std::string& s);
		static RadioButtonEvent parseRadioButtonEvent(const std::string& s);
		static Rect parseRect(const std::string& s);
		static ScrollBarEvent parseScrollBarEvent(const std::string& s);
		static SheetEvent parseSheetEvent(const std::string& s);
		static Size parseSize(const std::string& s);
		static TabControlEvent parseTabControlEvent(const std::string& s);
		static ToolBarItemLayout parseToolBarItemLayout(const std::string& s);
		static TreeViewEvent parseTreeViewEvent(const std::string& s);
		static TreeViewCheckBoxNodeEvent parseTreeViewCheckBoxNodeEvent(const std::string& s);
		static TreeViewRadioButtonNodeEvent parseTreeViewRadioButtonNodeEvent(const std::string& s);
		static VerticalAnchor parseVerticalAnchor(const std::string& s);
		static VerticalTextAlignment parseVerticalTextAlignment(const std::string& s);
		static VScrollBarButtonLayout parseVScrollBarButtonLayout(const std::string& s);
		static WidgetEvent parseWidgetEvent(const std::string& s);
		static WindowEvent parseWindowEvent(const std::string& s);

	protected:
	private:
	};
}

#endif
