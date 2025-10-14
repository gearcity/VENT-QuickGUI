#include "QuickGUIListTextItem.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"

namespace QuickGUI
{
	void ListTextItem::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ListTextItem");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ListTextItem",d);
	}

	ListTextItemDesc::ListTextItemDesc() :
		ListItemDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void ListTextItemDesc::resetToDefault()
	{
		ListItemDesc::resetToDefault();

		listtextitem_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;
		
		TextUserDesc::resetToDefault();
	}

	void ListTextItemDesc::serialize(SerialBase* b)
	{
		ListItemDesc::serialize(b);

		b->IO("VerticalTextAlignment",&listtextitem_verticalTextAlignment);

		TextUserDesc::serialize(b);
	}

	ListTextItem::ListTextItem(const Ogre::String& name) :
		ListItem(name),
		TextUser()
	{
	}

	ListTextItem::~ListTextItem()
	{
	}

	void ListTextItem::_initialize(WidgetDesc* d)
	{

		ListItem::_initialize(d);

		mDesc = dynamic_cast<ListTextItemDesc*>(mWidgetDesc);

		ListTextItemDesc* ltid = dynamic_cast<ListTextItemDesc*>(d);

		setSkinType(d->widget_skinTypeName);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = ltid->textDesc;

		mDesc->listtextitem_verticalTextAlignment = ltid->listtextitem_verticalTextAlignment;
		TextUser::_initialize(this,mDesc);
	}

	Ogre::String ListTextItem::getClass()
	{
		return "ListTextItem";
	}

	void ListTextItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = 0;

		switch(mDesc->listtextitem_verticalTextAlignment)
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
}
