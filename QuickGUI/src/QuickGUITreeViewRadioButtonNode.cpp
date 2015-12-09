#include "QuickGUITreeViewRadioButtonNode.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIRadioButton.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIEventHandlerManager.h"

namespace QuickGUI
{
	const Ogre::String TreeViewRadioButtonNode::RADIOBUTTON = "radiobutton";

	void TreeViewRadioButtonNode::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TreeViewRadioButtonNode");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->defineComponent(RADIOBUTTON);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TreeViewRadioButtonNode",d);
	}

	TreeViewRadioButtonNodeDesc::TreeViewRadioButtonNodeDesc() :
		TreeViewNodeDesc()
	{
		resetToDefault();
	}

	void TreeViewRadioButtonNodeDesc::resetToDefault()
	{
		TreeViewNodeDesc::resetToDefault();

		treeviewradiobuttonnode_radioButtonSelected = false;
		for(int index = 0; index < TREEVIEWRADIOBUTTONNODE_EVENT_COUNT; ++index)
			treeviewradiobuttonnode_userHandlers[index] = "";
	}

	void TreeViewRadioButtonNodeDesc::serialize(SerialBase* b)
	{
		TreeViewNodeDesc::serialize(b);

		b->IO("RadioButtonSelected",&treeviewradiobuttonnode_radioButtonSelected);

		if(b->begin("UserDefinedHandlers","TreeViewRadioButtonNodeEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < TREEVIEWRADIOBUTTONNODE_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<TreeViewRadioButtonNodeEvent>(index)),&(treeviewradiobuttonnode_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < TREEVIEWRADIOBUTTONNODE_EVENT_COUNT; ++index)
				{
					if(treeviewradiobuttonnode_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<TreeViewRadioButtonNodeEvent>(index)),&(treeviewradiobuttonnode_userHandlers[index]));
				}
			}
			b->end();
		}
	}

	TreeViewRadioButtonNode::TreeViewRadioButtonNode(const Ogre::String& name) :
		TreeViewNode(name),
		mRadioButton(NULL)
	{
	}

	TreeViewRadioButtonNode::~TreeViewRadioButtonNode()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < TREEVIEWRADIOBUTTONNODE_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mTreeViewRadioButtonNodeEventHandlers[index].begin(); it != mTreeViewRadioButtonNodeEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void TreeViewRadioButtonNode::_initialize(WidgetDesc* d)
	{		
		TreeViewRadioButtonNodeDesc* tvrbnd = dynamic_cast<TreeViewRadioButtonNodeDesc*>(d);
		
		TreeViewNode::_initialize(d);

		mDesc = dynamic_cast<TreeViewRadioButtonNodeDesc*>(mWidgetDesc);

		for(int index = 0; index < TREEVIEWRADIOBUTTONNODE_EVENT_COUNT; ++index)
			mDesc->treeviewradiobuttonnode_userHandlers[index] = tvrbnd->treeviewradiobuttonnode_userHandlers[index];

		RadioButtonDesc* rbd = DescManager::getSingletonPtr()->getDefaultRadioButtonDesc();
		rbd->resetToDefault();
		rbd->widget_dimensions.size.width = mWidgetDesc->widget_dimensions.size.height;
		rbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		rbd->widget_dimensions.position.x = mWidgetDesc->widget_dimensions.size.height;

		mRadioButton = dynamic_cast<RadioButton*>(_createWidget(rbd));
		mRadioButton->addRadioButtonEventHandler(RADIOBUTTON_EVENT_SELECTED,&TreeViewRadioButtonNode::onRadioButtonSelected,this);
		addComponent(RADIOBUTTON,mRadioButton);

		setSkinType(d->widget_skinTypeName);

		if(tvrbnd->treeviewradiobuttonnode_radioButtonSelected)
			selectRadioButton();
	}

	void TreeViewRadioButtonNode::addUserDefinedTreeViewRadioButtonNodeEventHandler(TreeViewRadioButtonNodeEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->treeviewradiobuttonnode_userHandlers[EVENT] = handlerName;
	}

	void TreeViewRadioButtonNode::addTreeViewRadioButtonNodeEventHandler(TreeViewRadioButtonNodeEvent EVENT, EventHandlerSlot* function)
	{
		mTreeViewRadioButtonNodeEventHandlers[EVENT].push_back(function);
	}

	void TreeViewRadioButtonNode::clearUserDefinedTreeViewRadioButtonNodeEventHandler(TreeViewRadioButtonNodeEvent EVENT)
	{
		mDesc->treeviewradiobuttonnode_userHandlers[EVENT] = "";
	}

	bool TreeViewRadioButtonNode::fireTreeViewRadioButtonNodeEvent(TreeViewRadioButtonNodeEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mTreeViewRadioButtonNodeEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->treeviewradiobuttonnode_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->treeviewradiobuttonnode_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	Ogre::String TreeViewRadioButtonNode::getClass()
	{
		return "TreeViewRadioButtonNode";
	}

	bool TreeViewRadioButtonNode::getRadioButtonSelected()
	{
		return mRadioButton->getSelected();
	}

	void TreeViewRadioButtonNode::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		// Draw Background
		Point backgroundPosition(mTexturePosition);
		backgroundPosition.x += mWidgetDesc->widget_dimensions.size.height;
		brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		// if Selected, draw Selected SkinElement
		if(mDesc->treeviewnode_selected)
			brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),mSkinType->getSkinElement(SELECTED));

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Draw Icon
		Point iconPosition(mTexturePosition.x + (mWidgetDesc->widget_dimensions.size.height * 2),mTexturePosition.y);
		brush->drawSkinElement(Rect(iconPosition,Size(mWidgetDesc->widget_dimensions.size.height)),mSkinType->getSkinElement(ICON));

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);

		// Clip to client dimensions
		Rect clipRegion(mTexturePosition,Size(mWidgetDesc->widget_dimensions.size.width - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height));
		clipRegion.translate(Point(mWidgetDesc->widget_dimensions.size.height * 3,0));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		Point textPosition = clipRegion.position;
		textPosition.y += yPos;		

		mText->draw(textPosition);

		// Restore Clipping
		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void TreeViewRadioButtonNode::onRadioButtonSelected(const EventArgs& args)
	{
		if((mParentWidget != NULL) && (mParentWidget->isContainerWidget()))
		{
			std::vector<Widget*> children = dynamic_cast<ContainerWidget*>(mParentWidget)->getChildren();
			for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it)
			{
				if(((*it) != this) && ((*it)->getClass() == "TreeViewRadioButtonNode"))
				{
					RadioButton* rb = dynamic_cast<TreeViewRadioButtonNode*>((*it))->mRadioButton;
					rb->mDesc->radiobutton_selected = false;
					rb->mSkinElement = rb->mSkinType->getSkinElement(RadioButton::UNSELECTED);
				}
			}
		}

		mDesc->treeviewradiobuttonnode_radioButtonSelected = true;

		WidgetEventArgs wea(this);
		fireTreeViewRadioButtonNodeEvent(TREEVIEWRADIOBUTTONNODE_EVENT_RADIOBUTTON_SELECTED,wea);
	}

	void TreeViewRadioButtonNode::selectRadioButton()
	{
		mRadioButton->select();
	}

	void TreeViewRadioButtonNode::updateClientDimensions()
	{
		TreeViewNode::updateClientDimensions();

		if(mRadioButton != NULL)
		{
			mRadioButton->setWidth(mDesc->widget_dimensions.size.height);
			mRadioButton->setHeight(mDesc->widget_dimensions.size.height);
			mRadioButton->setPosition(Point(mDesc->widget_dimensions.size.height,0));
		}
	}
}
