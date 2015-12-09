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

#include "QuickGUICheckBox.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	const Ogre::String CheckBox::CHECKED = "checked";
	const Ogre::String CheckBox::CHECKED_DOWN = "checked_down";
	const Ogre::String CheckBox::CHECKED_OVER = "checked_over";
	const Ogre::String CheckBox::UNCHECKED = "unchecked";
	const Ogre::String CheckBox::UNCHECKED_DOWN = "unchecked_down";
	const Ogre::String CheckBox::UNCHECKED_OVER = "unchecked_over";

	void CheckBox::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("CheckBox");
		d->defineSkinElement(CHECKED);
		d->defineSkinElement(CHECKED_DOWN);
		d->defineSkinElement(CHECKED_OVER);
		d->defineSkinElement(UNCHECKED);
		d->defineSkinElement(UNCHECKED_DOWN);
		d->defineSkinElement(UNCHECKED_OVER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("CheckBox",d);
	}

	CheckBoxDesc::CheckBoxDesc() :
		WidgetDesc()
	{
		resetToDefault();
	}

	void CheckBoxDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		widget_dimensions.size = Size(25,25);

		checkbox_checked = false;

		for(int index = 0; index < CHECKBOX_EVENT_COUNT; ++index)
			checkbox_userHandlers[index] = "";
	}

	void CheckBoxDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		CheckBoxDesc* defaultValues = DescManager::getSingleton().createDesc<CheckBoxDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("Checked", &checkbox_checked, defaultValues->checkbox_checked);

		DescManager::getSingleton().destroyDesc(defaultValues);

		if(b->begin("UserDefinedHandlers","CheckBoxEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < CHECKBOX_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<CheckBoxEvent>(index)),&(checkbox_userHandlers[index]),"");
			}
			else
			{
				for(int index = 0; index < CHECKBOX_EVENT_COUNT; ++index)
				{
					if(checkbox_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<CheckBoxEvent>(index)),&(checkbox_userHandlers[index]),"");
				}
			}
			b->end();
		}
	}

	CheckBox::CheckBox(const Ogre::String& name) :
		Widget(name)
	{
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&CheckBox::onMouseEnter,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&CheckBox::onMouseLeave,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&CheckBox::onMouseLeftButtonDown,this);
		Widget::addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&CheckBox::onMouseLeftButtonUp,this);
	}

	CheckBox::~CheckBox()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < CHECKBOX_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mCheckBoxEventHandlers[index].begin(); it != mCheckBoxEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void CheckBox::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<CheckBoxDesc*>(mWidgetDesc);

		CheckBoxDesc* cbd = dynamic_cast<CheckBoxDesc*>(d);

		for(int index = 0; index < CHECKBOX_EVENT_COUNT; ++index)
			mDesc->checkbox_userHandlers[index] = cbd->checkbox_userHandlers[index];

		setSkinType(cbd->widget_skinTypeName);
		setChecked(cbd->checkbox_checked);
	}

	void CheckBox::addUserDefinedCheckBoxEventHandler(CheckBoxEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->checkbox_userHandlers[EVENT] = handlerName;
	}

	void CheckBox::addCheckBoxEventHandler(CheckBoxEvent EVENT, EventHandlerSlot* function)
	{
		mCheckBoxEventHandlers[EVENT].push_back(function);
	}

	void CheckBox::clearUserDefinedCheckBoxEventHandler(CheckBoxEvent EVENT)
	{
		mDesc->checkbox_userHandlers[EVENT] = "";
	}

	Ogre::String CheckBox::getClass()
	{
		return "CheckBox";
	}

	bool CheckBox::fireCheckBoxEvent(CheckBoxEvent EVENT, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mCheckBoxEventHandlers[EVENT]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->checkbox_userHandlers[EVENT] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->checkbox_userHandlers[EVENT],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	bool CheckBox::getChecked()
	{
		return mDesc->checkbox_checked;
	}

	void CheckBox::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void CheckBox::onMouseEnter(const EventArgs& args)
	{
		if(mDesc->checkbox_checked)
			mSkinElement = mSkinType->getSkinElement(CHECKED_OVER);
		else
			mSkinElement = mSkinType->getSkinElement(UNCHECKED_OVER);

		redraw();
	}

	void CheckBox::onMouseLeave(const EventArgs& args)
	{
		if(mDesc->checkbox_checked)
			mSkinElement = mSkinType->getSkinElement(CHECKED);
		else
			mSkinElement = mSkinType->getSkinElement(UNCHECKED);

		redraw();
	}

	void CheckBox::onMouseLeftButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(mDesc->checkbox_checked)
				mSkinElement = mSkinType->getSkinElement(CHECKED_DOWN);
			else
				mSkinElement = mSkinType->getSkinElement(UNCHECKED_DOWN);

			redraw();
		}
	}

	void CheckBox::onMouseLeftButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			mDesc->checkbox_checked = !mDesc->checkbox_checked;

			if(mDesc->checkbox_checked)
				mSkinElement = mSkinType->getSkinElement(CHECKED_OVER);
			else
				mSkinElement = mSkinType->getSkinElement(UNCHECKED_OVER);

			redraw();

			WidgetEventArgs wea(this);
			fireCheckBoxEvent(CHECKBOX_EVENT_CHECK_CHANGED,wea);
		}
	}

	void CheckBox::removeEventHandlers(void* obj)
	{
		Widget::removeEventHandlers(obj);

		for(int index = 0; index < CHECKBOX_EVENT_COUNT; ++index)
		{
			std::vector<EventHandlerSlot*> updatedList;
			std::vector<EventHandlerSlot*> listToCleanup;

			for(std::vector<EventHandlerSlot*>::iterator it = mCheckBoxEventHandlers[index].begin(); it != mCheckBoxEventHandlers[index].end(); ++it)
			{
				if((*it)->getClass() == obj)
					listToCleanup.push_back((*it));
				else
					updatedList.push_back((*it));
			}

			mCheckBoxEventHandlers[index].clear();
			for(std::vector<EventHandlerSlot*>::iterator it = updatedList.begin(); it != updatedList.end(); ++it)
				mCheckBoxEventHandlers[index].push_back((*it));

			for(std::vector<EventHandlerSlot*>::iterator it = listToCleanup.begin(); it != listToCleanup.end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void CheckBox::setChecked(bool checked)
	{
		if(mDesc->checkbox_checked == checked)
			return;

		mDesc->checkbox_checked = checked;

		if(mDesc->checkbox_checked)
			mSkinElement = mSkinType->getSkinElement(CHECKED);
		else
			mSkinElement = mSkinType->getSkinElement(UNCHECKED);

		redraw();

		WidgetEventArgs wea(this);
		fireCheckBoxEvent(CHECKBOX_EVENT_CHECK_CHANGED,wea);
	}

	void CheckBox::setEnabled(bool enabled)
	{
		if(mWidgetDesc->widget_enabled == enabled)
			return;

		mWidgetDesc->widget_enabled = enabled;

		if(!mWidgetDesc->widget_enabled)
		{
			if(mWidgetDesc->widget_disabledSkinType != "")
				_setSkinType(mWidgetDesc->widget_disabledSkinType);
		}
		else
			_setSkinType(mWidgetDesc->widget_skinTypeName);

		if(mDesc->checkbox_checked)
			mSkinElement = mSkinType->getSkinElement(CHECKED);
		else
			mSkinElement = mSkinType->getSkinElement(UNCHECKED);

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_ENABLED_CHANGED,args);
	}

	void CheckBox::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(UNCHECKED);
	}
}
