#include "QuickGUIForwardDeclaredDesc.h"

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
		b->IO("Index",&tabpage_index);
		b->IO("TabFrontWidth",&tabpage_tabFrontWidth);
		b->IO("TabImageName",&tabpage_tabImageName);
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

		b->IO("tabcontrol_tabHeight",&tabcontrol_tabHeight);
		b->IO("tabcontrol_tabOverlap",&tabcontrol_tabOverlap);
		b->IO("tabcontrol_tabReordering",&tabcontrol_tabReordering);

		if(b->begin("UserDefinedHandlers","TabControlEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < TABCONTROL_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<TabControlEvent>(index)),&(tabcontrol_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < TABCONTROL_EVENT_COUNT; ++index)
				{
					if(tabcontrol_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<TabControlEvent>(index)),&(tabcontrol_userHandlers[index]));
				}
			}
			b->end();
		}
	}
}
