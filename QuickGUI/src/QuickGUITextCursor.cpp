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

#include "QuickGUITextCursor.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIWidget.h"
#include "QuickGUITextBox.h"
#include "QuickGUITextArea.h"
#include "QuickGUISkinDefinition.h"

namespace QuickGUI
{
	const Ogre::String TextCursor::BACKGROUND = "background";

	void TextCursor::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TextCursor");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TextCursor",d);
	}

	TextCursor::TextCursor(Widget* owner) :
		mOwner(owner),
		mSkinElement(NULL),
		mSkinType(NULL)
	{
		mVisible = false;

		setSkinType("default");
	}

	TextCursor::~TextCursor()
	{
	}

	Point TextCursor::getPosition()
	{
		return mDimensions.position;
	}

	Size TextCursor::getSize()
	{
		return mDimensions.size;
	}

	bool TextCursor::getVisible()
	{
		return mVisible;
	}

	void TextCursor::onDraw()
	{
		if(!mVisible)
			return;

		QuickGUI::Brush* brush = QuickGUI::Brush::getSingletonPtr();

		// Store Render Target
		Ogre::Viewport* currentRenderTarget = brush->getRenderTarget();

		// Set Render Target to Viewport
		brush->setRenderTarget(static_cast<Ogre::Viewport*>(NULL));

		// Save current clipping region
		Rect clipRegion = brush->getClipRegion();

		// Set clip region
		if(mOwner->getClass() == "TextBox")
			brush->setClipRegion(dynamic_cast<TextBox*>(mOwner)->getScreenRect());
		else if(mOwner->getClass() == "TextArea")
			brush->setClipRegion(dynamic_cast<TextArea*>(mOwner)->getScreenRect());
		else
			brush->setClipRegion(Rect(mOwner->getScreenPosition(),mOwner->getSize()));

		// Draw SkinElement
		brush->drawSkinElement(mDimensions,mSkinElement);

		// Restore Render Target
		brush->setRenderTarget(currentRenderTarget);

		// Restore Clip Region
		brush->setClipRegion(clipRegion);
	}

	void TextCursor::setHeight(float heightInPixels)
	{
		float percentage = heightInPixels / mDimensions.size.height;
		mDimensions.size *= percentage;
	}

	void TextCursor::setPosition(Point& p)
	{
		mDimensions.position.x = p.x - (mDimensions.size.width / 2.0);
		mDimensions.position.y = p.y;
	}

	void TextCursor::setSkinType(const Ogre::String type)
	{
		mSkinType = SkinTypeManager::getSingleton().getSkinType("TextCursor",type);
		mSkinElement = mSkinType->getSkinElement(BACKGROUND);

		mDimensions.size.width = mSkinElement->getWidth();
		mDimensions.size.height = mSkinElement->getHeight();
	}

	void TextCursor::setVisible(bool visible)
	{
		mVisible = visible;
	}
}
