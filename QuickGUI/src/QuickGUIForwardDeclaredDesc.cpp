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

#include "QuickGUIForwardDeclaredDesc.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	PanelDesc::PanelDesc() :
		ContainerWidgetDesc()
	{
		resetToDefault();
	}

	void PanelDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(200,100);
	}

	TabPageDesc::TabPageDesc() :
		PanelDesc()
	{
		resetToDefault();
	}

	void TabPageDesc::resetToDefault()
	{
		PanelDesc::resetToDefault();

		widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;

		containerwidget_supportScrollBars = false;

		tabpage_index = -1;
		tabpage_tabFrontWidth = 0;
		tabpage_tabImageName = "";

		textDesc.resetToDefault();
	}

	void TabPageDesc::serialize(SerialBase* b)
	{
		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		TabPageDesc* defaultValues = DescManager::getSingleton().createDesc<TabPageDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("Index",			&tabpage_index,			defaultValues->tabpage_index);
		b->IO("TabFrontWidth",	&tabpage_tabFrontWidth, defaultValues->tabpage_tabFrontWidth);
		b->IO("TabImageName",	&tabpage_tabImageName,	defaultValues->tabpage_tabImageName);

		DescManager::getSingleton().destroyDesc(defaultValues);
	}

	TabControlDesc::TabControlDesc() :
		ContainerWidgetDesc()
	{
		resetToDefault();
	}

	void TabControlDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(200,100);

		containerwidget_supportScrollBars = false;
		tabcontrol_selectedTab = 0;
		tabcontrol_tabHeight = 20;
		tabcontrol_tabOverlap = 10;
		widget_transparencyPicking = false;
		tabcontrol_tabReordering = false;

		for(int index = 0; index < TABCONTROL_EVENT_COUNT; ++index)
			tabcontrol_userHandlers[index] = "";
	}

	void TabControlDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		TabControlDesc* defaultValues = DescManager::getSingleton().createDesc<TabControlDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("tabcontrol_tabHeight",&tabcontrol_tabHeight,defaultValues->tabcontrol_tabHeight);
		b->IO("tabcontrol_tabOverlap",&tabcontrol_tabOverlap,defaultValues->tabcontrol_tabOverlap);
		b->IO("tabcontrol_tabReordering",&tabcontrol_tabReordering,defaultValues->tabcontrol_tabReordering);

		DescManager::getSingleton().destroyDesc(defaultValues);

		if(b->begin("UserDefinedHandlers","TabControlEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < TABCONTROL_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<TabControlEvent>(index)),&(tabcontrol_userHandlers[index]),0);
			}
			else
			{
				for(int index = 0; index < TABCONTROL_EVENT_COUNT; ++index)
				{
					if(tabcontrol_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<TabControlEvent>(index)),&(tabcontrol_userHandlers[index]),0);
				}
			}
			b->end();
		}
	}
}
