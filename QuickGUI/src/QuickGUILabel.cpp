#include "QuickGUILabel.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"

namespace QuickGUI
{
	const Ogre::String Label::BACKGROUND = "background";

	void Label::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Label");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Label",d);
	}

	LabelDesc::LabelDesc() :
		WidgetDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void LabelDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		TextUserDesc::resetToDefault();

		widget_dimensions.size = Size::ZERO;
		widget_minSize = Size::ZERO;

		label_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;
	}

	void LabelDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("VerticalTextAlignment",&label_verticalTextAlignment);

		TextUserDesc::serialize(b);
	}

	Label::Label(const Ogre::String& name) :
		Widget(name),
		TextUser()
	{
	}

	Label::~Label()
	{
	}

	void Label::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<LabelDesc*>(mWidgetDesc);

		LabelDesc* ld = dynamic_cast<LabelDesc*>(d);

		setSkinType(d->widget_skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is umSkinElementd for mSkinElementrialization.
		mDesc->textDesc = ld->textDesc;

		if(mDesc->widget_dimensions.size.width == 1)
		{
			if(mDesc->textDesc.segments.empty())
				mDesc->widget_dimensions.size.width = 50;
			else
				mDesc->widget_dimensions.size.width = mSkinElement->getBorderThickness(BORDER_LEFT) + mDesc->textDesc.getTextWidth() + mSkinElement->getBorderThickness(BORDER_RIGHT);
		}
		if(mDesc->widget_dimensions.size.height == 1)
		{
			if(mDesc->textDesc.segments.empty())
				mDesc->widget_dimensions.size.height = 20;
			else
				mDesc->widget_dimensions.size.height = mSkinElement->getBorderThickness(BORDER_TOP) + mDesc->textDesc.getTextHeight() + mSkinElement->getBorderThickness(BORDER_BOTTOM);
		}
		// Now that dimensions may have changed, update client dimensions
		updateClientDimensions();

		mDesc->label_verticalTextAlignment = ld->label_verticalTextAlignment;
		mDesc->textDesc.allottedWidth = mDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
		TextUser::_initialize(this,mDesc);
	}

	Ogre::String Label::getClass()
	{
		return "Label";
	}

	VerticalTextAlignment Label::getVerticalTextAlignment()
	{
		return mDesc->label_verticalTextAlignment;
	}

	void Label::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = 0;

		switch(mDesc->label_verticalTextAlignment)
		{
		case TEXT_ALIGNMENT_VERTICAL_BOTTOM:
			yPos = mDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM) - textHeight;
			break;
		case TEXT_ALIGNMENT_VERTICAL_CENTER:
			yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);
			break;
		case TEXT_ALIGNMENT_VERTICAL_TOP:
			yPos = mSkinElement->getBorderThickness(BORDER_TOP);
			break;
		}

		// Clip to client dimensions
		Rect clipRegion(mClientDimensions);
		clipRegion.translate(mTexturePosition);

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		clipRegion = mClientDimensions;
		clipRegion.position.y = yPos;
		clipRegion.translate(mTexturePosition);		

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void Label::setVerticalTextAlignment(VerticalTextAlignment a)
	{
		mDesc->label_verticalTextAlignment = a;

		redraw();
	}

	void Label::updateClientDimensions()
	{
		Widget::updateClientDimensions();
		if(mText != NULL)
			mText->setAllottedWidth(mClientDimensions.size.width);

		redraw();
	}
}
