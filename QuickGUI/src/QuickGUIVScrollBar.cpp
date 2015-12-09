#include "QuickGUIVScrollBar.h"
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
	const Ogre::String VScrollBar::BAR = "bar";
	const Ogre::String VScrollBar::UP1 = "up1";
	const Ogre::String VScrollBar::UP2 = "up2";
	const Ogre::String VScrollBar::DOWN1 = "down1";
	const Ogre::String VScrollBar::DOWN2 = "down2";
	const Ogre::String VScrollBar::SLIDER = "slider";

	void VScrollBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("VScrollBar");
		d->defineSkinElement(BAR);
		d->defineComponent(UP1);
		d->defineComponent(UP2);
		d->defineComponent(DOWN1);
		d->defineComponent(DOWN2);
		d->defineComponent(SLIDER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("VScrollBar",d);
	}

	VScrollBarDesc::VScrollBarDesc() :
		ComponentWidgetDesc()
	{
		resetToDefault();
	}

	void VScrollBarDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(25,125);

		vscrollbar_buttonScrollPercent = 0.1;
		vscrollbar_barScrollPercent = 0.2;
		vscrollbar_scrollBarButtonLayout = VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		vscrollbar_sliderHeight = 15;
		vscrollbar_sliderPercentage = 0;

		for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
			vscrollbar_userHandlers[index] = "";
	}

	void VScrollBarDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("ButtonScrollPercent",&vscrollbar_buttonScrollPercent);
		b->IO("BarScrollPercent",&vscrollbar_barScrollPercent);
		b->IO("ScrollBarButtonLayout",&vscrollbar_scrollBarButtonLayout);
		b->IO("SliderHeight",&vscrollbar_sliderHeight);
		b->IO("SliderPercentage",&vscrollbar_sliderPercentage);

		if(b->begin("UserDefinedHandlers","ScrollBarEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<ScrollBarEvent>(index)),&(vscrollbar_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
				{
					if(vscrollbar_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<ScrollBarEvent>(index)),&(vscrollbar_userHandlers[index]));
				}
			}
			b->end();
		}
	}

	VScrollBar::VScrollBar(const Ogre::String& name) :
		ComponentWidget(name),
		mButton_Up1(NULL),
		mButton_Up2(NULL),
		mButton_Down1(NULL),
		mButton_Down2(NULL),
		mButton_Slider(NULL),
		mDesc(NULL)
	{
	}

	VScrollBar::~VScrollBar()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mScrollBarEventHandlers[index].begin(); it != mScrollBarEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void VScrollBar::_applyButtonLayout()
	{
		if(mSkinType == NULL)
		{
			mSliderBounds.x = 0;
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.height;

			mButton_Up1->setVisible(false);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(false);
			return;
		}

		switch(mDesc->vscrollbar_scrollBarButtonLayout)
		{
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN:
			mSliderBounds.x = mSkinElement->getBorderThickness(BORDER_TOP);
			mSliderBounds.y = mButton_Up2->getPosition().y;

			mButton_Up1->setVisible(false);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(true);
			mButton_Down2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM);

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(true);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(false);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mButton_Up2->getPosition().y;

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(true);
			mButton_Up2->setVisible(true);
			mButton_Down2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mButton_Down2->getPosition().y;

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(true);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP:
			mSliderBounds.x = mButton_Down1->getPosition().y;
			mSliderBounds.y = mButton_Up2->getPosition().y;

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(true);
			mButton_Down2->setVisible(true);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_NONE:
			mSliderBounds.x = mSkinElement->getBorderThickness(BORDER_TOP);
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM);

			mButton_Up1->setVisible(false);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(false);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:
			mSliderBounds.x = mButton_Down1->getPosition().y;
			mSliderBounds.y = mButton_Down2->getPosition().y;

			mButton_Up1->setVisible(true);
			mButton_Down1->setVisible(false);
			mButton_Up2->setVisible(false);
			mButton_Down2->setVisible(true);
			break;
		}
	}

	void VScrollBar::_checkSliderPosition()
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
		// Modify position so that x position is within client area is correct
		sliderPosition.x = 0;

		float sliderHeight = mButton_Slider->getHeight();
		if(sliderPosition.y + sliderHeight > mSliderBounds.y)
			mButton_Slider->setPosition(Point(sliderPosition.x,mSliderBounds.y - sliderHeight));
		else if(sliderPosition.y < mSliderBounds.x)
			mButton_Slider->setPosition(Point(sliderPosition.x,mSliderBounds.x));
		else
			mButton_Slider->setPosition(sliderPosition);

		// Update the percentage: Position of slider within Bounds as a percentage
		sliderPosition = mButton_Slider->getPosition();
		// calculate the total distance the slider can move, ie how far will the slider go at 100%? (Answer: Slider bounds minus Slider height)
		float maxSliderDisplacement = ((mSliderBounds.y - mSliderBounds.x) - mButton_Slider->getHeight());
		// Percentage = displacement / max displacement.  Make sure to gaurd against a 0 max displacement!
		if(maxSliderDisplacement < 0.001)
			mDesc->vscrollbar_sliderPercentage = 0.0;
		else
		{
			mDesc->vscrollbar_sliderPercentage = (sliderPosition.y - mSliderBounds.x) / maxSliderDisplacement;
			// Caps percentage to thousandths place
			mDesc->vscrollbar_sliderPercentage = Ogre::Math::Floor(Ogre::Math::Ceil(mDesc->vscrollbar_sliderPercentage * 1000.0)) / 1000.0;
		}
	}

	void VScrollBar::_determineSliderBounds()
	{
		if(mDesc == NULL)
			return;

		switch(mDesc->vscrollbar_scrollBarButtonLayout)
		{
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN:
			mSliderBounds.x = mSkinElement->getBorderThickness(BORDER_TOP);
			mSliderBounds.y = mButton_Up2->getPosition().y;
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mButton_Up2->getPosition().y;
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN:
			mSliderBounds.x = (mButton_Down1->getPosition().y + mButton_Down1->getHeight());
			mSliderBounds.y = mButton_Down2->getPosition().y;
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP:
			mSliderBounds.x = mButton_Down1->getPosition().y;
			mSliderBounds.y = mButton_Up2->getPosition().y;
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_NONE:
			mSliderBounds.x = mSkinElement->getBorderThickness(BORDER_TOP);
			mSliderBounds.y = mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM);
			break;
		case VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE:
			mSliderBounds.x = mButton_Down1->getPosition().y;
			mSliderBounds.y = mButton_Down2->getPosition().y;
			break;
		}
	}

	void VScrollBar::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<VScrollBarDesc*>(mWidgetDesc);

		VScrollBarDesc* vsd = dynamic_cast<VScrollBarDesc*>(d);

		for(int index = 0; index < SCROLLBAR_EVENT_COUNT; ++index)
			mDesc->vscrollbar_userHandlers[index] = vsd->vscrollbar_userHandlers[index];

		mDesc->vscrollbar_barScrollPercent = vsd->vscrollbar_barScrollPercent;
		mDesc->vscrollbar_buttonScrollPercent = vsd->vscrollbar_buttonScrollPercent;

		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();
		bd->widget_resizeFromBottom = false;
		bd->widget_resizeFromLeft = false;
		bd->widget_resizeFromRight = false;
		bd->widget_resizeFromTop = false;
		bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		bd->widget_dimensions.size.width = mWidgetDesc->widget_dimensions.size.width;
		bd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.width;

		mButton_Up1 = dynamic_cast<Button*>(_createWidget(bd));
		mButton_Up1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onUpClicked,this);
		//mButton_Up1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK,&VScrollBar::onUpClicked,this);
		//mButton_Up1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_DOUBLE,&VScrollBar::onUpClicked,this);
		addComponent(UP1,mButton_Up1);
		mButton_Up1->setPositionRelativeToParentClientDimensions(true);

		bd->widget_dimensions.position.y = 15;
		mButton_Down1 = dynamic_cast<Button*>(_createWidget(bd));
		mButton_Down1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onDownClicked,this);
		//mButton_Down1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK,&VScrollBar::onDownClicked,this);
		//mButton_Down1->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_DOUBLE,&VScrollBar::onDownClicked,this);
		addComponent(DOWN1,mButton_Down1);
		mButton_Down1->setPositionRelativeToParentClientDimensions(true);

		bd->widget_verticalAnchor = ANCHOR_VERTICAL_BOTTOM;
		bd->widget_dimensions.position.y = mWidgetDesc->widget_dimensions.size.height - (2 * bd->widget_dimensions.size.height);
		mButton_Up2 = dynamic_cast<Button*>(_createWidget(bd));
		mButton_Up2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onUpClicked,this);
		//mButton_Up2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK,&VScrollBar::onUpClicked,this);
		//mButton_Up2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_DOUBLE,&VScrollBar::onUpClicked,this);
		addComponent(UP2,mButton_Up2);
		mButton_Up2->setPositionRelativeToParentClientDimensions(true);

		bd->widget_dimensions.position.y = mWidgetDesc->widget_dimensions.size.height - bd->widget_dimensions.size.height;
		mButton_Down2 = dynamic_cast<Button*>(_createWidget(bd));
		mButton_Down2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onDownClicked,this);
		//mButton_Down2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK,&VScrollBar::onDownClicked,this);
		//mButton_Down2->addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_DOUBLE,&VScrollBar::onDownClicked,this);
		addComponent(DOWN2,mButton_Down2);
		mButton_Down2->setPositionRelativeToParentClientDimensions(true);

		bd->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		bd->widget_dimensions.position.y = 30;
		bd->widget_dragable = true;
		mButton_Slider = dynamic_cast<Button*>(_createWidget(bd));
		addComponent(SLIDER,mButton_Slider);
		mButton_Slider->setPositionRelativeToParentClientDimensions(true);

		mButton_Slider->addWidgetEventHandler(WIDGET_EVENT_DRAGGED,&VScrollBar::onSliderDragged,this);

		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&VScrollBar::onBarClicked,this);

		setSkinType(vsd->widget_skinTypeName);
		setButtonLayout(vsd->vscrollbar_scrollBarButtonLayout);
		setPercentage(vsd->vscrollbar_sliderPercentage);
	}

	void VScrollBar::addUserDefinedScrollBarEventHandler(ScrollBarEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->vscrollbar_userHandlers[EVENT] = handlerName;
	}

	void VScrollBar::addScrollBarEventHandler(ScrollBarEvent EVENT, EventHandlerSlot* function)
	{
		mScrollBarEventHandlers[EVENT].push_back(function);
	}

	void VScrollBar::clearUserDefinedScrollBarEventHandler(ScrollBarEvent EVENT)
	{
		mDesc->vscrollbar_userHandlers[EVENT] = "";
	}

	bool VScrollBar::fireScrollBarEvent(ScrollBarEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mScrollBarEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->vscrollbar_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->vscrollbar_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	float VScrollBar::getBarScrollPercent()
	{
		return mDesc->vscrollbar_barScrollPercent;
	}

	VScrollBarButtonLayout VScrollBar::getButtonLayout()
	{
		return mDesc->vscrollbar_scrollBarButtonLayout;
	}

	float VScrollBar::getButtonScrollPercent()
	{
		return mDesc->vscrollbar_buttonScrollPercent;
	}

	Ogre::String VScrollBar::getClass()
	{
		return "VScrollBar";
	}

	float VScrollBar::getPercentage()
	{
		return mDesc->vscrollbar_sliderPercentage;
	}

	Rect VScrollBar::getSliderBounds()
	{
		return Rect(mClientDimensions.position.x,mSliderBounds.x,mClientDimensions.size.width,mSliderBounds.y - mSliderBounds.x);
	}

	void VScrollBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void VScrollBar::onUpClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			setPercentage(mDesc->vscrollbar_sliderPercentage - mDesc->vscrollbar_buttonScrollPercent);
		else
			setPercentage(mDesc->vscrollbar_sliderPercentage + mDesc->vscrollbar_buttonScrollPercent);
	}

	void VScrollBar::onDownClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			setPercentage(mDesc->vscrollbar_sliderPercentage + mDesc->vscrollbar_buttonScrollPercent);
		else
			setPercentage(mDesc->vscrollbar_sliderPercentage - mDesc->vscrollbar_buttonScrollPercent);
	}

	void VScrollBar::onBarClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mWindow == NULL)
			return;

		Point p = mea.position - mWindow->getPosition() - mTexturePosition;
		bool above = (p.y < mButton_Slider->getPosition().y);

		setPercentage(p.y/this->getHeight());
		
		/*if(mea.button == MB_Left)
		{
			if(above)
				setPercentage(mDesc->vscrollbar_sliderPercentage - mDesc->vscrollbar_barScrollPercent);
			else
				setPercentage(mDesc->vscrollbar_sliderPercentage + mDesc->vscrollbar_barScrollPercent);
		}
		else if(mea.button == MB_Right)
		{
			if(above)
				setPercentage(mDesc->vscrollbar_sliderPercentage + mDesc->vscrollbar_barScrollPercent);
			else
				setPercentage(mDesc->vscrollbar_sliderPercentage - mDesc->vscrollbar_barScrollPercent);
		}*/
	}

	void VScrollBar::onSliderDragged(const EventArgs& args)
	{
		_checkSliderPosition();

		WidgetEventArgs wea(this);
		fireScrollBarEvent(SCROLLBAR_EVENT_ON_SCROLLED,wea);
	}

	void VScrollBar::scrollDown()
	{
		setPercentage(mDesc->vscrollbar_sliderPercentage + mDesc->vscrollbar_buttonScrollPercent);
	}

	void VScrollBar::scrollToEnd()
	{
		setPercentage((mSliderBounds.y - mButton_Slider->getHeight() - mSliderBounds.x) / (mSliderBounds.y - mSliderBounds.x));
	}

	void VScrollBar::scrollUp()
	{
		setPercentage(mDesc->vscrollbar_sliderPercentage - mDesc->vscrollbar_buttonScrollPercent);
	}

	void VScrollBar::setBarScrollPercent(float percent)
	{
		mDesc->vscrollbar_barScrollPercent = percent;
	}

	void VScrollBar::setButtonLayout(VScrollBarButtonLayout l)
	{
		mDesc->vscrollbar_scrollBarButtonLayout = l;

		// Toggle visibility for buttons and set the slider bounds according to layout and button positions
		_applyButtonLayout();
		// Try to maintain the same percentage, given the new layout.
		setPercentage(mDesc->vscrollbar_sliderPercentage);
	}

	void VScrollBar::setButtonScrollPercent(float percent)
	{
		mDesc->vscrollbar_buttonScrollPercent = percent;
	}

	void VScrollBar::setPercentage(float percentage)
	{
		// Cap percentage within bounds: [0.0,1.0]
		if(percentage <= 0.0)
			percentage = 0.0;
		else if(percentage > 1.0)
			percentage = 1.0;
		else // Caps percentage to thousandths place
			percentage = Ogre::Math::Floor(Ogre::Math::Ceil(percentage * 1000.0)) / 1000.0;

		// Set percentage property
		mDesc->vscrollbar_sliderPercentage = percentage;

		// calculate the total distance the slider can move, ie how far will the slider go at 100%? (Answer: Slider bounds minus Slider height)
		float maxSliderDisplacement = ((mSliderBounds.y - mSliderBounds.x) - mButton_Slider->getHeight());

		// Position slider according to percentage:
		//   At 0.0, slider is at beginning of slider bounds.
		//   At 1.0, slider is at end of slider bounds.
		mButton_Slider->setPosition(Point(0,mSliderBounds.x + (maxSliderDisplacement * percentage)));

		WidgetEventArgs args(this);
		fireScrollBarEvent(SCROLLBAR_EVENT_ON_SCROLLED,args);
	}

	void VScrollBar::setSkinType(const Ogre::String type)
	{
		if(!SkinTypeManager::getSingleton().hasSkinType(getClass(),type))
			throw Exception(Exception::ERR_SKINNING,"SkinType \"" + type + "\" does not exist!","Widget::_setSkinType");

		mSkinType = SkinTypeManager::getSingleton().getSkinType(getClass(),type);
		updateSkinElement();

		mWidgetDesc->widget_skinTypeName = type;

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getSkinReference((*it).first)->typeName);

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
		float buttonThickness = mClientDimensions.size.width;
		Size s(buttonThickness,buttonThickness);

		mButton_Up1->setSize(s);
		mButton_Down1->setSize(s);
		mButton_Up2->setSize(s);
		mButton_Down2->setSize(s);
		mButton_Slider->setWidth(mClientDimensions.size.width);

		// Update button positions
		Point p;
		mButton_Up1->setPosition(p);

		p.y += buttonThickness;
		mButton_Down1->setPosition(p);

		p.y = mClientDimensions.size.height - buttonThickness - buttonThickness;
		mButton_Up2->setPosition(p);

		p.y += buttonThickness;
		mButton_Down2->setPosition(p);

		p.y = mButton_Slider->getPosition().y;
		mButton_Slider->setPosition(p);

		_determineSliderBounds();
		// Now that bounds are properly updated, restore percentage of the scrollbar
		setPercentage(mDesc->vscrollbar_sliderPercentage);

		WidgetEventArgs wea(this);
		fireWidgetEvent(WIDGET_EVENT_SKIN_CHANGED,wea);
	}

	void VScrollBar::setSliderHeight(float height)
	{
		mButton_Slider->setHeight(height);

		_checkSliderPosition();
	}

	void VScrollBar::updateClientDimensions()
	{
		ComponentWidget::updateClientDimensions();

		// Update Slider Bounds, reliant on button layout and position.  In this situation
		// Button positions may have changed.
		_determineSliderBounds();
		// Make sure Slider is within slider bounds
		_checkSliderPosition();
	}

	void VScrollBar::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(BAR);
	}
}
