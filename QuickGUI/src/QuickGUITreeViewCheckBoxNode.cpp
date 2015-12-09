#include "QuickGUITreeViewCheckBoxNode.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIEventHandlerManager.h"

namespace QuickGUI
{
	const Ogre::String TreeViewCheckBoxNode::CHECKBOX = "checkbox";

	void TreeViewCheckBoxNode::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TreeViewCheckBoxNode");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->defineComponent(CHECKBOX);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TreeViewCheckBoxNode",d);
	}

	TreeViewCheckBoxNodeDesc::TreeViewCheckBoxNodeDesc() :
		TreeViewNodeDesc()
	{
		resetToDefault();
	}

	void TreeViewCheckBoxNodeDesc::resetToDefault()
	{
		TreeViewNodeDesc::resetToDefault();

		treeviewcheckboxnode_checked = false;
		for(int index = 0; index < TREEVIEWCHECKBOXNODE_EVENT_COUNT; ++index)
			treeviewcheckboxnode_userHandlers[index] = "";
	}

	void TreeViewCheckBoxNodeDesc::serialize(SerialBase* b)
	{
		TreeViewNodeDesc::serialize(b);

		b->IO("Checked",&treeviewcheckboxnode_checked);

		if(b->begin("UserDefinedHandlers","TreeViewCheckBoxNodeEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < TREEVIEWCHECKBOXNODE_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<TreeViewCheckBoxNodeEvent>(index)),&(treeviewcheckboxnode_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < TREEVIEWCHECKBOXNODE_EVENT_COUNT; ++index)
				{
					if(treeviewcheckboxnode_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<TreeViewCheckBoxNodeEvent>(index)),&(treeviewcheckboxnode_userHandlers[index]));
				}
			}
			b->end();
		}
	}

	TreeViewCheckBoxNode::TreeViewCheckBoxNode(const Ogre::String& name) :
		TreeViewNode(name),
		mCheckBox(NULL)
	{
	}

	TreeViewCheckBoxNode::~TreeViewCheckBoxNode()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < TREEVIEWCHECKBOXNODE_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mTreeViewCheckBoxNodeEventHandlers[index].begin(); it != mTreeViewCheckBoxNodeEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void TreeViewCheckBoxNode::_initialize(WidgetDesc* d)
	{		
		TreeViewCheckBoxNodeDesc* tvcbnd = dynamic_cast<TreeViewCheckBoxNodeDesc*>(d);
		
		TreeViewNode::_initialize(d);

		mDesc = dynamic_cast<TreeViewCheckBoxNodeDesc*>(mWidgetDesc);

		for(int index = 0; index < TREEVIEWCHECKBOXNODE_EVENT_COUNT; ++index)
			mDesc->treeviewcheckboxnode_userHandlers[index] = tvcbnd->treeviewcheckboxnode_userHandlers[index];

		CheckBoxDesc* cbd = DescManager::getSingletonPtr()->getDefaultCheckBoxDesc();
		cbd->resetToDefault();
		cbd->widget_dimensions.size.width = mWidgetDesc->widget_dimensions.size.height;
		cbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		cbd->widget_dimensions.position.x = mWidgetDesc->widget_dimensions.size.height;

		mCheckBox = dynamic_cast<CheckBox*>(_createWidget(cbd));
		mCheckBox->addCheckBoxEventHandler(CHECKBOX_EVENT_CHECK_CHANGED,&TreeViewCheckBoxNode::onCheckChanged,this);
		addComponent(CHECKBOX,mCheckBox);

		setSkinType(d->widget_skinTypeName);

		if(tvcbnd->treeviewcheckboxnode_checked)
			setChecked(true);
	}

	void TreeViewCheckBoxNode::addUserDefinedTreeViewCheckBoxNodeEventHandler(TreeViewCheckBoxNodeEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->treeviewcheckboxnode_userHandlers[EVENT] = handlerName;
	}

	void TreeViewCheckBoxNode::addTreeViewCheckBoxNodeEventHandler(TreeViewCheckBoxNodeEvent EVENT, EventHandlerSlot* function)
	{
		mTreeViewCheckBoxNodeEventHandlers[EVENT].push_back(function);
	}

	void TreeViewCheckBoxNode::clearUserDefinedTreeViewCheckBoxNodeEventHandler(TreeViewCheckBoxNodeEvent EVENT)
	{
		mDesc->treeviewcheckboxnode_userHandlers[EVENT] = "";
	}

	bool TreeViewCheckBoxNode::fireTreeViewCheckBoxNodeEvent(TreeViewCheckBoxNodeEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mTreeViewCheckBoxNodeEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->treeviewcheckboxnode_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->treeviewcheckboxnode_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	bool TreeViewCheckBoxNode::getChecked()
	{
		return mCheckBox->getChecked();
	}

	Ogre::String TreeViewCheckBoxNode::getClass()
	{
		return "TreeViewCheckBoxNode";
	}

	void TreeViewCheckBoxNode::onCheckChanged(const EventArgs& args)
	{
		mDesc->treeviewcheckboxnode_checked = mCheckBox->getChecked();

		WidgetEventArgs wea(this);
		fireTreeViewCheckBoxNodeEvent(TREEVIEWCHECKBOXNODE_EVENT_CHECK_CHANGED,wea);
	}

	void TreeViewCheckBoxNode::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

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

	void TreeViewCheckBoxNode::setChecked(bool checked)
	{
		mCheckBox->setChecked(checked);
	}

	void TreeViewCheckBoxNode::updateClientDimensions()
	{
		TreeViewNode::updateClientDimensions();

		if(mCheckBox != NULL)
		{
			mCheckBox->setWidth(mDesc->widget_dimensions.size.height);
			mCheckBox->setHeight(mDesc->widget_dimensions.size.height);
			mCheckBox->setPosition(Point(mDesc->widget_dimensions.size.height,0));
		}
	}
}
