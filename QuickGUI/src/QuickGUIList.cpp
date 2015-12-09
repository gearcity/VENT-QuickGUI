#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIListItem.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUIListPanelItem.h"
#include "QuickGUIListImageItem.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUISheet.h"
#include "QuickGUIRoot.h"

#include "OgreStringConverter.h"

namespace QuickGUI
{
	const Ogre::String List::BACKGROUND = "background";
	const Ogre::String List::LISTITEM = "listitem";

	void List::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("List");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->defineComponent(LISTITEM);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("List",d);
	}

	ListDesc::ListDesc() :
		ContainerWidgetDesc()
	{
		resetToDefault();
	}

	void ListDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(125,100);

		list_itemHeight = 25;
		list_supportMultiSelect = false;

		for(int index = 0; index < LIST_EVENT_COUNT; ++index)
			list_userHandlers[index] = "";
	}

	void ListDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("ListItemHeight",&list_itemHeight);
		b->IO("MultiSelect",&list_supportMultiSelect);

		if(b->begin("UserDefinedHandlers","ListEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < LIST_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<CheckBoxEvent>(index)),&(list_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < LIST_EVENT_COUNT; ++index)
				{
					if(list_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<CheckBoxEvent>(index)),&(list_userHandlers[index]));
				}
			}
			b->end();
		}
	}

	List::List(const Ogre::String& name) :
		ContainerWidget(name),
		mPrevSelectedIndex(0),
		mAutoNameCounter(0)
	{
	}

	List::~List()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < LIST_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mListEventHandlers[index].begin(); it != mListEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void List::_clearSelection()
	{
		for(std::list<ListItem*>::iterator it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
		{
			(*it)->setSelected(false);
		}
		mSelectedItems.clear();
	}

	unsigned int List::_getItemIndex(ListItem* li)
	{
		int count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(li == (*it))
				return count;

			++count;
		}

		return 0;
	}

	void List::_initialize(WidgetDesc* d)
	{
		ListDesc* ld = dynamic_cast<ListDesc*>(d);

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<ListDesc*>(mWidgetDesc);

		for(int index = 0; index < LIST_EVENT_COUNT; ++index)
			ld->list_userHandlers[index] = ld->list_userHandlers[index];

		setItemHeight(ld->list_itemHeight);
		setMultiSelect(ld->list_supportMultiSelect);

		setSkinType(ld->widget_skinTypeName);
	}

	void List::addChild(Widget* w)
	{
		ListItem* i = dynamic_cast<ListItem*>(w);

		if(i->getWidth() < mClientDimensions.size.width)
		{
			i->setWidth(mClientDimensions.size.width);
		}

		i->setHeight(mDesc->list_itemHeight);
		i->setSkinType(mSkinType->getSkinReference(LISTITEM)->typeName);

		int itemIndex = i->getIndex();

		// Adding Item to End of List
		if((itemIndex < 0) || (itemIndex >= static_cast<int>(mItems.size())))
		{
			// Update Index
			i->setIndex(static_cast<int>(mItems.size()));
			
			// Update Position
			if(!mItems.empty())
			{
				Point p = mItems.back()->getPosition();
				p.y += i->getSize().height;
				i->setPosition(p);
			}

			mItems.push_back(i);
		}
		// Inserting Item into the List
		else
		{
			int count = 0;
			for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			{
				if(count == itemIndex)
				{
					// Insert Item into List.  All items after this will need to have
					// index and position updated.

					Point p = (*it)->getPosition();

					mItems.insert(it,i);
					i->setPosition(p);

					break;
				}

				++count;
			}

			count = 0;
			Point p;
			for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			{
				// Update Index
				(*it)->setIndex(count);
				// Update Position
				(*it)->setPosition(p);

				p.y += (*it)->getHeight();

				++count;
			}
		}

		ContainerWidget::addChild(w);
	}

	void List::addUserDefinedListEventHandler(ListEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->list_userHandlers[EVENT] = handlerName;
	}

	void List::addListEventHandler(ListEvent EVENT, EventHandlerSlot* function)
	{
		mListEventHandlers[EVENT].push_back(function);
	}

	void List::clearItems()
	{
		mSelectedItems.clear();

		destroyChildren();

		mItems.clear();

		redraw();
	}

	void List::clearSelection()
	{
		_clearSelection();

		WidgetEventArgs args(this);
		fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);
	}

	std::list<ListItem*> List::getItemList()
	{
		return mItems;
	}

	void List::clearUserDefinedListEventHandler(ListEvent EVENT)
	{
		mDesc->list_userHandlers[EVENT] = "";
	}

	ListItem* List::createItem(ListItemDesc* d, int index)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.Item." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->listitem_index = index;

		ListItem* newListItem = dynamic_cast<ListItem*>(_createWidget(d));
		addChild(newListItem);

		return newListItem;
	}

	ListImageItem* List::createImageItem(const Ogre::String& imageName, int index)
	{
		ListImageItemDesc* d = DescManager::getSingletonPtr()->getDefaultListImageItemDesc();
		d->resetToDefault();
		d->listimageitem_imageName = imageName;

		ListImageItem* i = dynamic_cast<ListImageItem*>(createItem(d,index));

		return i;
	}

	ListImageItem* List::createImageItem(int index)
	{
		return createImageItem("",index);
	}

	ListPanelItem* List::createPanelItem(int index)
	{
		ListPanelItemDesc* d = DescManager::getSingletonPtr()->getDefaultListPanelItemDesc();
		d->resetToDefault();

		ListPanelItem* i = dynamic_cast<ListPanelItem*>(createItem(d,index));

		return i;
	}

	ListTextItem* List::createTextItem(const Ogre::UTFString& text, int index)
	{
		ListTextItemDesc* d = DescManager::getSingletonPtr()->getDefaultListTextItemDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		ListTextItem* i = dynamic_cast<ListTextItem*>(createItem(d,index));

		return i;
	}

	ListTextItem* List::createTextItem(std::vector<TextSegment> segments, int index)
	{
		ListTextItemDesc* d = DescManager::getSingletonPtr()->getDefaultListTextItemDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		ListTextItem* i = dynamic_cast<ListTextItem*>(createItem(d,index));

		return i;
	}

	ListTextItem* List::createTextItem(int index)
	{
		return createTextItem("ListTextItem",index);
	}

	void List::deselectItem(unsigned int index)
	{
		if(index >= mItems.size())
			return;

		ListItem* deselected = NULL;

		unsigned int count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
			{
				deselected = (*it);
				(*it)->setSelected(false);
				break;
			}

			++count;
		}

		if(deselected == NULL)
			return;

		for(std::list<ListItem*>::iterator it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
		{
			if((*it) == deselected)
			{
				mSelectedItems.erase(it);
				break;
			}
		}

		WidgetEventArgs args(this);
		fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);
	}

	void List::destroyItem(unsigned int index)
	{
		WidgetFactory* f = FactoryManager::getSingleton().getWidgetFactory();

		bool itemRemovedFromList = false;
		int count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
			{
				removeChild((*it));

				if(mDesc->sheet != NULL)
					mDesc->sheet->mFreeList.push_back((*it));
				else
					Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back((*it));
				mItems.erase(it);

				itemRemovedFromList = true;

				break;
			}

			++count;
		}

		if(!itemRemovedFromList)
			return;

		float yPos = 0;
		count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			// Update index
			(*it)->setIndex(count);

			// Update Position
			Point p = (*it)->getPosition();
			p.y = yPos;
			(*it)->setPosition(p);

			// Store position of next item
			yPos += (*it)->getSize().height;

			++count;
		}

		redraw();
	}

	void List::destroyItem(ListItem* i)
	{
		destroyItem(i->getIndex());
	}

	bool List::fireListEvent(ListEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mListEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->list_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->list_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	Ogre::String List::getClass()
	{
		return "List";
	}

	ListItem* List::getItem(unsigned int index)
	{
		if(index >= mItems.size())
			return NULL;

		unsigned int count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
				return (*it);

			++count;
		}

		return NULL;
	}

	float List::getItemHeight()
	{
		return mDesc->list_itemHeight;
	}

	bool List::getMultiSelect()
	{
		return mDesc->list_supportMultiSelect;
	}

	int List::getNumberOfItems()
	{
		return static_cast<int>(mItems.size());
	}

	std::list<ListItem*> List::getSelection()
	{
		return mSelectedItems;
	}

	void List::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void List::selectItem(const MouseEventArgs& mea)
	{
		ListItem* li = dynamic_cast<ListItem*>(mea.widget);

		// Single Selection
		if(!mDesc->list_supportMultiSelect)
		{
			bool selected = li->getSelected();

			_clearSelection();

			li->setSelected(!selected);

			if(li->getSelected())
				mSelectedItems.push_back(li);
			else
				li->mSkinElement = li->mSkinType->getSkinElement(ListItem::OVER);
		}
		// Multi Selection
		else
		{
			bool ctrlDown = (mea.keyModifiers & CTRL) > 0;
			bool shiftDown = (mea.keyModifiers & SHIFT) > 0;

			if(ctrlDown)
			{
				if(shiftDown)
				{
					// Select everything in the range that is unselected
					unsigned int currentIndex = _getItemIndex(li);
					if(currentIndex < mPrevSelectedIndex)
					{
						unsigned int count = 0;
						for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
						{
							if(count > mPrevSelectedIndex)
								break;

							if(count >= currentIndex)
							{
								mSelectedItems.push_back(*it);
								(*it)->setSelected(true);
							}

							++count;
						}
					}
					else
					{
						unsigned int count = 0;
						for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
						{
							if(count > currentIndex)
								break;

							if(count >= mPrevSelectedIndex)
							{
								mSelectedItems.push_back(*it);
								(*it)->setSelected(true);
							}

							++count;
						}
					}
				}
				else
				{
					// toggle selection
					li->setSelected(!li->getSelected());

					// if selected, add to list
					if(li->getSelected())
						mSelectedItems.push_back(li);
					// otherwise remove from selected list
					else
					{
						for(std::list<ListItem*>::iterator it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
						{
							if(li == (*it))
							{
								mSelectedItems.erase(it);
								break;
							}
						}

						li->mSkinElement = li->mSkinType->getSkinElement(ListItem::OVER);
					}
				}
			}
			else
			{
				if(shiftDown)
				{
					_clearSelection();

					// Select everything in the range that is unselected
					unsigned int currentIndex = _getItemIndex(li);
					if(currentIndex < mPrevSelectedIndex)
					{
						unsigned int count = 0;
						for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
						{
							if(count > mPrevSelectedIndex)
								break;

							if(count >= currentIndex)
							{
								mSelectedItems.push_back(*it);
								(*it)->setSelected(true);
							}

							++count;
						}
					}
					else
					{
						unsigned int count = 0;
						for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
						{
							if(count > currentIndex)
								break;

							if(count >= mPrevSelectedIndex)
							{
								mSelectedItems.push_back(*it);
								(*it)->setSelected(true);
							}

							++count;
						}
					}
				}
				else
				{
					bool selected = li->getSelected();

					_clearSelection();

					// toggle selection
					li->setSelected(!selected);

					// if selected, add to list
					if(li->getSelected())
						mSelectedItems.push_back(li);
					else
						li->mSkinElement = li->mSkinType->getSkinElement(ListItem::OVER);
				}
			}
		}

		mPrevSelectedIndex = _getItemIndex(li);

		redraw();

		WidgetEventArgs args(this);
		fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);
	}

	void List::selectItem(unsigned int index)
	{
		_clearSelection();

		if(index >= mItems.size())
			return;

		unsigned int count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
			{
				(*it)->setSelected(true);
				mSelectedItems.push_back(*it);

				WidgetEventArgs args(this);
				fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);

				return;
			}

			++count;
		}
	}

	void List::setMultiSelect(bool MultiSelect)
	{
		mDesc->list_supportMultiSelect = MultiSelect;
	}

	void List::setItemHeight(float height)
	{
		mDesc->list_itemHeight = height;

		float y = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			(*it)->setHeight(mDesc->list_itemHeight);

			y += mDesc->list_itemHeight;
		}
	}

	void List::setSkinType(const Ogre::String type)
	{
		ContainerWidget::setSkinType(type);

		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->setSkinType(mSkinType->getSkinReference(LISTITEM)->typeName);
	}

	void List::updateClientDimensions()
	{
		ContainerWidget::updateClientDimensions();

		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if((*it)->getWidth() < mClientDimensions.size.width )
			{
				(*it)->setWidth(mClientDimensions.size.width);
			}
		}
	}


}
