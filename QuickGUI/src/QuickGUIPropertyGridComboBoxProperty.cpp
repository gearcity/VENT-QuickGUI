#include "QuickGUIPropertyGridComboBoxProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUIListImageItem.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	void PropertyGridComboBoxProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridComboBoxProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridComboBoxProperty",d);
	}

	PropertyGridComboBoxPropertyDesc::PropertyGridComboBoxPropertyDesc() :
		PropertyGridItemDesc()
	{
		resetToDefault();
	}

	void PropertyGridComboBoxPropertyDesc::resetToDefault()
	{
		PropertyGridItemDesc::resetToDefault();
	}

	void PropertyGridComboBoxPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridItemDesc::serialize(b);
	}

	PropertyGridComboBoxProperty::PropertyGridComboBoxProperty(const Ogre::String& name) :
		PropertyGridItem(name),
		mComboBox(NULL)
	{
	}

	PropertyGridComboBoxProperty::~PropertyGridComboBoxProperty()
	{
	}

	void PropertyGridComboBoxProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridItem::_initialize(d);

		mDesc = dynamic_cast<PropertyGridComboBoxPropertyDesc*>(mWidgetDesc);

		ComboBoxDesc* cbd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cbd->resetToDefault();
		cbd->widget_dimensions.position.x = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.width = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		cbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

		mComboBox = dynamic_cast<ComboBox*>(_createWidget(cbd));
		addComponent("ComboBoxField",mComboBox);

		mFieldWidget = mComboBox;
	}

	ListImageItem* PropertyGridComboBoxProperty::createImageItem(const Ogre::String& imageName, int index)
	{
		return mComboBox->createImageItem(imageName,index);
	}

	ListImageItem* PropertyGridComboBoxProperty::createImageItem(int index)
	{
		return mComboBox->createImageItem(index);
	}

	ListTextItem* PropertyGridComboBoxProperty::createTextItem(const Ogre::UTFString& text, int index)
	{
		return mComboBox->createTextItem(text,index);
	}

	ListTextItem* PropertyGridComboBoxProperty::createTextItem(std::vector<TextSegment> segments, int index)
	{
		return mComboBox->createTextItem(segments,index);
	}

	ListTextItem* PropertyGridComboBoxProperty::createTextItem(int index)
	{
		return mComboBox->createTextItem(index);
	}

	void PropertyGridComboBoxProperty::destroyItem(unsigned int index)
	{
		mComboBox->destroyItem(index);
	}
	
	void PropertyGridComboBoxProperty::destroyItem(ListItem* i)
	{
		mComboBox->destroyItem(i);
	}

	Ogre::String PropertyGridComboBoxProperty::getClass()
	{
		return "PropertyGridComboBoxProperty";
	}

	float PropertyGridComboBoxProperty::getDropDownWidth()
	{
		return mComboBox->getDropDownWidth();
	}

	ListItem* PropertyGridComboBoxProperty::getItem(unsigned int index)
	{
		return mComboBox->getItem(index);
	}

	float PropertyGridComboBoxProperty::getMaxDropDownHeight()
	{
		return mComboBox->getMaxDropDownHeight();
	}

	int PropertyGridComboBoxProperty::getNumberOfItems()
	{
		return mComboBox->getNumberOfItems();
	}

	ListItem* PropertyGridComboBoxProperty::getSelectedItem()
	{
		return mComboBox->getSelectedItem();
	}

	void PropertyGridComboBoxProperty::hideDropDownList()
	{
		mComboBox->hideDropDownList();
	}

	void PropertyGridComboBoxProperty::selectItem(unsigned int index)
	{
		mComboBox->selectItem(index);
	}

	void PropertyGridComboBoxProperty::setDropDownWidth(float width)
	{
		mComboBox->setDropDownWidth(width);
	}

	void PropertyGridComboBoxProperty::setMaxDropDownHeight(float height)
	{
		mComboBox->setMaxDropDownHeight(height);
	}

	void PropertyGridComboBoxProperty::showDropDownList()
	{
		mComboBox->showDropDownList();
	}

	void PropertyGridComboBoxProperty::updateFieldSkin()
	{
		if((mDesc->propertyGrid != NULL) && (mComboBox != NULL))
			mComboBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->propertyGrid)->getComboBoxFieldSkin());
	}
}
