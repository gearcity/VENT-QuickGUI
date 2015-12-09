#include "QuickGUIListItem.h"
#include "QuickGUIList.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIRoot.h"
#include "QuickGUIManager.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	const Ogre::String ListItem::DEFAULT = "default";
	const Ogre::String ListItem::OVER = "over";
	const Ogre::String ListItem::SELECTED = "selected";

	ListItemDesc::ListItemDesc() :
		ContainerWidgetDesc()
	{
		resetToDefault();
	}

	void ListItemDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		listitem_index = -1;
		listitem_selected = false;
		containerwidget_supportScrollBars = true;
		widget_transparencyPicking = false;
	}

	void ListItemDesc::serialize(SerialBase* b)
	{
		b->IO("Index",&listitem_index);
		b->IO("Selected",&listitem_selected);
	}

	ListItem::ListItem(const Ogre::String& name) :
		ContainerWidget(name)
	{
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&ListItem::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&ListItem::onMouseEnter,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&ListItem::onMouseLeave,this);
		//addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&ListItem::onMouseWheel,this);
		//addWidgetEventHandler(WIDGET_EVENT_MOUSE_WHEEL,&ListItem::onMouseWheel,this);

	}

	ListItem::~ListItem()
	{
	}

	void ListItem::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		ListItemDesc* lid = dynamic_cast<ListItemDesc*>(d);
		lid->containerwidget_supportScrollBars = true;
		lid->widget_scrollable = true;
		mDesc = dynamic_cast<ListItemDesc*>(mWidgetDesc);

		setSkinType(d->widget_skinTypeName);
		setIndex(lid->listitem_index);
		setSelected(lid->listitem_selected);
	}

	void ListItem::destroy()
	{
		if(isComponentOfAWidget())
			return;

		if(mParentWidget != NULL)
		{
			Ogre::String className = mParentWidget->getClass();
			if(className == "List")
				dynamic_cast<List*>(mParentWidget)->destroyItem(this);
			else if(className == "MenuPanel")
			{
				Widget* owner = dynamic_cast<MenuPanel*>(mParentWidget)->getOwner();
				Ogre::String ownerClassName = owner->getClass();
				if(ownerClassName == "ComboBox")
					dynamic_cast<ComboBox*>(owner)->destroyItem(this);
			}
		}
		else
		{
			if(mWidgetDesc->sheet != NULL)
				mWidgetDesc->sheet->mFreeList.push_back(this);
			else
				Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back(this);
		}
	}

	bool ListItem::getEnabled()
	{
		return ContainerWidget::getEnabled();
	}

	int ListItem::getIndex()
	{
		return mDesc->listitem_index;
	}

	bool ListItem::getSelected()
	{
		return mDesc->listitem_selected;
	}

	bool ListItem::isPositionManaged()
	{
		return true;
	}

	bool ListItem::isSizeManaged()
	{
		return true;
	}

	void ListItem::onMouseButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			// List Items can belong to Lists or ComboBox
			if(mParentWidget->getClass() == "List")
				dynamic_cast<List*>(mParentWidget)->selectItem(mea);
			else if(mParentWidget->getClass() == "MenuPanel")
			{
				MenuPanel* lp = dynamic_cast<MenuPanel*>(mParentWidget);
				if(lp->getOwner() != NULL)
				{
					if(lp->getOwner()->getClass() == "ComboBox")
						dynamic_cast<ComboBox*>(lp->getOwner())->selectItem(mea);
				}
			}
		}
	}

	void ListItem::onMouseEnter(const EventArgs& args)
	{
		if(!mDesc->listitem_selected)
		{
			mSkinElement = mSkinType->getSkinElement(OVER);
			redraw();
		}
	}

	void ListItem::onMouseWheel(const EventArgs& args)
	{
	/*	EventArgs Gargs = args;
		
		

			if(mParentWidget->getClass() == "List")
			{	//mParentWidget->fireWidgetEvent(WIDGET_EVENT_MOUSE_WHEEL,Gargs);
				List* pList = dynamic_cast<List*>(mParentWidget);
				//VScrollBar* vbard = static_cast<VScrollBar*>(mParentWidget->findFirstWidgetOfClass("vscrollbar"));



				if(pList->mVScrollBar == 0 || pList->mVScrollBar->getVisible())
				{
					return;
				}
				else
				{
					const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

					if(mea.wheelChange > 0)
						pList->mVScrollBar->scrollUp();
					else
						pList->mVScrollBar->scrollDown();
				}
			}
			else if(mParentWidget->getClass() == "MenuPanel")
			{
				MenuPanel* lp = dynamic_cast<MenuPanel*>(mParentWidget);
				if(lp->getOwner() != NULL)
				{
					if(lp->getOwner()->getClass() == "ComboBox")
					{//mParentWidget->fireWidgetEvent(WIDGET_EVENT_MOUSE_WHEEL,Gargs);
							VScrollBar* vbard = dynamic_cast<VScrollBar*>(mParentWidget->findFirstWidgetOfClass("vscrollbar"));
							if(vbard == 0 || vbard->getVisible())
							{
								return;
							}
							else
							{
								const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

								if(mea.wheelChange > 0)
									vbard->scrollUp();
								else
									vbard->scrollDown();
							}
					}
				}
			}
	*/}

	void ListItem::onMouseLeave(const EventArgs& args)
	{
		if(!mDesc->listitem_selected)
		{
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
			redraw();
		}
	}

	void ListItem::setEnabled(bool enabled)
	{
		ContainerWidget::setEnabled(enabled);
	}

	void ListItem::setIndex(unsigned int index)
	{
		mDesc->listitem_index = index;
	}

	void ListItem::setSelected(bool selected)
	{
		mDesc->listitem_selected = selected;
		
		if(mSkinElement != NULL)
		{
			if(mDesc->listitem_selected)
				mSkinElement = mSkinType->getSkinElement(SELECTED);
			else
				mSkinElement = mSkinType->getSkinElement(DEFAULT);

			redraw();
		}
	}

	void ListItem::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
	}
}
