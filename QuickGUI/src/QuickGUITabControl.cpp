#include "QuickGUITabControl.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITabPage.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUITab.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"

namespace QuickGUI
{
	const Ogre::String TabControl::BACKGROUND = "background";

	void TabControl::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TabControl");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TabControl",d);
	}

	TabControl::TabControl(const Ogre::String& name) :
		ContainerWidget(name),
		mSelectedTab(NULL)
	{
	}

	TabControl::~TabControl()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < TABCONTROL_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mTabControlEventHandlers[index].begin(); it != mTabControlEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void TabControl::_initialize(WidgetDesc* d)
	{
		TabControlDesc* tcd = dynamic_cast<TabControlDesc*>(d);
		tcd->containerwidget_supportScrollBars = false;

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<TabControlDesc*>(mWidgetDesc);

		for(int index = 0; index < CHECKBOX_EVENT_COUNT; ++index)
			mDesc->tabcontrol_userHandlers[index] = tcd->tabcontrol_userHandlers[index];

		setTabHeight(tcd->tabcontrol_tabHeight);
		setTabOverlap(tcd->tabcontrol_tabOverlap);
		setTabReordering(tcd->tabcontrol_tabReordering);

		setSkinType(d->widget_skinTypeName);
	}

	void TabControl::addChild(Widget* w)
	{
		ContainerWidget::addChild(w);

		TabPage* p = dynamic_cast<TabPage*>(w);
		p->setSize(mClientDimensions.size);
		// Important that Size is set before TabHeight is set, as setTabHeight resizes/repositions
		// Page, and relies on Widget Size.
		p->setTabHeight(mDesc->tabcontrol_tabHeight);
		p->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		p->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);

		// Insert Tab Page into mTabs list

		int index = p->getIndex();

		if((index < 0) || (index >= static_cast<int>(mTabs.size())))
		{
			// Update Index
			p->setIndex(static_cast<int>(mTabs.size()));

			mTabs.push_back(p);
		}
		else
		{
			int counter = 0;
			bool pageInserted = false;
			for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
			{
				if(pageInserted)
					(*it)->setIndex(counter);

				if(counter == index)
				{
					// Increment the current page's index
					(*it)->setIndex(counter + 1);

					mTabs.insert(it,p);
					p->setIndex(counter);
					pageInserted = true;

					++counter;
				}

				++counter;
			}
		}
		
		updateTabPositions();

		selectTabPage(p);
	}

	void TabControl::addUserDefinedTabControlEventHandler(TabControlEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->tabcontrol_userHandlers[EVENT] = handlerName;
	}

	void TabControl::addTabControlEventHandler(TabControlEvent EVENT, EventHandlerSlot* function)
	{
		mTabControlEventHandlers[EVENT].push_back(function);
	}

	void TabControl::clearUserDefinedTabControlEventHandler(TabControlEvent EVENT)
	{
		mDesc->tabcontrol_userHandlers[EVENT] = "";
	}

	TabPage* TabControl::createTabPage(TabPageDesc* d, int index)
	{
		d->tabpage_index = index;
		TabPage* newTabPage = dynamic_cast<TabPage*>(_createWidget(d));

		addChild(newTabPage);

		return newTabPage;
	}

	TabPage* TabControl::createTabPage(int index)
	{
		TabPageDesc* d = DescManager::getSingletonPtr()->getDefaultTabPageDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment("TabPage"));

		TabPage* p = createTabPage(d,index);

		d->resetToDefault();

		return p;
	}

	TabPage* TabControl::createTabPage(const Ogre::UTFString& text, int index)
	{
		TabPageDesc* d = DescManager::getSingletonPtr()->getDefaultTabPageDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		TabPage* p = createTabPage(d,index);

		d->resetToDefault();

		return p;
	}

	TabPage* TabControl::createTabPage(std::vector<TextSegment> segments, int index)
	{
		TabPageDesc* d = DescManager::getSingletonPtr()->getDefaultTabPageDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		TabPage* p = createTabPage(d,index);

		d->resetToDefault();

		return p;
	}

	TabPage* TabControl::createTabPageWithIcon(const Ogre::String& imageName, int index)
	{
		TabPageDesc* d = DescManager::getSingletonPtr()->getDefaultTabPageDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(" "));
		d->tabpage_tabImageName = imageName;

		TabPage* p = createTabPage(d,index);

		d->resetToDefault();

		return p;
	}

	TabPage* TabControl::createTabPageWithIcon(const Ogre::String& imageName, const Ogre::UTFString& text, int index)
	{
		TabPageDesc* d = DescManager::getSingletonPtr()->getDefaultTabPageDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));
		d->tabpage_tabImageName = imageName;

		TabPage* p = createTabPage(d,index);

		d->resetToDefault();

		return p;
	}

	TabPage* TabControl::createTabPageWithIcon(const Ogre::String& imageName, std::vector<TextSegment> segments, int index)
	{
		TabPageDesc* d = DescManager::getSingletonPtr()->getDefaultTabPageDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;
		d->tabpage_tabImageName = imageName;

		TabPage* p = createTabPage(d,index);

		d->resetToDefault();

		return p;
	}

	void TabControl::destroyTabPage(unsigned int index)
	{
		WidgetFactory* f = FactoryManager::getSingleton().getWidgetFactory();

		bool updatePages = false;
		float yPos = 0;
		int count = 0;
		for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
		{
			if(updatePages)
			{
				// Update index
				(*it)->setIndex(count);
			}

			if(!updatePages && (count == index))
			{
				updatePages = true;

				removeChild((*it));
				mTabs.erase(it);

				if(mDesc->sheet != NULL)
					mDesc->sheet->mFreeList.push_back((*it));
				else
					Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back((*it));

				--count;
			}

			++count;
		}

		updateTabPositions();
	}

	void TabControl::draw()
	{
		// check visibility
		if( !mWidgetDesc->widget_visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// check and store clip region
		Rect prevClipRegion = brush->getClipRegion();
		Rect widgetClipRegion = prevClipRegion;
		if ( widgetClipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size)) == Rect::ZERO )
			return;

		// set clip region to dimensions
		brush->setClipRegion(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).getIntersection(prevClipRegion));

		// draw self
		onDraw();

		// set clip region to client dimensions

		Rect clientClipRegion(mTexturePosition,mClientDimensions.size);
		clientClipRegion.translate(mClientDimensions.position);
		clientClipRegion = prevClipRegion.getIntersection(clientClipRegion);
		
		brush->setClipRegion(clientClipRegion);

		// draw children - draw all of the unselected TabPages first, followed by the selected TabPage
		for(std::vector<Widget*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
		{
			TabPage* tp = dynamic_cast<TabPage*>(*it);

			if(tp != mSelectedTab)
				dynamic_cast<TabPage*>(*it)->draw();
		}

		if(mSelectedTab != NULL)
			mSelectedTab->draw();

		// restore clip region
		brush->setClipRegion(prevClipRegion);
	}

	Widget* TabControl::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		Widget* w = Widget::findWidgetAtPoint(p,ignoreDisabled);

		if(w == NULL)
			return NULL;

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			Widget* w = (*it).second->findWidgetAtPoint(p,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		// Get the client bounds as displayed on the texture
		Rect clientBounds(mTexturePosition,mClientDimensions.size);
		clientBounds.translate(mClientDimensions.position);

		if(clientBounds.isPointWithinBounds(p))
		{
			// Check selected tab first.
			if(mSelectedTab != NULL)
			{
				w = mSelectedTab->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}

			for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				Widget* w = (*it)->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}
		}
				
		return this;
	}

	bool TabControl::fireTabControlEvent(TabControlEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mTabControlEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->tabcontrol_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->tabcontrol_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	Ogre::String TabControl::getClass()
	{
		return "TabControl";
	}

	TabPage* TabControl::getSelectedTabPage()
	{
		return mSelectedTab;
	}

	float TabControl::getTabHeight()
	{
		return mDesc->tabcontrol_tabHeight;
	}

	float TabControl::getTabOverlap()
	{
		return mDesc->tabcontrol_tabOverlap;
	}

	TabPage* TabControl::getTabPage(int index)
	{
		if(mTabs.empty())
			return NULL;

		if((index < 0) || (index >= static_cast<int>(mTabs.size())))
			return mTabs.back();

		int counter = 0;
		for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
		{
			if(counter == index)
				return dynamic_cast<TabPage*>(*it);
		}

		return mTabs.back();
	}

	bool TabControl::getTabReordering()
	{
		return mDesc->tabcontrol_tabReordering;
	}

	void TabControl::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void TabControl::reorderTabPage(TabPage* p, int index)
	{
		std::list<TabPage*>::iterator it = std::find(mTabs.begin(),mTabs.end(),p);
		if(it == mTabs.end())
			return;

		mTabs.erase(it);

		if((index < 0) || (index >= static_cast<int>(mTabs.size())))
		{
			mTabs.push_back(p);
			p->setIndex(static_cast<int>(mTabs.size()));
		}
		else
		{
			int counter = 0;
			for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
			{
				if(counter == index)
				{
					mTabs.insert(it,p);
					break;
				}

				++counter;
			}
		}

		int counter = 0;
		for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
		{
			(*it)->setIndex(counter);
			++counter;
		}

		updateTabPositions();
	}

	void TabControl::selectTabPage(TabPage* p)
	{
		if(p == NULL)
			throw Exception(Exception::ERR_INVALID_CHILD,"TabPage is NULL, cannot select it!","TabControl::selecteTabPage");

		if(p->getParentWidget() != this)
			throw Exception(Exception::ERR_INVALID_CHILD,"TabPage is not a child of this TabControl!","TabControl::selectTabPage");

		mSelectedTab = p;

		for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
			(*it)->deselect();

		mSelectedTab->select();

		WidgetEventArgs args(this);
		fireTabControlEvent(TABCONTROL_EVENT_SELECTION_CHANGED,args);
	}

	void TabControl::selectTabPage(int index)
	{
		if((index < 0) || (index >= static_cast<int>(mTabs.size())))
			return;

		int count = 0;
		for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
		{
			if(count == index)
			{
				mSelectedTab = (*it);
				mSelectedTab->select();
			}
			else
				(*it)->deselect();

			++count;
		}

		WidgetEventArgs args(this);
		fireTabControlEvent(TABCONTROL_EVENT_SELECTION_CHANGED,args);
	}

	void TabControl::setTabHeight(float height)
	{
		mDesc->tabcontrol_tabHeight = height;

		for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
		{
			(*it)->setTabHeight(mDesc->tabcontrol_tabHeight);
		}
	}

	void TabControl::setTabOverlap(float overlap)
	{
		mDesc->tabcontrol_tabOverlap = overlap;

		updateTabPositions();
	}

	void TabControl::setTabReordering(bool reordering)
	{
		mDesc->tabcontrol_tabReordering = reordering;
	}

	void TabControl::setTransparencyPicking(bool transparencyPicking)
	{
		ContainerWidget::setTransparencyPicking(transparencyPicking);

		for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
			(*it)->setTransparencyPicking(transparencyPicking);
	}

	void TabControl::updateTabPositions()
	{
		int counter = 0;
		float x = 0;
		for(std::list<TabPage*>::iterator it = mTabs.begin(); it != mTabs.end(); ++it)
		{
			(*it)->mTab->setPosition(Point(x,0));
			x += (*it)->mTab->getSize().width - mDesc->tabcontrol_tabOverlap;

			++counter;
		}

		redraw();
	}
}
