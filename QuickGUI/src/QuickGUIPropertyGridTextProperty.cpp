#include "QuickGUIPropertyGridTextProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUITextBox.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	void PropertyGridTextProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridTextProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridTextProperty",d);
	}

	PropertyGridTextPropertyDesc::PropertyGridTextPropertyDesc() :
		PropertyGridItemDesc()
	{
		resetToDefault();
	}

	void PropertyGridTextPropertyDesc::resetToDefault()
	{
		PropertyGridItemDesc::resetToDefault();
	}

	void PropertyGridTextPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridItemDesc::serialize(b);
	}

	PropertyGridTextProperty::PropertyGridTextProperty(const Ogre::String& name) :
		PropertyGridItem(name),
		mTextBox(NULL)
	{
	}

	PropertyGridTextProperty::~PropertyGridTextProperty()
	{
	}

	void PropertyGridTextProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridItem::_initialize(d);

		mDesc = dynamic_cast<PropertyGridTextPropertyDesc*>(mWidgetDesc);

		TextBoxDesc* tbd = DescManager::getSingletonPtr()->getDefaultTextBoxDesc();
		tbd->resetToDefault();
		tbd->widget_dimensions.position.x = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		tbd->widget_dimensions.size.width = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		tbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		tbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

		mTextBox = dynamic_cast<TextBox*>(_createWidget(tbd));
		addComponent("TextBoxField",mTextBox);

		mFieldWidget = mTextBox;
	}

	Ogre::String PropertyGridTextProperty::getClass()
	{
		return "PropertyGridTextProperty";
	}

	void PropertyGridTextProperty::updateFieldSkin()
	{
		if((mDesc->propertyGrid != NULL) && (mTextBox != NULL))
			mTextBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->propertyGrid)->getTextFieldSkin());
	}
}
