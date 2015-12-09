#include "QuickGUIRadioButton.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIContainerWidget.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIEventHandlerManager.h"

namespace QuickGUI
{
	const Ogre::String RadioButton::SELECTED = "selected";
	const Ogre::String RadioButton::SELECTED_DOWN = "selected_down";
	const Ogre::String RadioButton::SELECTED_OVER = "selected_over";
	const Ogre::String RadioButton::UNSELECTED = "unselected";
	const Ogre::String RadioButton::UNSELECTED_DOWN = "unselected_down";
	const Ogre::String RadioButton::UNSELECTED_OVER = "unselected_over";

	void RadioButton::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("RadioButton");
		d->defineSkinElement(SELECTED);
		d->defineSkinElement(SELECTED_DOWN);
		d->defineSkinElement(SELECTED_OVER);
		d->defineSkinElement(UNSELECTED);
		d->defineSkinElement(UNSELECTED_DOWN);
		d->defineSkinElement(UNSELECTED_OVER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("RadioButton",d);
	}

	RadioButtonDesc::RadioButtonDesc() :
		WidgetDesc()
	{
		resetToDefault();
	}

	void RadioButtonDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		widget_dimensions.size = Size(25,25);

		radiobutton_selected = false;

		for(int index = 0; index < RADIOBUTTON_EVENT_COUNT; ++index)
			radiobutton_userHandlers[index] = "";
	}

	void RadioButtonDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("Selected",&radiobutton_selected);

		if(b->begin("UserDefinedHandlers","RadioButtonEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < RADIOBUTTON_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<RadioButtonEvent>(index)),&(radiobutton_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < RADIOBUTTON_EVENT_COUNT; ++index)
				{
					if(radiobutton_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<RadioButtonEvent>(index)),&(radiobutton_userHandlers[index]));
				}
			}
			b->end();
		}
	}

	RadioButton::RadioButton(const Ogre::String& name) :
		Widget(name)
	{
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&RadioButton::onMouseEnter,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&RadioButton::onMouseLeave,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&RadioButton::onMouseLeftButtonDown,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&RadioButton::onMouseLeftButtonUp,this);
	}

	RadioButton::~RadioButton()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < RADIOBUTTON_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mRadioButtonEventHandlers[index].begin(); it != mRadioButtonEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void RadioButton::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<RadioButtonDesc*>(mWidgetDesc);

		RadioButtonDesc* cbd = dynamic_cast<RadioButtonDesc*>(d);

		for(int index = 0; index < RADIOBUTTON_EVENT_COUNT; ++index)
			mDesc->radiobutton_userHandlers[index] = cbd->radiobutton_userHandlers[index];

		setSkinType(d->widget_skinTypeName);
		if(cbd->radiobutton_selected)
			select();
	}

	void RadioButton::addUserDefinedRadioButtonEventHandler(RadioButtonEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->radiobutton_userHandlers[EVENT] = handlerName;
	}

	void RadioButton::addRadioButtonEventHandler(RadioButtonEvent EVENT, EventHandlerSlot* function)
	{
		mRadioButtonEventHandlers[EVENT].push_back(function);
	}

	void RadioButton::clearUserDefinedRadioButtonEventHandler(RadioButtonEvent EVENT)
	{
		mDesc->radiobutton_userHandlers[EVENT] = "";
	}

	Ogre::String RadioButton::getClass()
	{
		return "RadioButton";
	}

	bool RadioButton::fireRadioButtonEvent(RadioButtonEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mRadioButtonEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->radiobutton_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->radiobutton_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	bool RadioButton::getSelected()
	{
		return mDesc->radiobutton_selected;
	}

	void RadioButton::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void RadioButton::onMouseEnter(const EventArgs& args)
	{
		if(mDesc->radiobutton_selected)
			mSkinElement = mSkinType->getSkinElement(SELECTED_OVER);
		else
			mSkinElement = mSkinType->getSkinElement(UNSELECTED_OVER);

		redraw();
	}

	void RadioButton::onMouseLeave(const EventArgs& args)
	{
		if(mDesc->radiobutton_selected)
			mSkinElement = mSkinType->getSkinElement(SELECTED);
		else
			mSkinElement = mSkinType->getSkinElement(UNSELECTED);

		redraw();
	}

	void RadioButton::onMouseLeftButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(mDesc->radiobutton_selected)
				mSkinElement = mSkinType->getSkinElement(SELECTED_DOWN);
			else
				mSkinElement = mSkinType->getSkinElement(UNSELECTED_DOWN);

			redraw();
		}
	}

	void RadioButton::onMouseLeftButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			select();

			mSkinElement = mSkinType->getSkinElement(SELECTED_OVER);

			redraw();
		}
	}

	void RadioButton::select()
	{
		if(mDesc->radiobutton_selected)
		{
			mSkinElement = mSkinType->getSkinElement(SELECTED);
			redraw();
			return;
		}

		if((mParentWidget != NULL) && (mParentWidget->isContainerWidget()))
		{
			std::vector<Widget*> children = dynamic_cast<ContainerWidget*>(mParentWidget)->getChildren();
			for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it)
			{
				if((*it)->getClass() == "RadioButton")
				{
					RadioButton* rb = dynamic_cast<RadioButton*>((*it));
					rb->mDesc->radiobutton_selected = false;
					rb->mSkinElement = rb->mSkinType->getSkinElement(UNSELECTED);
				}
			}
		}

		mDesc->radiobutton_selected = true;

		mSkinElement = mSkinType->getSkinElement(SELECTED);
		
		redraw();

		WidgetEventArgs wea(this);
		fireRadioButtonEvent(RADIOBUTTON_EVENT_SELECTED,wea);
	}

	void RadioButton::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(UNSELECTED);
	}
}
