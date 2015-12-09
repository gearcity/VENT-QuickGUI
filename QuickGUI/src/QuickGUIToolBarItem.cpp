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

#include "QuickGUIToolBarItem.h"
#include "QuickGUIToolBar.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUIDescManager.h"

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
		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		ToolBarItemDesc* defaultValues = DescManager::getSingleton().createDesc<ToolBarItemDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("Index", &index, defaultValues->index);

		DescManager::getSingleton().destroyDesc(defaultValues);
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
