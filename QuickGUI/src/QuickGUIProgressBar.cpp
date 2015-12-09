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

#include "QuickGUIProgressBar.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIDescManager.h"

#include "OgreTexture.h"

namespace QuickGUI
{
	const Ogre::String ProgressBar::BACKGROUND = "background";
	const Ogre::String ProgressBar::BAR = "bar";
	const Ogre::String ProgressBar::CLIPMAP = "clipmap";

	void ProgressBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ProgressBar");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(BAR);
		d->defineSkinElement(CLIPMAP);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ProgressBar",d);
	}

	ProgressBarDesc::ProgressBarDesc() :
		WidgetDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void ProgressBarDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		widget_dimensions.size = Size(100,25);

		progressbar_fillDirection = PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE;
		progressbar_layout = PROGRESSBAR_LAYOUT_HORIZONTAL;
		progressbar_clippingEdge = PROGRESSBAR_CLIP_LEFT_BOTTOM;
		progressbar_progress = 100;

		for(int index = 0; index < PROGRESSBAR_EVENT_COUNT; ++index)
			progressbar_userHandlers[index] = "";

		widget_transparencyPicking = false;

		TextUserDesc::resetToDefault();
	}

	void ProgressBarDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		TextUserDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		ProgressBarDesc* defaultValues = DescManager::getSingleton().createDesc<ProgressBarDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("Progress",		&progressbar_progress,		defaultValues->progressbar_progress);
		b->IO("FillDirection",	&progressbar_fillDirection, defaultValues->progressbar_fillDirection);
		b->IO("Layout",			&progressbar_layout,		defaultValues->progressbar_layout);
		b->IO("ClippingEdge",	&progressbar_clippingEdge,	defaultValues->progressbar_clippingEdge);

		DescManager::getSingleton().destroyDesc(defaultValues);

		if(b->begin("UserDefinedHandlers","ProgressBarEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < PROGRESSBAR_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<ProgressBarEvent>(index)),&(progressbar_userHandlers[index]),"");
			}
			else
			{
				for(int index = 0; index < PROGRESSBAR_EVENT_COUNT; ++index)
				{
					if(progressbar_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<ProgressBarEvent>(index)),&(progressbar_userHandlers[index]),"");
				}
			}
			b->end();
		}
	}

	ProgressBar::ProgressBar(const Ogre::String& name) :
		Widget(name),
		TextUser(),
		mOutputBarTexture(NULL)
	{
	}

	ProgressBar::~ProgressBar()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < PROGRESSBAR_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mProgressBarEventHandlers[index].begin(); it != mProgressBarEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}

		if(mOutputBarTexture != NULL)
		{
			Ogre::String textureName = mOutputBarTexture->getName();
			mOutputBarTexture = NULL;
			Ogre::TextureManager::getSingletonPtr()->remove(textureName);
		}
	}

	void ProgressBar::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<ProgressBarDesc*>(mWidgetDesc);

		ProgressBarDesc* pd = dynamic_cast<ProgressBarDesc*>(d);

		for(int index = 0; index < PROGRESSBAR_EVENT_COUNT; ++index)
			mDesc->progressbar_userHandlers[index] = pd->progressbar_userHandlers[index];

		mDesc->progressbar_clippingEdge = pd->progressbar_clippingEdge;
		mDesc->progressbar_fillDirection = pd->progressbar_fillDirection;
		mDesc->progressbar_layout = pd->progressbar_layout;
		mDesc->progressbar_progress = pd->progressbar_progress;

		setSkinType(d->widget_skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = pd->textDesc;
		mDesc->textDesc.allottedSize.height = mDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));
		mDesc->textDesc.allottedSize.width = mDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
		TextUser::_initialize(this,mDesc);
	}

	void ProgressBar::_processClipMap()
	{
		if(mSkinType != NULL)
		{
			SkinElement* se = mSkinType->getSkinElement(CLIPMAP);
			Ogre::Image i = se->getTextureImage();

			int width = static_cast<int>(i.getWidth());
			int height = static_cast<int>(i.getHeight());

			mClipMap.assign(width * height,true);

			for(int row = 0; row < height; ++row)
			{
				for(int col = 0; col < width; ++col)
				{
					if(i.getColourAt(col,row,0).a < 0.01)
						mClipMap[(row * width) + col] = false;
				}
			}
		}
	}

	void ProgressBar::addUserDefinedProgressBarEventHandler(ProgressBarEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->progressbar_userHandlers[EVENT] = handlerName;
	}

	void ProgressBar::addProgressBarEventHandler(ProgressBarEvent EVENT, EventHandlerSlot* function)
	{
		mProgressBarEventHandlers[EVENT].push_back(function);
	}

	void ProgressBar::clearUserDefinedProgressBarEventHandler(ProgressBarEvent EVENT)
	{
		mDesc->progressbar_userHandlers[EVENT] = "";
	}

	bool ProgressBar::fireProgressBarEvent(ProgressBarEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mProgressBarEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->progressbar_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->progressbar_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	Ogre::String ProgressBar::getClass()
	{
		return "ProgressBar";
	}

	ProgressBarClippingEdge ProgressBar::getClippingEdge()
	{
		return mDesc->progressbar_clippingEdge;
	}

	ProgressBarFillDirection ProgressBar::getFillDirection()
	{
		return mDesc->progressbar_fillDirection;
	}

	void ProgressBar::removeEventHandlers(void* obj)
	{
		Widget::removeEventHandlers(obj);

		for(int index = 0; index < PROGRESSBAR_EVENT_COUNT; ++index)
		{
			std::vector<EventHandlerSlot*> updatedList;
			std::vector<EventHandlerSlot*> listToCleanup;

			for(std::vector<EventHandlerSlot*>::iterator it = mProgressBarEventHandlers[index].begin(); it != mProgressBarEventHandlers[index].end(); ++it)
			{
				if((*it)->getClass() == obj)
					listToCleanup.push_back((*it));
				else
					updatedList.push_back((*it));
			}

			mProgressBarEventHandlers[index].clear();
			for(std::vector<EventHandlerSlot*>::iterator it = updatedList.begin(); it != updatedList.end(); ++it)
				mProgressBarEventHandlers[index].push_back((*it));

			for(std::vector<EventHandlerSlot*>::iterator it = listToCleanup.begin(); it != listToCleanup.end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	ProgressBarLayout ProgressBar::setLayout()
	{
		return mDesc->progressbar_layout;
	}

	float ProgressBar::getProgress()
	{
		return mDesc->progressbar_progress;
	}

	void ProgressBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		brush->setTexture(mOutputBarTexture);
		Rect barRect = mClientDimensions;
		barRect.translate(mTexturePosition);
		brush->drawRectangle(barRect,UVRect(0,0,1,1));

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		if(mText->empty())
			return;

		// Clip to client dimensions
		Rect clipRegion(mClientDimensions);
		clipRegion.translate(mTexturePosition);

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);		

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void ProgressBar::setClippingEdge(ProgressBarClippingEdge e)
	{
		mDesc->progressbar_clippingEdge = e;

		setProgress(mDesc->progressbar_progress);
	}

	void ProgressBar::setFillDirection(ProgressBarFillDirection d)
	{
		mDesc->progressbar_fillDirection = d;

		setProgress(mDesc->progressbar_progress);
	}

	void ProgressBar::setLayout(ProgressBarLayout l)
	{
		mDesc->progressbar_layout = l;

		setProgress(mDesc->progressbar_progress);
	}

	void ProgressBar::setProgress(float percent)
	{
		if(percent > 100)
			percent = 100;
		else if(percent < 0)
			percent = 0;

		mDesc->progressbar_progress = percent;

		// Update Bar Output Texture to match percentage
		{
			// Determine the Rect to blit from the bar texture. All values are percentages
			Rect sourceArea;
			Rect destArea;

			if(mDesc->progressbar_layout == PROGRESSBAR_LAYOUT_HORIZONTAL)
			{
				// Left to right
				if(mDesc->progressbar_fillDirection == PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE)
				{
					if(mDesc->progressbar_clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = (100 - mDesc->progressbar_progress) / 100.0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progressbar_progress / 100.0;
						sourceArea.size.height = 1.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progressbar_progress / 100.0;
						sourceArea.size.height = 1.0;
					}

					destArea.position.x = 0;
					destArea.position.y = 0;
					destArea.size.width = mDesc->progressbar_progress / 100.0;
					destArea.size.height = 1.0;
				}
				// Right to left
				else
				{
					if(mDesc->progressbar_clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = (100 - mDesc->progressbar_progress) / 100.0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progressbar_progress / 100.0;
						sourceArea.size.height = 1.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = mDesc->progressbar_progress / 100.0;
						sourceArea.size.height = 1.0;
					}

					destArea.position.x = (100 - mDesc->progressbar_progress) / 100.0;
					destArea.position.y = 0;
					destArea.size.width = mDesc->progressbar_progress / 100.0;
					destArea.size.height = 1.0;
				}
			}
			else
			{
				// Bottom to top
				if(mDesc->progressbar_fillDirection == PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE)
				{
					if(mDesc->progressbar_clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = (100 - mDesc->progressbar_progress) / 100.0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progressbar_progress / 100.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progressbar_progress / 100.0;
					}

					destArea.position.x = 0;
					destArea.position.y = (100 - mDesc->progressbar_progress) / 100.0;
					destArea.size.width = 1.0;
					destArea.size.height = mDesc->progressbar_progress / 100.0;
				}
				// Top to Bottom
				else
				{
					if(mDesc->progressbar_clippingEdge == PROGRESSBAR_CLIP_LEFT_BOTTOM)
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = (100 - mDesc->progressbar_progress) / 100.0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progressbar_progress / 100.0;
					}
					else
					{
						sourceArea.position.x = 0;
						sourceArea.position.y = 0;
						sourceArea.size.width = 1.0;
						sourceArea.size.height = mDesc->progressbar_progress / 100.0;
					}
					
					destArea.position.x = 0;
					destArea.position.y = 0;
					destArea.size.width = 1.0;
					destArea.size.height = mDesc->progressbar_progress / 100.0;
				}
			}

			// Clear our output bar texture and blit to it - src rect from Bar texture, dst rect is location in output texture
			
			int barWidth = static_cast<int>(mBarImage.getWidth());
			int barHeight = static_cast<int>(mBarImage.getHeight());

			// Hardware Pixel Buffer for moving pixels around, and correctly creating the barTexture.
			Ogre::HardwarePixelBufferSharedPtr buf = mOutputBarTexture->getBuffer();
			buf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
			Ogre::uint8* bufPtr = static_cast<Ogre::uint8*>((buf->getCurrentLock()).data);
			for( int i = 0; i < barHeight; ++i )
			{
				for( int j = 0; j < barWidth; ++j )
				{
					*bufPtr++ = 0;	// R
					*bufPtr++ = 0;	// B
					*bufPtr++ = 0;	// G
					*bufPtr++ = 0;	// A
				}
			}
			buf->unlock();

			if(mDesc->progressbar_progress <= 0.0)
				return;

			// Blit the area of the Bar texture to the output texture. (Grab pixels from Src in Bar texture, and add to Dest in Output Texture)

			Ogre::PixelBox srcBox = mBarImage.getPixelBox().getSubVolume(Ogre::Box(static_cast<size_t>(sourceArea.position.x * barWidth),static_cast<size_t>(sourceArea.position.y * barHeight),static_cast<size_t>((sourceArea.position.x * barWidth) + (sourceArea.size.width * barWidth)),static_cast<size_t>((sourceArea.position.y * barHeight) + (sourceArea.size.height * barHeight))));
			Ogre::Image::Box dstBox = Ogre::Image::Box(static_cast<size_t>(destArea.position.x * barWidth),static_cast<size_t>(destArea.position.y * barHeight),static_cast<size_t>((destArea.position.x * barWidth) + (destArea.size.width * barWidth)),static_cast<size_t>((destArea.position.y * barHeight) + (destArea.size.height * barHeight)));

			buf->blitFromMemory(srcBox,dstBox);

			// Use clipmap to clip pixels

			buf->lock(Ogre::HardwareBuffer::HBL_NORMAL);
			bufPtr = static_cast<Ogre::uint8*>((buf->getCurrentLock()).data);
			for( int i = 0; i < barHeight; ++i )
			{
				for( int j = 0; j < barWidth; ++j )
				{
					bufPtr += 3; // Not concerned with RBG at this point

					if(!mClipMap[(i * barWidth) + j])
						*bufPtr = 0;	// Set transparent

					bufPtr++;
				}
			}
			buf->unlock();
		}

		redraw();
	}

	void ProgressBar::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);

		mBarImage = mSkinType->getSkinElement(BAR)->getTextureImage();
		_processClipMap();

		// Create Output Bar texture - If it already exists, destroy previous one
		if(mOutputBarTexture != NULL)
		{
			Ogre::String textureName = mOutputBarTexture->getName();
			mOutputBarTexture = NULL;
			Ogre::TextureManager::getSingletonPtr()->remove(textureName);
		}

		Ogre::String texName = getName() + ".bar.output";
		while(Ogre::TextureManager::getSingleton().resourceExists(texName))
			texName += "_";

		mOutputBarTexture = Ogre::TextureManager::getSingletonPtr()->createManual(texName, 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
			Ogre::TEX_TYPE_2D,
			static_cast<Ogre::uint>(mBarImage.getWidth()),
			static_cast<Ogre::uint>(mBarImage.getHeight()),
			0, 
			Ogre::PF_B8G8R8A8,
			Ogre::TU_STATIC).getPointer();

		setProgress(mDesc->progressbar_progress);
	}

	void ProgressBar::updateClientDimensions()
	{
		Widget::updateClientDimensions();
		if(mText != NULL)
			mText->setAllottedSize(mClientDimensions.size);

		redraw();
	}
}
