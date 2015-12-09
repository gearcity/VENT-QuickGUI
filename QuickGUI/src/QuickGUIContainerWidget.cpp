#include "QuickGUIContainerWidget.h"
#include "QuickGUIBrush.h"
#include "QuickGUISerialWriter.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIScriptDefinition.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIVScrollBar.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIRoot.h"

namespace QuickGUI
{
	const Ogre::String ContainerWidget::HSCROLLBAR = "hscrollbar";
	const Ogre::String ContainerWidget::VSCROLLBAR = "vscrollbar";

	ContainerWidgetDesc::ContainerWidgetDesc() :
		ComponentWidgetDesc()
	{
		resetToDefault();
	}

	void ContainerWidgetDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		containerwidget_clipChildrenToDimensions = true;
		containerwidget_horzBarScrollPercent = 0.1;
		containerwidget_horzButtonScrollPercent = 0.02;
		containerwidget_supportScrollBars = true;
		containerwidget_scrollBarThickness = 15;
		containerwidget_vertBarScrollPercent = 0.1;
		containerwidget_vertButtonScrollPercent = 0.02;
		containerwidget_xScrollOffset = 0;
		containerwidget_yScrollOffset = 0;
	}

	void ContainerWidgetDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("ClipChildrenToDimensions",&containerwidget_clipChildrenToDimensions);
		b->IO("HorzBarScrollPercent",&containerwidget_horzBarScrollPercent);
		b->IO("HorzButtonScrollPercent",&containerwidget_horzButtonScrollPercent);
		b->IO("SupportScrolling",&containerwidget_supportScrollBars);
		b->IO("ScrollBarThickness",&containerwidget_scrollBarThickness);
		b->IO("VertBarScrollPercent",&containerwidget_vertBarScrollPercent);
		b->IO("VertButtonScrollPercent",&containerwidget_vertButtonScrollPercent);
		b->IO("XScrollOffset",&containerwidget_xScrollOffset);
		b->IO("YScrollOffset",&containerwidget_yScrollOffset);
	}

	ContainerWidget::ContainerWidget(const Ogre::String& name) :
		ComponentWidget(name),
		mDesc(NULL),
		mHScrollBar(NULL),
		mVScrollBar(NULL),
		mUpdatingClientDimensions(false)
	{
	}

	ContainerWidget::~ContainerWidget()
	{
		WidgetFactory* f = FactoryManager::getSingleton().getWidgetFactory();
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			f->destroyInstance((*it));
	}

	void ContainerWidget::_determineScrollBarVisibility()
	{
		if(mDesc == NULL)
			return;

		if(!mDesc->containerwidget_supportScrollBars)
			return;

		bool clientDimensionsNeedToBeUpdated = false;

		if(static_cast<int>(mVirtualSize.width) == static_cast<int>(mClientDimensions.size.width))
		{
			if(mHScrollBar->getVisible())
			{
				mHScrollBar->setVisible(false);
				clientDimensionsNeedToBeUpdated = true;
			}
		}
		else
		{
			if(!mHScrollBar->getVisible())
			{
				mHScrollBar->setVisible(true);
				clientDimensionsNeedToBeUpdated = true;
			}
		}

		if(static_cast<int>(mVirtualSize.height) == static_cast<int>(mClientDimensions.size.height))
		{
			if(mVScrollBar->getVisible())
			{
				mVScrollBar->setVisible(false);
				clientDimensionsNeedToBeUpdated = true;
			}
		}
		else
		{
			if(!mVScrollBar->getVisible())
			{
				mVScrollBar->setVisible(true);
				clientDimensionsNeedToBeUpdated = true;
			}
		}

		if(clientDimensionsNeedToBeUpdated)
			updateClientDimensions();
	}

	void ContainerWidget::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<ContainerWidgetDesc*>(mWidgetDesc);
		ContainerWidgetDesc* cwd = dynamic_cast<ContainerWidgetDesc*>(d);

		mDesc->containerwidget_clipChildrenToDimensions = cwd->containerwidget_clipChildrenToDimensions;
		mDesc->containerwidget_supportScrollBars = cwd->containerwidget_supportScrollBars;
		mDesc->containerwidget_scrollBarThickness = cwd->containerwidget_scrollBarThickness;
		mDesc->containerwidget_horzBarScrollPercent = cwd->containerwidget_horzBarScrollPercent;
		mDesc->containerwidget_horzButtonScrollPercent = cwd->containerwidget_horzButtonScrollPercent;
		mDesc->containerwidget_vertBarScrollPercent = cwd->containerwidget_vertBarScrollPercent;
		mDesc->containerwidget_vertButtonScrollPercent = cwd->containerwidget_vertButtonScrollPercent;

		if(mDesc->containerwidget_supportScrollBars)
		{
			HScrollBarDesc* hd = DescManager::getSingletonPtr()->getDefaultHScrollBarDesc();
			hd->resetToDefault();
			hd->widget_dimensions.size.height = mDesc->containerwidget_scrollBarThickness;
			hd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
			hd->widget_verticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			hd->hscrollbar_barScrollPercent = mDesc->containerwidget_horzBarScrollPercent;
			hd->hscrollbar_buttonScrollPercent = mDesc->containerwidget_horzButtonScrollPercent;
			hd->widget_visible = false;

			mHScrollBar = dynamic_cast<HScrollBar*>(_createWidget(hd));
			mHScrollBar->addScrollBarEventHandler(QuickGUI::SCROLLBAR_EVENT_ON_SCROLLED,&ContainerWidget::onHorizontalScroll,this);
			addComponent(HSCROLLBAR,mHScrollBar);

			VScrollBarDesc* vd = DescManager::getSingletonPtr()->getDefaultVScrollBarDesc();
			vd->resetToDefault();
			vd->widget_dimensions.size.width = mDesc->containerwidget_scrollBarThickness;
			vd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			vd->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
			vd->vscrollbar_barScrollPercent = mDesc->containerwidget_vertBarScrollPercent;
			vd->vscrollbar_buttonScrollPercent = mDesc->containerwidget_vertButtonScrollPercent;
			vd->widget_visible = false;

			mVScrollBar = dynamic_cast<VScrollBar*>(_createWidget(vd));
			mVScrollBar->addScrollBarEventHandler(QuickGUI::SCROLLBAR_EVENT_ON_SCROLLED,&ContainerWidget::onVerticalScroll,this);
			addComponent(VSCROLLBAR,mVScrollBar);

			addWidgetEventHandler(WIDGET_EVENT_MOUSE_WHEEL,&ContainerWidget::onMouseWheel,this);
		}
	}

	void ContainerWidget::_setGUIManager(GUIManager* gm)
	{
		ComponentWidget::_setGUIManager(gm);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->_setGUIManager(gm);
	}

	void ContainerWidget::_setSheet(Sheet* sheet)
	{
		ComponentWidget::_setSheet(sheet);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->_setSheet(sheet);
	}

	void ContainerWidget::_setScrollX(float percentage)
	{
		if(!mDesc->containerwidget_supportScrollBars)
			return;

		// Cap percentage within bounds: [0.0,1.0]
		if(percentage <= 0.0)
			percentage = 0.0;
		else if(percentage > 1.0)
			percentage = 1.0;
		else // Caps percentage to thousandths place
			percentage = Ogre::Math::Floor(Ogre::Math::Ceil(percentage * 1000.0)) / 1000.0;

		// calculate the total distance the View can be offset, ie how far will the view offset be at 100%? (Answer: Virtual width minus actual width)
		float maxViewDisplacement = (mVirtualSize.width - mClientDimensions.size.width);

		// Offset horizontal view according to percentage:
		//   At 0.0, horizontal view is not offset at all.
		//   At 1.0, horizontal view is offset to maximum.
		mDesc->containerwidget_xScrollOffset = (percentage * maxViewDisplacement);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setScrollX(mDesc->containerwidget_xScrollOffset);
		}
	}

	void ContainerWidget::_setScrollY(float percentage)
	{
		if(!mDesc->containerwidget_supportScrollBars)
			return;

		// Cap percentage within bounds: [0.0,1.0]
		if(percentage <= 0.0)
			percentage = 0.0;
		else if(percentage > 1.0)
			percentage = 1.0;
		else // Caps percentage to thousandths place
			percentage = Ogre::Math::Floor(Ogre::Math::Ceil(percentage * 1000.0)) / 1000.0;

		// calculate the total distance the View can be offset, ie how far will the view offset be at 100%? (Answer: Virtual height minus actual height)
		float maxViewDisplacement = (mVirtualSize.height - mClientDimensions.size.height);

		// Offset vertical view according to percentage:
		//   At 0.0, vertical view is not offset at all.
		//   At 1.0, vertical view is offset to maximum.
		mDesc->containerwidget_yScrollOffset = (percentage * maxViewDisplacement);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setScrollY(mDesc->containerwidget_yScrollOffset);
		}
	}

	void ContainerWidget::addChild(Widget* w)
	{
		mChildren.push_back(w);
		w->setParent(this);
		w->_setGUIManager(mWidgetDesc->guiManager);
		w->_setSheet(mWidgetDesc->sheet);

		if(mDesc->containerwidget_supportScrollBars)
		{
			// Register Event Handler for when child is resized, moved, or visibility changes
			w->addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&ContainerWidget::onChildVisibilityChanged,this);
			w->addWidgetEventHandler(WIDGET_EVENT_SIZE_CHANGED,&ContainerWidget::onChildDimensionsChanged,this);
			w->addWidgetEventHandler(WIDGET_EVENT_POSITION_CHANGED,&ContainerWidget::onChildDimensionsChanged,this);

			Point tempPoint = w->getPosition();
			Size tempSize = w->getSize();

			if((tempPoint.x + tempSize.width) > mVirtualSize.width)
				mVirtualSize.width = (tempPoint.x + tempSize.width);
			if((tempPoint.y + tempSize.height) > mVirtualSize.height)
				mVirtualSize.height = (tempPoint.y + tempSize.height);

			updateVirtualDimensions();
		}
	}

	void ContainerWidget::destroyChild(Widget* w)
	{
		removeChild(w);

		if(mDesc->sheet != NULL)
			mDesc->sheet->mFreeList.push_back(w);
		else
			Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back(w);

		if(mDesc->containerwidget_supportScrollBars)
			updateVirtualDimensions();
	}

	void ContainerWidget::destroyChildren()
	{
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->_setSheet(NULL);
			(*it)->_setGUIManager(NULL);
			(*it)->setParent(NULL);

			if(mDesc->containerwidget_supportScrollBars)
			{
				// Register Event Handler for when child is resized, moved, or visibility changed
				(*it)->removeEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,this);
				(*it)->removeEventHandler(WIDGET_EVENT_SIZE_CHANGED,this);
				(*it)->removeEventHandler(WIDGET_EVENT_POSITION_CHANGED,this);
			}

			(*it)->destroy();

		}
		mChildren.clear();

		if(mDesc->containerwidget_supportScrollBars)
			updateVirtualDimensions();
	}

	void ContainerWidget::draw()
	{
		// check visibility
		if( !mWidgetDesc->widget_visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// store clip region
		Rect prevClipRegion = brush->getClipRegion();

		// set clip region to dimensions
		Rect widgetClipRegion = Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size);
		Rect derivedClipRegion = widgetClipRegion.getIntersection(prevClipRegion);
		brush->setClipRegion(derivedClipRegion);

		// Set color before drawing operations
		brush->setColor(mWidgetDesc->widget_baseColor);

		// Set opacity before drawing operations
		brush->setOpacity(getAbsoluteOpacity());

		// check and clip region - do not draw if we are outside clipping region
		if (derivedClipRegion != Rect::ZERO )
		{
			// draw self
			onDraw();
		}

		// See if we need to set clipping dimensions for children (normal scenario)
		if(mDesc->containerwidget_clipChildrenToDimensions)
		{
			// set clip region to client dimensions
			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);
			brush->setClipRegion(clipRegion.getIntersection(prevClipRegion));
		}
		else
			// make sure clip region is restored to previous clip region
			brush->setClipRegion(prevClipRegion);

		// draw children
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->draw();

		// Set clipping region to widget bounds
		brush->setClipRegion(derivedClipRegion);

		// draw components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->draw();

		// restore clip region
		brush->setClipRegion(prevClipRegion);
	}

	Widget* ContainerWidget::findFirstWidgetOfClass(const Ogre::String& className)
	{
		Widget* w = NULL;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			w = (*it)->findFirstWidgetOfClass(className);
			if(w != NULL)
				return w;
		}

		return Widget::findFirstWidgetOfClass(className);
	}

	Widget* ContainerWidget::findWidget(const Ogre::String& name)
	{
		Widget* w;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			w = (*it)->findWidget(name);
			if(w != NULL)
				return w;
		}

		return ComponentWidget::findWidget(name);
	}

	Widget* ContainerWidget::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		// If we are not widget_visible, return NULL
		if(!mWidgetDesc->widget_visible)
			return NULL;

		// If we ignore disabled and this widget is !widget_enabled, return NULL
		if(ignoreDisabled && !mWidgetDesc->widget_enabled)
			return NULL;

		// Check components before verifying point is within bounds. (Components can lie outside widget dimensions)
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			Widget* w = (*it).second->findWidgetAtPoint(p,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		// See if point is within widget bounds
		if(!Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).isPointWithinBounds(p))
			return NULL;

		// Get the client bounds as displayed on the texture
		Rect clientBounds(mTexturePosition,mClientDimensions.size);
		clientBounds.translate(mClientDimensions.position);

		if(clientBounds.isPointWithinBounds(p))
		{
			for(std::vector<Widget*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
			{
				Widget* w = (*it)->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}
		}
				
		// Take transparency picking into account
		if(mWidgetDesc->widget_transparencyPicking && (mSkinType != NULL))
		{
			// If the background is transparent, return NULL
			if((mSkinElement == NULL) || (mSkinElement->getTextureName() == ""))
				return NULL;

			// Get relative position
			Point relPos = p - mTexturePosition;
			// Get percentage of position relative to widget widget_dimensions
			relPos.x = relPos.x / mWidgetDesc->widget_dimensions.size.width;
			relPos.y = relPos.y / mWidgetDesc->widget_dimensions.size.height;
			// Get pixel position of texture
			Point pixelPos;
			pixelPos.x = relPos.x * (mSkinElement->getWidth() - 1);
			pixelPos.y = relPos.y * (mSkinElement->getHeight() - 1);
			
			if(mSkinElement->transparentPixel(pixelPos))
				return NULL;
		}

		return this;
	}

	std::vector<Widget*> ContainerWidget::getChildren()
	{
		std::vector<Widget*> list(mChildren);

		return list;
	}

	bool ContainerWidget::getClipChildrenToDimensions()
	{
		return mDesc->containerwidget_clipChildrenToDimensions;
	}

	bool ContainerWidget::isContainerWidget()
	{
		return true;
	}

	void ContainerWidget::onChildVisibilityChanged(const EventArgs& args)
	{
		if(mUpdatingClientDimensions)
			return;

		updateVirtualDimensions();
	}

	void ContainerWidget::onChildDimensionsChanged(const EventArgs& args)
	{
		if(mUpdatingClientDimensions)
			return;

		updateVirtualDimensions();
	}

	void ContainerWidget::onHorizontalScroll(const EventArgs& args)
	{
		_setScrollX(mHScrollBar->getPercentage());
	}

	void ContainerWidget::onMouseWheel(const EventArgs& args)
	{
		if((mVScrollBar == NULL) || !mVScrollBar->getVisible())
			return;

		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.wheelChange > 0)
			mVScrollBar->scrollUp();
		else
			mVScrollBar->scrollDown();
	}

	void ContainerWidget::onVerticalScroll(const EventArgs& args)
	{
		_setScrollY(mVScrollBar->getPercentage());
	}

	void ContainerWidget::removeChild(Widget* w)
	{
		std::vector<Widget*>::iterator it = std::find(mChildren.begin(),mChildren.end(),w);
		if(it == mChildren.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Widget \"" + w->getName() + "\" is not a child of widget \"" + getName() + "\"","ContainerWidget::removeChild");

		mChildren.erase(it);

		w->_setSheet(NULL);
		w->_setGUIManager(NULL);
		w->setParent(NULL);

		if(mDesc->containerwidget_supportScrollBars)
		{
			// Register Event Handler for when child is resized, moved, or visibility changed
			w->removeEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,this);
			w->removeEventHandler(WIDGET_EVENT_SIZE_CHANGED,this);
			w->removeEventHandler(WIDGET_EVENT_POSITION_CHANGED,this);

			updateVirtualDimensions();
		}
	}

	void ContainerWidget::removeChildren()
	{
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->_setSheet(NULL);
			(*it)->_setGUIManager(NULL);
			(*it)->setParent(NULL);

			if(mDesc->containerwidget_supportScrollBars)
			{
				// Register Event Handler for when child is resized, moved, or visibility changed
				(*it)->removeEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,this);
				(*it)->removeEventHandler(WIDGET_EVENT_SIZE_CHANGED,this);
				(*it)->removeEventHandler(WIDGET_EVENT_POSITION_CHANGED,this);
			}

			if(mDesc->sheet != NULL)
				mDesc->sheet->mFreeList.push_back((*it));
			else
				Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back((*it));
		}
		mChildren.clear();

		if(mDesc->containerwidget_supportScrollBars)
			updateVirtualDimensions();
	}

	void ContainerWidget::scrollChildIntoView(Widget* child)
	{
		std::vector<Widget*>::iterator it = std::find(mChildren.begin(),mChildren.end(),child);
		if(it == mChildren.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Widget \"" + child->getName() + "\" is not a child of widget \"" + getName() + "\"","ContainerWidget::scrollChildIntoView");

		Point scrollAmount = child->getScroll();
		Point relativePosition = child->getPosition();

		scrollPointIntoView(relativePosition + scrollAmount);
	}

	void ContainerWidget::scrollPointIntoView(const Point& p)
	{
		Point pointFromVirtualArea = p - Point(mDesc->containerwidget_xScrollOffset,mDesc->containerwidget_yScrollOffset);

		// Check x for scrolling

		// calculate the total distance the View can be offset, ie how far will the view offset be at 100%? (Answer: Virtual width minus actual width)
		float maxWidthDisplacement = (mVirtualSize.width - mClientDimensions.size.width);

		// Check if we need to scroll left
		if(pointFromVirtualArea.x < 0)
		{
			if(mHScrollBar != NULL)
				mHScrollBar->setPercentage(p.x / maxWidthDisplacement);
		}
		// Check if we need to scroll right
		else if(pointFromVirtualArea.x > mClientDimensions.size.width)
		{
			if(mHScrollBar != NULL)
				mHScrollBar->setPercentage((p.x - mClientDimensions.size.width) / maxWidthDisplacement);
		}

		// Check y for scrolling

		// calculate the total distance the View can be offset, ie how far will the view offset be at 100%? (Answer: Virtual height minus actual height)
		float maxHeightDisplacement = (mVirtualSize.height - mClientDimensions.size.height);
		
		// Check if we need to scroll up
		if(pointFromVirtualArea.y < 0)
		{
			if(mVScrollBar != NULL)
				mVScrollBar->setPercentage(p.y / maxHeightDisplacement);
		}
		// Check if we need to scroll down
		else if(pointFromVirtualArea.y > mClientDimensions.size.height)
		{
			if(mVScrollBar != NULL)
				mVScrollBar->setPercentage((p.y - mClientDimensions.size.height) / maxHeightDisplacement);
		}
	}

	void ContainerWidget::serialize(SerialBase* b)
	{
		// Create internal Desc object if it doesn't exist
		_createDescObject();

		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		if(b->isSerialReader())
			// Apply Desc properties
			_initialize(mWidgetDesc);

		if(b->begin("Child","Widgets"))
		{
			// If we are reading, we need to create and populate widgets
			if(b->isSerialReader())
			{
				std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions();
				for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
				{
					// Create Empty Widget, supplying class name and widget name from script
					Widget* newWidget = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>((*it)->getType(),(*it)->getID());

					// Populate Desc object from Script Text, and initialize widget
					newWidget->serialize(b);

					// Add as child Widget
					addChild(newWidget);
				}
			}
			// If we are writing, we need to serialize children
			else
			{
				for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
				{
					(*it)->serialize(b);
				}
			}
			b->end();
		}

		b->end();
	}

	void ContainerWidget::setClipChildrenToDimensions(bool clip)
	{
		mDesc->containerwidget_clipChildrenToDimensions = clip;

		redraw();
	}

	void ContainerWidget::setHorzBarScrollPercent(float percent)
	{
		percent = Ogre::Math::Floor(Ogre::Math::Ceil(percent * 1000.0)) / 1000.0;

		mDesc->containerwidget_horzBarScrollPercent = percent;

		if(mHScrollBar)
			mHScrollBar->setBarScrollPercent(percent);
	}

	void ContainerWidget::setHorzButtonScrollPercent(float percent)
	{
		percent = Ogre::Math::Floor(Ogre::Math::Ceil(percent * 1000.0)) / 1000.0;

		mDesc->containerwidget_horzButtonScrollPercent = percent;

		if(mHScrollBar)
			mHScrollBar->setButtonScrollPercent(percent);
	}

	void ContainerWidget::setSkinType(const Ogre::String type)
	{
		mWidgetDesc->widget_skinTypeName = type;

		_setSkinType(type);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if(mSkinType->hasSkinReference((*it).first))
				(*it).second->setSkinType(mSkinType->getSkinReference((*it).first)->typeName);
		}
	}

	bool ContainerWidget::supportsScrolling()
	{
		return mDesc->containerwidget_supportScrollBars;
	}

	void ContainerWidget::setVertBarScrollPercent(float percent)
	{
		percent = Ogre::Math::Floor(Ogre::Math::Ceil(percent * 1000.0)) / 1000.0;

		mDesc->containerwidget_vertBarScrollPercent = percent;

		if(mVScrollBar)
			mVScrollBar->setBarScrollPercent(percent);
	}

	void ContainerWidget::setVertButtonScrollPercent(float percent)
	{
		percent = Ogre::Math::Floor(Ogre::Math::Ceil(percent * 1000.0)) / 1000.0;

		mDesc->containerwidget_vertButtonScrollPercent = percent;

		if(mVScrollBar)
			mVScrollBar->setButtonScrollPercent(percent);
	}

	void ContainerWidget::setParent(Widget* parent)
	{
		ComponentWidget::setParent(parent);

		// Update children's window reference via setParent
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setParent(this);
		}
	}

	void ContainerWidget::updateClientDimensions()
	{
		mUpdatingClientDimensions = true;

		Size previousSize = mClientDimensions.size;

		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinElement != NULL)
		{
			// Default
			mClientDimensions.position.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = mSkinElement->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));

			// First we want to adjust the client dimensions, depending on whether the scrollbars are visible.

			// If HorizontalScrollBar is visible, update its dimensions and update client dimensions
			if(mHScrollBar && mHScrollBar->getVisible())
				mClientDimensions.size.height -= mHScrollBar->getHeight();

			// If VerticalScrollBar is visible, update its dimensions and update client dimensions
			if(mVScrollBar && mVScrollBar->getVisible())
				mClientDimensions.size.width -= mVScrollBar->getWidth();

			// Now that client dimensions are fully updated, we can adjust ScrollBars position and size

			if(mHScrollBar)
			{
				mHScrollBar->setWidth(mClientDimensions.size.width);
				mHScrollBar->setPosition(Point(mSkinElement->getBorderThickness(BORDER_LEFT),mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM) - mDesc->containerwidget_scrollBarThickness));
			}

			if(mVScrollBar)
			{
				mVScrollBar->setHeight(mClientDimensions.size.height);
				mVScrollBar->setPosition(Point(mWidgetDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT) - mVScrollBar->getWidth(),mSkinElement->getBorderThickness(BORDER_TOP)));
			}
		}

		// Get difference.  A larger size indicates a positive difference
		Size difference = mClientDimensions.size - previousSize;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if((*it).second == mHScrollBar)
				continue;

			if((*it).second == mVScrollBar)
				continue;

			applyAnchor((*it).second,difference);
		}

		// Handle Anchoring for children
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			applyAnchor((*it),difference);
		}

		mUpdatingClientDimensions = false;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		// Determine Virtual Size.  This function also calls _determineScrollBarVisibility, which checks if
		// ScrollBars should be shown/hidden.  If scrollBar visibility changes, so does the client area.  In
		// this case, updateClientDimensions will be called again.
		updateVirtualDimensions();

		redraw();
	}

	void ContainerWidget::updateTexturePosition()
	{
		// Derive texture position and client dimensions
		Widget::updateTexturePosition();

		// Update component screen dimensions, must be done after client and screen rect have been calculated
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			(*it).second->updateTexturePosition();
		}

		// Update children screen dimensions, must be done after client and screen rect have been calculated
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->updateTexturePosition();
		}
	}

	void ContainerWidget::updateVirtualDimensions()
	{
		if(mDesc == NULL)
			return;

		if(!mDesc->containerwidget_supportScrollBars)
			return;

		mVirtualSize = mClientDimensions.size;

		Point tempPoint;
		Size tempSize;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			if((*it)->getVisible())
			{
				tempPoint = (*it)->getPosition();
				tempSize = (*it)->getSize();

				if((tempPoint.x + tempSize.width) > mVirtualSize.width)
					mVirtualSize.width = (tempPoint.x + tempSize.width);
				if((tempPoint.y + tempSize.height) > mVirtualSize.height)
					mVirtualSize.height = (tempPoint.y + tempSize.height);
			}
		}

		// Set Slider width/height
		mHScrollBar->setSliderWidth((mClientDimensions.size.width / mVirtualSize.width) * mHScrollBar->getSliderBounds().size.width);
		mVScrollBar->setSliderHeight((mClientDimensions.size.height / mVirtualSize.height) * mVScrollBar->getSliderBounds().size.height);

		// Now that sliders have changed in size, adjust scroll according to slider positions
		_setScrollX(mHScrollBar->getPercentage());
		_setScrollY(mVScrollBar->getPercentage());

		_determineScrollBarVisibility();
	}
}
