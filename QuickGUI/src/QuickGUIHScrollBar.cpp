#include "QuickGUIHScrollBar.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIWindow.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIButton.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIEventHandlerManager.h"

namespace QuickGUI
{
	const Ogre::String HScrollBar::BAR = "bar";
	const Ogre::String HScrollBar::LEFT1 = "left1";
	const Ogre::String HScrollBar::LEFT2 = "left2";
	const Ogre::String HScrollBar::RIGHT1 = "right1";
	const Ogre::String HScrollBar::RIGHT2 = "right2";
	const Ogre::String HScrollBar::SLIDER = "slider";

	void HScrollBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("HScrollBar");
		d->defineSkinElement(BAR);
		d->defineComponent(LEFT1);
		d->defineComponent(LEFT2);
		d->defineComponent(RIGHT1);
		d->defineComponent(RIGHT2);
		d->defineComponent(SLIDER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("HScrollBar",d);
	}

	HScrollBarDesc::HScrollBarDesc() :
		ComponentWidgetDesc()
	{
		resetToDefault();
	}

	void HScrollBarDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(125,25);

		hscrollbar_buttonScrollPercent = 0.1;
		hscrollbar_barScrollPercent = 0.2;
		hscrollbar_scrollBarButtonLayout = HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		hscrollbar_sliderWidth = 15;
		hscrollbar_sliderPercentage = 0;

