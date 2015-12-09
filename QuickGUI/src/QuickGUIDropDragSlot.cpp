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

#include "QuickGUIDropDragSlot.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIDescManager.h"

#include "OgreTextureManager.h"

namespace QuickGUI
{
	const Ogre::String DropDragSlot::BACKGROUND = "background";

	void DropDragSlot::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("DropDragSlot");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("DropDragSlot",d);
	}

	DropDragSlotDesc::DropDragSlotDesc() :
		WidgetDesc()
	{
		resetToDefault();
	}

	void DropDragSlotDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		widget_dimensions.size = Size::ZERO;
		widget_transparencyPicking = false;
	}

	void DropDragSlotDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		DropDragSlotDesc* defaultValues = DescManager::getSingleton().createDesc<DropDragSlotDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		DescManager::getSingleton().destroyDesc(defaultValues);
	}

	DropDragSlot::DropDragSlot(const Ogre::String& name) :
		Widget(name)
	{
	}

	DropDragSlot::~DropDragSlot()
	{
	}

	void DropDragSlot::_initialize(WidgetDesc* d)
	{
		// Record the size initially passed in.
		Size sizeFromDesc = d->widget_dimensions.size;

		Widget::_initialize(d);

		mDesc = dynamic_cast<DropDragSlotDesc*>(mWidgetDesc);

		DropDragSlotDesc* id = dynamic_cast<DropDragSlotDesc*>(d);

		setSkinType(id->widget_skinTypeName);

		if(sizeFromDesc == Size::ZERO)
		{
			if(mSkinElement != NULL)
			{
				mWidgetDesc->widget_dimensions.size.width = mSkinElement->getWidth();
				mWidgetDesc->widget_dimensions.size.height = mSkinElement->getHeight();
				redraw();
			}
		}
	}

	Ogre::String DropDragSlot::getClass()
	{
		return "DropDragSlot";
	}

	void DropDragSlot::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);
	}
}
