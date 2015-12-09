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

#include "QuickGUIPropertyGrid.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISheet.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIPropertyGridSection.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIVScrollBar.h"

namespace QuickGUI
{
	const Ogre::String PropertyGrid::BACKGROUND = "background";
	const Ogre::String PropertyGrid::EXPAND = "expand";
	const Ogre::String PropertyGrid::MINIMIZE = "minimize";
	const Ogre::String PropertyGrid::TEXTFIELD = "textfield";
	const Ogre::String PropertyGrid::BOOLFIELD = "boolfield";
	const Ogre::String PropertyGrid::MULTIFIELD = "multifield";

	void PropertyGrid::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGrid");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinReference(EXPAND,"Button");
		d->defineSkinReference(MINIMIZE,"Button");
		d->defineSkinReference(HSCROLLBAR,"HScrollBar");
		d->defineSkinReference(VSCROLLBAR,"VScrollBar");
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGrid",d);
	}

	PropertyGridDesc::PropertyGridDesc() :
		ContainerWidgetDesc()
	{
		resetToDefault();
	}

	void PropertyGridDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(125,100);

		propertygrid_doubleClickToToggle = true;
		propertygrid_keyNavigation = true;
		propertygrid_propertyFieldWidth = 100;
		propertygrid_itemHeight = 10;

		for(int index = 0; index < PROPERTYGRID_EVENT_COUNT; ++index)
			propertygrid_userHandlers[index] = "";
	}

	void PropertyGridDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		PropertyGridDesc* defaultValues = DescManager::getSingleton().createDesc<PropertyGridDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("DoubleClickToToggle",	&propertygrid_doubleClickToToggle,	defaultValues->propertygrid_doubleClickToToggle);
		b->IO("KeyNavigation",			&propertygrid_keyNavigation,		defaultValues->propertygrid_keyNavigation);
		b->IO("PropertyFieldWidth",		&propertygrid_propertyFieldWidth,	defaultValues->propertygrid_propertyFieldWidth);
		b->IO("PropertyHeight",			&propertygrid_itemHeight,			defaultValues->propertygrid_itemHeight);

		DescManager::getSingleton().destroyDesc(defaultValues);

		if(b->begin("UserDefinedHandlers","PropertyGridEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < PROPERTYGRID_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<PropertyGridEvent>(index)),&(propertygrid_userHandlers[index]),"");
			}
			else
			{
				for(int index = 0; index < PROPERTYGRID_EVENT_COUNT; ++index)
				{
					if(propertygrid_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<PropertyGridEvent>(index)),&(propertygrid_userHandlers[index]),"");
				}
			}
			b->end();
		}
	}

	PropertyGrid::PropertyGrid(const Ogre::String& name) :
		ContainerWidget(name),
		mSelectedProperty(NULL),
		mDesc(NULL)
	{
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&PropertyGrid::onMouseDown,this);
	}

	PropertyGrid::~PropertyGrid()
	{
	}

	float PropertyGrid::_getFarthestX()
	{
		float farthestX = 0;
		float childX = 0;

		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
		{
			childX = (*it)->_getFarthestX();
			if(childX > farthestX)
				farthestX = childX;
		}

		return farthestX;
	}

	void PropertyGrid::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		PropertyGridDesc* pgd = dynamic_cast<PropertyGridDesc*>(d);
		mDesc = dynamic_cast<PropertyGridDesc*>(mWidgetDesc);

		for(int index = 0; index < PROPERTYGRID_EVENT_COUNT; ++index)
			mDesc->propertygrid_userHandlers[index] = pgd->propertygrid_userHandlers[index];

		setSkinType(pgd->widget_skinTypeName);

		setDoubleClickToToggle(pgd->propertygrid_doubleClickToToggle);
		setKeyNavigation(pgd->propertygrid_keyNavigation);
		setPropertyFieldWidth(pgd->propertygrid_propertyFieldWidth);
		setItemHeight(pgd->propertygrid_itemHeight);
	}

	void PropertyGrid::addChild(Widget* w)
	{
		ContainerWidget::addChild(w);

		PropertyGridSection* pgs = dynamic_cast<PropertyGridSection*>(w);

		// Set link to TreeView
		pgs->notifyPropertyGridParent(this);
		// Update dimensions
		pgs->setHeight(getItemHeight());
		pgs->setWidth((getItemHeight() * 2) + pgs->getTextWidth() + pgs->mLabelBuffer);

		int index = pgs->getIndex();

		// If we're adding to the end, things are simple - don't have to update other node names, or positions.
		if((index < 0) || (index >= static_cast<int>(mSections.size())))
		{
			mSections.push_back(pgs);
		}
		else
		{
			// If we've made it here, we have to insert the node into the list, and update all node indices and positions.
			int counter = 0;
			for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
			{
				if(counter == index)
				{
					mSections.insert(it,pgs);
					break;
				}

				++counter;
			}
		}

		updateItems();

		float textHeight = pgs->getTextHeight();
		if(textHeight > mDesc->propertygrid_itemHeight)
			setItemHeight(textHeight);
	}

	void PropertyGrid::addUserDefinedPropertyGridEventHandler(PropertyGridEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->propertygrid_userHandlers[EVENT] = handlerName;
	}

	void PropertyGrid::addPropertyGridEventHandler(PropertyGridEvent EVENT, EventHandlerSlot* function)
	{
		mPropertyGridEventHandlers[EVENT].push_back(function);
	}

	void PropertyGrid::clearUserDefinedPropertyGridEventHandler(PropertyGridEvent EVENT)
	{
		mDesc->propertygrid_userHandlers[EVENT] = "";
	}

	PropertyGridSection* PropertyGrid::createSection(PropertyGridSectionDesc* d, int index)
	{
		d->propertygriditem_index = index;

		PropertyGridSection* newProperty = dynamic_cast<PropertyGridSection*>(_createWidget(d));

		addChild(newProperty);

		return newProperty;
	}

	PropertyGridSection* PropertyGrid::createSection(int index)
	{
		return createSection("Section",index);
	}

	PropertyGridSection* PropertyGrid::createSection(const Ogre::UTFString& text, int index)
	{
		PropertyGridSectionDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridSectionDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		PropertyGridSection* n = createSection(d,index);

		d->resetToDefault();

		return n;
	}

	PropertyGridSection* PropertyGrid::createSection(std::vector<TextSegment> segments, int index)
	{
		PropertyGridSectionDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridSectionDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		PropertyGridSection* n = createSection(d,index);

		d->resetToDefault();

		return n;
	}

	bool PropertyGrid::firePropertyGridEvent(PropertyGridEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mPropertyGridEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->propertygrid_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->propertygrid_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	Ogre::String PropertyGrid::getBoolFieldSkin()
	{
		return mSkinType->getSkinReference(BOOLFIELD)->typeName;
	}

	Ogre::String PropertyGrid::getClass()
	{
		return "PropertyGrid";
	}

	Ogre::String PropertyGrid::getComboBoxFieldSkin()
	{
		return mSkinType->getSkinReference(MULTIFIELD)->typeName;
	}

	bool PropertyGrid::getDoubleClickToToggle()
	{
		return mDesc->propertygrid_doubleClickToToggle;
	}

	Ogre::String PropertyGrid::getExpandSkinTypeName()
	{
		return mSkinType->getSkinReference(EXPAND)->typeName;
	}

	bool PropertyGrid::getKeyNavigation()
	{
		return mDesc->propertygrid_keyNavigation;
	}

	int PropertyGrid::getNumberOfSections()
	{
		return static_cast<int>(mSections.size());
	}

	Ogre::String PropertyGrid::getMinimizeSkinTypeName()
	{
		return mSkinType->getSkinReference(MINIMIZE)->typeName;
	}

	float PropertyGrid::getPropertyFieldWidth()
	{
		return mDesc->propertygrid_propertyFieldWidth;
	}

	float PropertyGrid::getItemHeight()
	{
		return mDesc->propertygrid_itemHeight;
	}

	std::list<PropertyGridSection*> PropertyGrid::getRootSections()
	{
		std::list<PropertyGridSection*> list(mSections);

		return list;
	}

	Ogre::String PropertyGrid::getTextFieldSkin()
	{
		return mSkinType->getSkinReference(TEXTFIELD)->typeName;
	}

	PropertyGridItem* PropertyGrid::getSelectedProperty()
	{
		return mSelectedProperty;
	}

	void PropertyGrid::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);

		Rect clipRegion(mTexturePosition,Size(mWidgetDesc->widget_dimensions.size));
		Rect prevClipRegion = brush->getClipRegion();
		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		brush->setClipRegion(prevClipRegion);
	}

	void PropertyGrid::onMouseDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			selectItem(NULL);
	}

	void PropertyGrid::removeEventHandlers(void* obj)
	{
		ContainerWidget::removeEventHandlers(obj);

		for(int index = 0; index < PROPERTYGRID_EVENT_COUNT; ++index)
		{
			std::vector<EventHandlerSlot*> updatedList;
			std::vector<EventHandlerSlot*> listToCleanup;

			for(std::vector<EventHandlerSlot*>::iterator it = mPropertyGridEventHandlers[index].begin(); it != mPropertyGridEventHandlers[index].end(); ++it)
			{
				if((*it)->getClass() == obj)
					listToCleanup.push_back((*it));
				else
					updatedList.push_back((*it));
			}

			mPropertyGridEventHandlers[index].clear();
			for(std::vector<EventHandlerSlot*>::iterator it = updatedList.begin(); it != updatedList.end(); ++it)
				mPropertyGridEventHandlers[index].push_back((*it));

			for(std::vector<EventHandlerSlot*>::iterator it = listToCleanup.begin(); it != listToCleanup.end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void PropertyGrid::selectItem(PropertyGridItem* pgp)
	{
		mSelectedProperty = pgp;
		mDesc->sheet->setKeyboardListener(pgp);

		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
			(*it)->_selectProperty(pgp);

		// Scroll node into view.  Node may be above or below current view,
		// so make sure both top and bottom are visible.
		if(pgp != NULL)
		{
			Point p = pgp->getPositionRelativeToPropertyGrid();

			// scroll top into view
			scrollPointIntoView(p);

			// scroll bottom into view
			p.x += pgp->getWidth();
			p.y += pgp->getHeight();
			scrollPointIntoView(p);
		}

		redraw();

		WidgetEventArgs wea(this);
		firePropertyGridEvent(PROPERTYGRID_EVENT_SELECTION_CHANGED,wea);
	}

	void PropertyGrid::setDoubleClickToToggle(bool enable)
	{
		mDesc->propertygrid_doubleClickToToggle = enable;
	}

	void PropertyGrid::setKeyNavigation(bool enable)
	{
		mDesc->propertygrid_keyNavigation = enable;
	}

	void PropertyGrid::setPropertyFieldWidth(float width)
	{
		mDesc->propertygrid_propertyFieldWidth = width;

		// Update all Sections
		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
			(*it)->notifyWidthUpdated();

		redraw();
	}

	void PropertyGrid::setItemHeight(float heightInPixels)
	{
		heightInPixels = Ogre::Math::Floor(heightInPixels + 0.5);

		mDesc->propertygrid_itemHeight = heightInPixels;

		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
			(*it)->notifyHeightUpdated();

		updateItems();
	}

	void PropertyGrid::updateItems()
	{
		float x = 0;
		float y = 0;
		unsigned int counter = 0;
		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
		{
			(*it)->setPosition(Point(x,y));
			(*it)->mDesc->propertygriditem_index = counter;
			(*it)->updateItems();

			y += (*it)->getBranchHeight();
			++counter;
		}

		mTotalHeight = y;

		updateVirtualDimensions();
		// In case a section has been minimized, re-apply scrolling offsets
		_setScrollX(mHScrollBar->getPercentage());
		_setScrollY(mVScrollBar->getPercentage());
	}

	void PropertyGrid::setSkinType(const Ogre::String type)
	{
		ContainerWidget::setSkinType(type);

		// Update all Sections
		for(std::list<PropertyGridSection*>::iterator it = mSections.begin(); it != mSections.end(); ++it)
			(*it)->updatePropertySkin();
	}

	void PropertyGrid::updateVirtualDimensions()
	{
		if(mDesc == NULL)
			return;

		if(!mDesc->containerwidget_supportScrollBars)
			return;

		mVirtualSize = mClientDimensions.size;

		// Farthest X is the distance from client origin.
		float farthestX = _getFarthestX();
		if(farthestX > mVirtualSize.width)
			mVirtualSize.width = farthestX;

		if(mTotalHeight > mVirtualSize.height)
			mVirtualSize.height = mTotalHeight;

		// Set Slider width/height
		mHScrollBar->setSliderWidth((mClientDimensions.size.width / mVirtualSize.width) * mHScrollBar->getSliderBounds().size.width);
		mVScrollBar->setSliderHeight((mClientDimensions.size.height / mVirtualSize.height) * mVScrollBar->getSliderBounds().size.height);

		// Now that sliders have changed in size, adjust scroll according to slider positions
		_setScrollX(mHScrollBar->getPercentage());
		_setScrollY(mVScrollBar->getPercentage());

		_determineScrollBarVisibility();
	}
}
