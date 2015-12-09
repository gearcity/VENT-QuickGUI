#include "QuickGUIToolBarItem.h"
#include "QuickGUIToolBar.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"

namespace QuickGUI
{
	ToolBarItemDesc::ToolBarItemDesc() :
		ContainerWidgetDesc(),
		toolBar(NULL)
	{
		resetToDefault();
	}

	void ToolBarItemDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		containerwidget_supportScrollBars = false;
	}

	void ToolBarItemDesc::serialize(SerialBase* b)
	{
		b->IO("Index",&index);
	}

	ToolBarItem::ToolBarItem(const Ogre::String& name) :
		ContainerWidget(name)
	{
	}

	ToolBarItem::~ToolBarItem()
	{
	}

	void ToolBarItem::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		dynamic_cast<ToolBarItemDesc*>(mWidgetDesc)->index = dynamic_cast<ToolBarItemDesc*>(d)->index;
		dynamic_cast<ToolBarItemDesc*>(mWidgetDesc)->toolBar = dynamic_cast<ToolBarItemDesc*>(d)->toolBar;

		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&ToolBarItem::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&ToolBarItem::onMouseButtonUp,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&ToolBarItem::onMouseEnter,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&ToolBarItem::onMouseLeave,this);
	}

	void ToolBarItem::destroy()
	{
		if(isComponentOfAWidget())
			return;

		if(mParentWidget != NULL)
			dynamic_cast<ToolBar*>(mParentWidget)->destroyItem(this);
		else
		{
			if(mWidgetDesc->sheet != NULL)
				mWidgetDesc->sheet->mFreeList.push_back(this);
			else
				Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back(this);
		}
	}

	bool ToolBarItem::getEnabled()
	{
		return ContainerWidget::getEnabled();
	}

	int ToolBarItem::getIndex()
	{
		return dynamic_cast<ToolBarItemDesc*>(mWidgetDesc)->index;
	}

	bool ToolBarItem::isPositionManaged()
	{
		return true;
	}

	bool ToolBarItem::isSizeManaged()
	{
		return true;
	}

	bool ToolBarItem::isToolBarItem()
	{
		return true;
	}

	void ToolBarItem::notifyToolBarParent(ToolBar* b)
	{
		dynamic_cast<ToolBarItemDesc*>(mWidgetDesc)->toolBar = b;
	}

	void ToolBarItem::onMouseButtonDown(const EventArgs& args)
	{
	}

	void ToolBarItem::onMouseButtonUp(const EventArgs& args)
	{
	}

	void ToolBarItem::onMouseEnter(const EventArgs& args)
	{
	}

	void ToolBarItem::onMouseLeave(const EventArgs& args)
	{
	}

	void ToolBarItem::setEnabled(bool enabled)
	{
		ContainerWidget::setEnabled(enabled);
	}

	void ToolBarItem::setIndex(unsigned int index)
	{
		dynamic_cast<ToolBarItemDesc*>(mWidgetDesc)->index = index;
	}
}