		for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
			hscrollbar_userHandlers[index] = "";
	}

	void HScrollBarDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("ButtonScrollPercent",&hscrollbar_buttonScrollPercent);
		b->IO("BarScrollPercent",&hscrollbar_barScrollPercent);
		b->IO("ScrollBarButtonLayout",&hscrollbar_scrollBarButtonLayout);
		b->IO("SliderWidth",&hscrollbar_sliderWidth);
		b->IO("SliderPercentage",&hscrollbar_sliderPercentage);

		if(b->begin("UserDefinedHandlers","ScrollBarEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<ScrollBarEvent>(index)),&(hscrollbar_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
				{
					if(hscrollbar_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<ScrollBarEvent>(index)),&(hscrollbar_userHandlers[index]));
				}
			}
			b->end();
		}
	}

	HScrollBar::HScrollBar(const Ogre::String& name) :
		ComponentWidget(name),
		mButton_Left1(NULL),
		mButton_Left2(NULL),
		mButton_Right1(NULL),
		mButton_Right2(NULL),
		mButton_Slider(NULL),
		mDesc(NULL)
	{
	}

	HScrollBar::~HScrollBar()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mScrollBarEventHandlers[index].begin(); it != mScrollBarEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void HScrollBar::_applyButtonLayout()
	{
		if(mSkinElement == NULL)
		{
			mSliderBounds.x = 0;
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.width;

			mButton_Left1->setVisible(false);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(false);
			return;
		}

		switch(mDesc->hscrollbar_scrollBarButtonLayout)
		{
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT:
			mSliderBounds.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mSliderBounds.y = mButton_Left2->getPosition().x;

			mButton_Left1->setVisible(false);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(true);
			mButton_Right2->setVisible(true);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT);

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(true);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(false);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mButton_Left2->getPosition().x;

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(true);
			mButton_Left2->setVisible(true);
			mButton_Right2->setVisible(true);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mButton_Right2->getPosition().x;

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(true);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(true);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT:
			mSliderBounds.x = mButton_Right1->getPosition().x;
			mSliderBounds.y = mButton_Left2->getPosition().x;

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(true);
			mButton_Right2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_NONE:
			mSliderBounds.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT);

			mButton_Left1->setVisible(false);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(false);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:
			mSliderBounds.x = mButton_Right1->getPosition().x;
			mSliderBounds.y = mButton_Right2->getPosition().x;

			mButton_Left1->setVisible(true);
			mButton_Right1->setVisible(false);
			mButton_Left2->setVisible(false);
			mButton_Right2->setVisible(true);
			break;
		}
	}

	void HScrollBar::_checkSliderPosition()
	{
		if(mButton_Slider == NULL)
			return;

		// If button is not visible, don't bother setting position
		if(!mButton_Slider->getVisible())
			return;

		// If the bounds are negative, don't do anything. (This will happen when scrollBar's size is so small the scroll buttons overlap)
		if(mSliderBounds.y < mSliderBounds.x)
			return;

		// Get current position
		Point sliderPosition = mButton_Slider->getPosition();
		// Modify position so that y position is within client area is correct
		sliderPosition.y = 0;

		float sliderWidth = mButton_Slider->getWidth();
		if(sliderPosition.x + sliderWidth > mSliderBounds.y)
			mButton_Slider->setPosition(Point(mSliderBounds.y - sliderWidth,sliderPosition.y));
		else if(sliderPosition.x < mSliderBounds.x)
			mButton_Slider->setPosition(Point(mSliderBounds.x,sliderPosition.y));
		else
			mButton_Slider->setPosition(sliderPosition);

		// Update the percentage: Position of slider within Bounds as a percentage
		sliderPosition = mButton_Slider->getPosition();
		// calculate the total distance the slider can move, ie how far will the slider go at 100%? (Answer: Slider bounds minus Slider width)
		float maxSliderDisplacement = ((mSliderBounds.y - mSliderBounds.x) - mButton_Slider->getWidth());
		// Percentage = displacement / max displacement.  Make sure to gaurd against a 0 max displacement!
		if(maxSliderDisplacement < 0.00000001)
			mDesc->hscrollbar_sliderPercentage = 0.0;
		else
		{
			mDesc->hscrollbar_sliderPercentage = (sliderPosition.x - mSliderBounds.x) / maxSliderDisplacement;
			// Caps percentage to thousandths place
			//mDesc->hscrollbar_sliderPercentage = Ogre::Math::Floor(Ogre::Math::Ceil(mDesc->hscrollbar_sliderPercentage * 1000.0)) / 1000.0;
		}
	}

	void HScrollBar::_determineSliderBounds()
	{
		if(mDesc == NULL)
			return;

		switch(mDesc->hscrollbar_scrollBarButtonLayout)
		{
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_RIGHT:
			mSliderBounds.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mSliderBounds.y = mButton_Left2->getPosition().x;
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_LEFT:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT);
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mButton_Left2->getPosition().x;
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_RIGHT:
			mSliderBounds.x = (mButton_Right1->getPosition().x + mButton_Right1->getWidth());
			mSliderBounds.y = mButton_Right2->getPosition().x;
			break;
		case HSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_LEFT:
			mSliderBounds.x = mButton_Right1->getPosition().x;
			mSliderBounds.y = mButton_Left2->getPosition().x;
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_NONE:
			mSliderBounds.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:
			mSliderBounds.x = mButton_Right1->getPosition().x;
			mSliderBounds.y = mButton_Right2->getPosition().x;
			break;
		}
	}

	void HScrollBar::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<HScrollBarDesc*>(mWidgetDesc);

		HScrollBarDesc* hsd = dynamic_cast<HScrollBarDesc*>(d);

		for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
			mDesc->hscrollbar_userHandlers[index] = hsd->hscrollbar_userHandlers[index];

		mDesc->hscrollbar_barScrollPercent = hsd->hscrollbar_barScrollPercent;
		mDesc->hscrollbar_buttonScrollPercent = hsd->hscrollbar_buttonScrollPercent;

		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();
		bd->widget_resizeFromBottom = false;
		bd->widget_resizeFromLeft = false;
		bd->widget_resizeFromRight = false;
		bd->widget_resizeFromTop = false;
		bd->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		bd->widget_dimensions.size.width = mWidgetDesc->widget_dimensions.size.height;
		bd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		

		mButton_Left1 = dynamic_cast<Button*>(_createWidget(bd));
		mButton_Left1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onLeftButtonClicked,this);
		//mButton_Left1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK,&HScrollBar::onLeftButtonClicked,this);
		
		addComponent(LEFT1,mButton_Left1);
		mButton_Left1->setPositionRelativeToParentClientDimensions(true);

		bd->widget_dimensions.position.x = 15;
		mButton_Right1 = dynamic_cast<Button*>(_createWidget(bd));
		mButton_Right1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onRightButtonClicked,this);
		//mButton_Right1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK,&HScrollBar::onRightButtonClicked,this);
		
		addComponent(RIGHT1,mButton_Right1);
		mButton_Right1->setPositionRelativeToParentClientDimensions(true);

		bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
		bd->widget_dimensions.position.x = mWidgetDesc->widget_dimensions.size.width - (2 * bd->widget_dimensions.size.width);
		mButton_Left2 = dynamic_cast<Button*>(_createWidget(bd));
		mButton_Left2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onLeftButtonClicked,this);
		//mButton_Left2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK,&HScrollBar::onLeftButtonClicked,this);
	
		addComponent(LEFT2,mButton_Left2);
		mButton_Left2->setPositionRelativeToParentClientDimensions(true);

		bd->widget_dimensions.position.x = mWidgetDesc->widget_dimensions.size.width - bd->widget_dimensions.size.width;
		mButton_Right2 = dynamic_cast<Button*>(_createWidget(bd));
		mButton_Right2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onRightButtonClicked,this);
		//mButton_Right2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK,&HScrollBar::onRightButtonClicked,this);
	
		addComponent(RIGHT2,mButton_Right2);
		mButton_Right2->setPositionRelativeToParentClientDimensions(true);

		bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		bd->widget_dimensions.position.x = 30;
		bd->widget_dragable = true;
		mButton_Slider = dynamic_cast<Button*>(_createWidget(bd));
		addComponent(SLIDER,mButton_Slider);
		mButton_Slider->setPositionRelativeToParentClientDimensions(true);

		mButton_Slider->addWidgetEventHandler(WIDGET_EVENT_DRAGGED,&HScrollBar::onSliderDragged,this);

		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&HScrollBar::onBarClicked,this);

		setSkinType(hsd->widget_skinTypeName);
		setButtonLayout(hsd->hscrollbar_scrollBarButtonLayout);
		setPercentage(hsd->hscrollbar_sliderPercentage);
	}

	void HScrollBar::addUserDefinedScrollBarEventHandler(ScrollBarEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->hscrollbar_userHandlers[EVENT] = handlerName;
	}

	void HScrollBar::addScrollBarEventHandler(ScrollBarEvent EVENT, EventHandlerSlot* function)
	{
		mScrollBarEventHandlers[EVENT].push_back(function);
	}

	void HScrollBar::clearUserDefinedScrollBarEventHandler(ScrollBarEvent EVENT)
	{
		mDesc->hscrollbar_userHandlers[EVENT] = "";
	}

	bool HScrollBar::fireScrollBarEvent(ScrollBarEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mScrollBarEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->hscrollbar_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->hscrollbar_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	float HScrollBar::getBarScrollPercent()
	{
		return mDesc->hscrollbar_barScrollPercent;
	}

	HScrollBarButtonLayout HScrollBar::getButtonLayout()
	{
		return mDesc->hscrollbar_scrollBarButtonLayout;
	}

	float HScrollBar::getButtonScrollPercent()
	{
		return mDesc->hscrollbar_buttonScrollPercent;
	}

	Ogre::String HScrollBar::getClass()
	{
		return "HScrollBar";
	}

	float HScrollBar::getPercentage()
	{
		return mDesc->hscrollbar_sliderPercentage;
	}

	Rect HScrollBar::getSliderBounds()
	{
		return Rect(mSliderBounds.x,mClientDimensions.position.y,mSliderBounds.y - mSliderBounds.x,mClientDimensions.size.height);
	}

	void HScrollBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void HScrollBar::onLeftButtonClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_buttonScrollPercent);
		else
			setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_buttonScrollPercent);
	}

	void HScrollBar::onRightButtonClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_buttonScrollPercent);
		else
			setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_buttonScrollPercent);
	}

	void HScrollBar::onBarClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mWindow == NULL)
			return;

		Point p = mea.position - mWindow->getPosition() - mTexturePosition;
		bool left = (p.x < mButton_Slider->getPosition().x);

		setPercentage(p.x/this->getWidth());

		/*if(mea.button == MB_Left)
		{
			if(left)
				setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_barScrollPercent);
			else
				setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_barScrollPercent);
		}
		else if(mea.button == MB_Right)
		{
			if(left)
				setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_barScrollPercent);
			else
				setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_barScrollPercent);
		}*/
	}

	void HScrollBar::onSliderDragged(const EventArgs& args)
	{
		_checkSliderPosition();

		WidgetEventArgs wea(this);
		fireScrollBarEvent(SCROLLBAR_EVENT_ON_SCROLLED,wea);
	}

	void HScrollBar::setBarScrollPercent(float percent)
	{
		mDesc->hscrollbar_barScrollPercent = percent;
	}

	void HScrollBar::setButtonLayout(HScrollBarButtonLayout l)
	{
		mDesc->hscrollbar_scrollBarButtonLayout = l;

		// Toggle visibility for buttons and set the slider bounds according to layout and button positions
		_applyButtonLayout();
		// Try to maintain the same percentage, given the new layout.
		setPercentage(mDesc->hscrollbar_sliderPercentage);
	}

	void HScrollBar::setButtonScrollPercent(float percent)
	{
		mDesc->hscrollbar_buttonScrollPercent = percent;
	}

	void HScrollBar::setPercentage(float percentage)
	{
		// Cap percentage within bounds: [0.0,1.0]
		if(percentage <= 0.0)
			percentage = 0.0;
		else if(percentage > 1.0)
			percentage = 1.0;
		//else // Caps percentage to thousandths place
			//percentage = Ogre::Math::Floor(Ogre::Math::Ceil(percentage * 1000.0)) / 1000.0;

		// Set percentage property
		mDesc->hscrollbar_sliderPercentage = percentage;

		// calculate the total distance the slider can move, ie how far will the slider go at 100%? (Answer: Slider bounds minus Slider width)
		float maxSliderDisplacement = ((mSliderBounds.y - mSliderBounds.x) - mButton_Slider->getWidth());

		// Position slider according to percentage:
		//   At 0.0, slider is at beginning of slider bounds.
		//   At 1.0, slider is at end of slider bounds.
		mButton_Slider->setPosition(Point(mSliderBounds.x + (maxSliderDisplacement * percentage),0));

		WidgetEventArgs args(this);
		fireScrollBarEvent(SCROLLBAR_EVENT_ON_SCROLLED,args);
	}

	void HScrollBar::setSkinType(const Ogre::String type)
	{
		if(!SkinTypeManager::getSingleton().hasSkinType(getClass(),type))
			throw Exception(Exception::ERR_SKINNING,"SkinType \"" + type + "\" does not exist!","Widget::_setSkinType");

		mSkinType = SkinTypeManager::getSingleton().getSkinType(getClass(),type);
		updateSkinElement();

		mWidgetDesc->widget_skinTypeName = type;
#if USEHASHMAPS
		for(std::unordered_map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getSkinReference((*it).first)->typeName);
#else
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getSkinReference((*it).first)->typeName);
#endif
		// Update client dimensions
		{
			mClientDimensions.position = Point::ZERO;
			mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

			if(mSkinElement != NULL)
			{
				mClientDimensions.position.x = mSkinElement->getBorderThickness(BORDER_LEFT);
				mClientDimensions.position.y = mSkinElement->getBorderThickness(BORDER_TOP);
				mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
				mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));
			}
		}

		// Update button Sizes - same width and height for each, except slider, where only height is modified
		float buttonThickness = mClientDimensions.size.height;
		Size s(buttonThickness,buttonThickness);

		mButton_Left1->setSize(s);
		mButton_Right1->setSize(s);
		mButton_Left2->setSize(s);
		mButton_Right2->setSize(s);
		mButton_Slider->setHeight(mClientDimensions.size.height);

		// Update button positions
		Point p;
		mButton_Left1->setPosition(p);

		p.x += buttonThickness;
		mButton_Right1->setPosition(p);

		p.x = mClientDimensions.size.width - buttonThickness - buttonThickness;
		mButton_Left2->setPosition(p);

		p.x += buttonThickness;
		mButton_Right2->setPosition(p);

		p.x = mButton_Slider->getPosition().x;
		mButton_Slider->setPosition(p);

		_determineSliderBounds();
		// Now that bounds are properly updated, restore percentage of the scrollbar
		setPercentage(mDesc->hscrollbar_sliderPercentage);

		WidgetEventArgs wea(this);
		fireWidgetEvent(WIDGET_EVENT_SKIN_CHANGED,wea);
	}

	void HScrollBar::scrollLeft()
	{
		setPercentage(mDesc->hscrollbar_sliderPercentage - mDesc->hscrollbar_buttonScrollPercent);
	}

	void HScrollBar::scrollToEnd()
	{
		setPercentage((mSliderBounds.y - mButton_Slider->getWidth() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x));
	}

	void HScrollBar::scrollRight()
	{
		setPercentage(mDesc->hscrollbar_sliderPercentage + mDesc->hscrollbar_buttonScrollPercent);
	}

	void HScrollBar::setSliderWidth(float width)
	{
		mButton_Slider->setWidth(width);

		_checkSliderPosition();
	}

	void HScrollBar::updateClientDimensions()
	{
		ComponentWidget::updateClientDimensions();

		// Update Slider Bounds, reliant on button layout and position.  In this situation
		// Button positions may have changed.
		_determineSliderBounds();
		// Make sure Slider is within slider bounds
		_checkSliderPosition();
	}

	void HScrollBar::updateSkinElement()
	{
		mSkinElement = NULL;

		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(BAR);
	}

	Ogre::list<QuickGUI::Widget*>::type HScrollBar::getScrollbarWidgets()
	{
		Ogre::list<QuickGUI::Widget*>::type sc;
		if(mButton_Left1 != NULL)
			sc.push_back(static_cast<QuickGUI::Widget*>(mButton_Left1));
		if(mButton_Left2 != NULL)
				sc.push_back(static_cast<QuickGUI::Widget*>(mButton_Left2));
		if(mButton_Right1 != NULL)
				sc.push_back(static_cast<QuickGUI::Widget*>(mButton_Right1));
		if(mButton_Right2 != NULL)
				sc.push_back(static_cast<QuickGUI::Widget*>(mButton_Right2));
		if(mButton_Slider != NULL)
				sc.push_back(static_cast<QuickGUI::Widget*>(mButton_Slider));


		return sc;
	}
	
	
	void HScrollBar::setHiddenID(int ID)
	{
		hiddenInt = ID;
	}

	int HScrollBar::getHiddenID()
	{
		return hiddenInt;
	}

	void HScrollBar::setHiddenString(Ogre::String str)
	{
		hiddenString = str;
	}

	Ogre::String HScrollBar::getHiddenString()
	{
		return hiddenString;
	}

}

