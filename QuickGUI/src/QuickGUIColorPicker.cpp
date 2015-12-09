#include "QuickGUIColorPicker.h"
#include "QuickGUISkinDefinitionManager.h"
#include "OgreTextureManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	const Ogre::String ColorPicker::BACKGROUND = "background";
	const Ogre::String ColorPicker::COLORSOURCE = "colorsource";
	const Ogre::String ColorPicker::TRANSPARENCYBACKGROUND = "transparencybackground";
	const Ogre::String ColorPicker::TRANSPARENCYSLIDER = "transparencyslider";

	void ColorPicker::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ColorPicker");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(COLORSOURCE);
		d->defineSkinElement(TRANSPARENCYBACKGROUND);
		d->defineComponent(TRANSPARENCYSLIDER);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ColorPicker",d);
	}

	ColorPickerDesc::ColorPickerDesc() :
		ComponentWidgetDesc()
	{
		resetToDefault();
	}

	void ColorPickerDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(235,225);
	}

	void ColorPickerDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);
	}

	ColorPicker::ColorPicker(const Ogre::String& name) :
		ComponentWidget(name),
		mColorSource(NULL),
		mTransparencySlider(NULL)
	{
	}

	ColorPicker::~ColorPicker()
	{
	}

	void ColorPicker::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<ColorPickerDesc*>(mWidgetDesc);

		ColorPickerDesc* cpd = dynamic_cast<ColorPickerDesc*>(d);

		ImageDesc* id = DescManager::getSingletonPtr()->getDefaultImageDesc();
		id->resetToDefault();

		// Create Color Source Image
		id->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		id->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		id->widget_dimensions.position = Point(20,55);
		id->widget_dimensions.size = Size(150,150);

		mColorSource = dynamic_cast<Image*>(_createWidget(id));
		mColorSource->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&ColorPicker::onMouseDownOnImage,this);
		mColorSource->addWidgetEventHandler(WIDGET_EVENT_MOUSE_MOVE,&ColorPicker::onMouseMovedOverImage,this);
		addComponent(COLORSOURCE,mColorSource);

		setSkinType(cpd->widget_skinTypeName);
	}

	Ogre::String ColorPicker::getClass()
	{
		return "ColorPicker";
	}

	void ColorPicker::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		// Draw Background
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		// Get Current settings
		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);

		// Set clipping region to client area
		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Draw Transparency Slider Region
		Rect transparencySliderRegion(Point(mTexturePosition),Size(20,mColorSource->getHeight()));
		Point p = mColorSource->getPosition();
		transparencySliderRegion.translate(Point(p.x + mColorSource->getWidth() + 25,p.y));

		brush->drawSkinElement(transparencySliderRegion,mSkinType->getSkinElement(TRANSPARENCYBACKGROUND));

/*
		if(mDesc->colorpicker_imageName != "")
		{
			ColourValue prevColor = brush->getColour();
			Rect prevClipRegion = brush->getClipRegion();

			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			brush->setTexture(mDesc->colorpicker_imageName);
			brush->drawRectangle(prevClipRegion.getIntersection(clipRegion),UVRect(0,0,1,1));

			brush->setClipRegion(prevClipRegion);
			Brush::getSingleton().setColor(prevColor);
		}
*/	}

	void ColorPicker::onMouseDownOnImage(const EventArgs& args)
	{
	}

	void ColorPicker::onMouseMovedOverImage(const EventArgs& args)
	{
	}
}
