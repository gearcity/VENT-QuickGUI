#include "QuickGUITitleBar.h"
#include "QuickGUIWindow.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIButton.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIRoot.h"

namespace QuickGUI
{
	const Ogre::String TitleBar::BACKGROUND = "background";
	const Ogre::String TitleBar::CLOSE_BUTTON = "close_button";

	void TitleBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TitleBar");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(CLOSE_BUTTON);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TitleBar",d);
	}

	TitleBarDesc::TitleBarDesc() :
		ComponentWidgetDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void TitleBarDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		widget_dimensions.size.height = 25;
		widget_dragable = true;
		// Do not set left/right anchoring, it will interfere with manual resizing/repositioning by the Window class.

		titlebar_closeButton = false;
		titlebar_closeButtonPadding = 2;
		titlebar_closeButtonSkinType = "default.close";
		titlebar_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;

		TextUserDesc::resetToDefault();
	}

	TitleBar::TitleBar(const Ogre::String& name) :
		ComponentWidget(name),
		TextUser(),
		mCloseButton(NULL),
		mHeightBuffer(3)
	{
	}

	TitleBar::~TitleBar()
	{
	}

	void TitleBar::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<TitleBarDesc*>(mWidgetDesc);

		setSkinType(d->widget_skinTypeName);

		TitleBarDesc* td = dynamic_cast<TitleBarDesc*>(d);

		// Create CloseButton if property is set.
		if(td->titlebar_closeButton)
		{
			ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
			bd->resetToDefault();
			bd->widget_name = getName() + ".CloseButton";

			if((td->titlebar_closeButtonPadding * 2.0) >= mClientDimensions.size.height)
				throw Exception(Exception::ERR_INVALIDPARAMS,"CloseButtonPadding exceeds height of TitleBar, cannot create Close Button!","TitleBar::_initialize");

			bd->widget_dimensions.size.width = (mClientDimensions.size.height - (td->titlebar_closeButtonPadding * 2.0))/ Root::getSingletonPtr()->getGUIScale()  ;
			bd->widget_dimensions.size.height = (bd->widget_dimensions.size.width);
			bd->widget_dimensions.position.x = mDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT) - td->titlebar_closeButtonPadding - bd->widget_dimensions.size.width;
			bd->widget_dimensions.position.y = td->titlebar_closeButtonPadding;
			bd->widget_skinTypeName = td->titlebar_closeButtonSkinType;
			bd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

			mCloseButton = dynamic_cast<Button*>(_createWidget(bd));
			addComponent(CLOSE_BUTTON,mCloseButton);
			
			mCloseButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&TitleBar::onCloseButtonUp,this);

			mClientDimensions.size.width = mCloseButton->getPosition().x - mSkinElement->getBorderThickness(BORDER_LEFT);
		}

		// Text

		mDesc->titlebar_verticalTextAlignment = td->titlebar_verticalTextAlignment;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;
		mDesc->textDesc.horizontalTextAlignment = TEXT_ALIGNMENT_HORIZONTAL_CENTER;
		mDesc->textDesc.allottedWidth = d->widget_dimensions.size.width;
		TextUser::_initialize(this,mDesc);
	}

	Ogre::String TitleBar::getClass()
	{
		return "TitleBar";
	}

	VerticalTextAlignment TitleBar::getVerticalTextAlignment()
	{
		return mDesc->titlebar_verticalTextAlignment;
	}

	void TitleBar::onCloseButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			mWindow->setVisible(false);
	}

	void TitleBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
		{
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);
		}

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		if(mText->empty())
			return;

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = 0;

		switch(mDesc->titlebar_verticalTextAlignment)
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

	void TitleBar::setVerticalTextAlignment(VerticalTextAlignment a)
	{
		mDesc->titlebar_verticalTextAlignment = a;

		redraw();
	}

	void TitleBar::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinElement != NULL)
		{
			mClientDimensions.position.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = mSkinElement->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));

			if(mCloseButton)
			{
				Point p;
				p.x = mDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT) - mDesc->titlebar_closeButtonPadding - mCloseButton->getWidth();
				p.y = mDesc->titlebar_closeButtonPadding;
				mCloseButton->setPosition(p);

				mClientDimensions.size.width = mCloseButton->getPosition().x - mSkinElement->getBorderThickness(BORDER_LEFT);
			}
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		redraw();
	}
}
