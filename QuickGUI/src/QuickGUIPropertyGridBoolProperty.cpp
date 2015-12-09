#include "QuickGUIPropertyGridBoolProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	void PropertyGridBoolProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridBoolProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridBoolProperty",d);
	}

	PropertyGridBoolPropertyDesc::PropertyGridBoolPropertyDesc() :
		PropertyGridItemDesc()
	{
		resetToDefault();
	}

	void PropertyGridBoolPropertyDesc::resetToDefault()
	{
		PropertyGridItemDesc::resetToDefault();
	}

	void PropertyGridBoolPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridItemDesc::serialize(b);
	}

	PropertyGridBoolProperty::PropertyGridBoolProperty(const Ogre::String& name) :
		PropertyGridItem(name),
		mComboBox(NULL)
	{
	}

	PropertyGridBoolProperty::~PropertyGridBoolProperty()
	{
	}

	void PropertyGridBoolProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridItem::_initialize(d);

		mDesc = dynamic_cast<PropertyGridBoolPropertyDesc*>(mWidgetDesc);

		ComboBoxDesc* cbd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cbd->resetToDefault();
		cbd->widget_dimensions.position.x = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.width = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		cbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

		mComboBox = dynamic_cast<ComboBox*>(_createWidget(cbd));
		addComponent("ComboBoxField",mComboBox);

		ListTextItemDesc* ltid = DescManager::getSingletonPtr()->getDefaultListTextItemDesc();
		ltid->resetToDefault();
		ltid->textDesc.segments.push_back(TextSegment("micross.12",ColourValue::White,"False"));
		mComboBox->createItem(ltid);

		ltid->resetToDefault();
		ltid->textDesc.segments.push_back(TextSegment("micross.12",ColourValue::White,"True"));
		mComboBox->createItem(ltid);

		mComboBox->selectItem(0);

		mFieldWidget = mComboBox;
	}

	Ogre::String PropertyGridBoolProperty::getClass()
	{
		return "PropertyGridBoolProperty";
	}

	bool PropertyGridBoolProperty::getProperty()
	{
		if(mComboBox->getSelectedItem()->getIndex() == 0)
			return false;
		else
			return true;
	}

	void PropertyGridBoolProperty::setProperty(bool b)
	{
		if(b)
			mComboBox->selectItem(1);
		else
			mComboBox->selectItem(0);
	}

	void PropertyGridBoolProperty::updateFieldSkin()
	{
		if((mDesc->propertyGrid != NULL) && (mComboBox != NULL))
			mComboBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->propertyGrid)->getBoolFieldSkin());
	}
}
