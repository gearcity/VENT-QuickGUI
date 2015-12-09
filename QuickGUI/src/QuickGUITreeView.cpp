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

#include "QuickGUITreeView.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISheet.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUITreeViewNode.h"
#include "QuickGUITreeViewCheckBoxNode.h"
#include "QuickGUITreeViewRadioButtonNode.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIVScrollBar.h"

#include "OgreStringConverter.h"

namespace QuickGUI
{
	const Ogre::String TreeView::BACKGROUND = "background";
	const Ogre::String TreeView::EXPAND = "expand";
	const Ogre::String TreeView::MINIMIZE = "minimize";

	void TreeView::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TreeView");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinReference(EXPAND,"Button");
		d->defineSkinReference(MINIMIZE,"Button");
		d->defineSkinReference(HSCROLLBAR,"HScrollBar");
		d->defineSkinReference(VSCROLLBAR,"VScrollBar");
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TreeView",d);
	}

	TreeViewDesc::TreeViewDesc() :
		ContainerWidgetDesc()
	{
		resetToDefault();
	}

	void TreeViewDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(125,100);

		treeview_doubleClickToToggle = true;
		treeview_keyNavigation = true;
		treeview_nodeHeight = 25;

		for(int index = 0; index < TREEVIEW_EVENT_COUNT; ++index)
			treeview_userHandlers[index] = "";
	}

	void TreeViewDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		TreeViewDesc* defaultValues = DescManager::getSingleton().createDesc<TreeViewDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("DoubleClickToToggle",	&treeview_doubleClickToToggle,	defaultValues->treeview_doubleClickToToggle);
		b->IO("KeyNavigation",			&treeview_keyNavigation,		defaultValues->treeview_keyNavigation);
		b->IO("NodeHeight",				&treeview_nodeHeight,			defaultValues->treeview_nodeHeight);

		DescManager::getSingleton().destroyDesc(defaultValues);

		if(b->begin("UserDefinedHandlers","TreeViewEvent"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < TREEVIEW_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<TreeViewEvent>(index)),&(treeview_userHandlers[index]),"");
			}
			else
			{
				for(int index = 0; index < TREEVIEW_EVENT_COUNT; ++index)
				{
					if(treeview_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<TreeViewEvent>(index)),&(treeview_userHandlers[index]),"");
				}
			}
			b->end();
		}
	}

	TreeView::TreeView(const Ogre::String& name) :
		ContainerWidget(name),
		mSelectedNode(NULL),
		mAutoNameCounter(0),
		mDesc(NULL)
	{
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TreeView::onMouseDown,this);
	}

	TreeView::~TreeView()
	{
	}

	float TreeView::_getFarthestX()
	{
		float farthestX = 0;
		float childX = 0;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			childX = (*it)->_getFarthestX();
			if(childX > farthestX)
				farthestX = childX;
		}

		return farthestX;
	}

	void TreeView::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		TreeViewDesc* tvd = dynamic_cast<TreeViewDesc*>(d);
		mDesc = dynamic_cast<TreeViewDesc*>(mWidgetDesc);

		for(int index = 0; index < TREEVIEW_EVENT_COUNT; ++index)
			mDesc->treeview_userHandlers[index] = tvd->treeview_userHandlers[index];

		setSkinType(d->widget_skinTypeName);

		setDoubleClickToToggle(tvd->treeview_doubleClickToToggle);
		setKeyNavigation(tvd->treeview_keyNavigation);
		setNodeHeight(tvd->treeview_nodeHeight);
	}

	void TreeView::addChild(Widget* w)
	{
		ContainerWidget::addChild(w);

		TreeViewNode* tvn = dynamic_cast<TreeViewNode*>(w);

		// Set link to TreeView
		tvn->notifyTreeViewParent(this);

		int index = tvn->getIndex();

		// If we're adding to the end, things are simple - don't have to update other node names, or positions.
		if((index < 0) || (index >= static_cast<int>(mNodes.size())))
		{
			mNodes.push_back(tvn);
		}
		else
		{
			// If we've made it here, we have to insert the node into the list, and update all node indices and positions.
			int counter = 0;
			for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			{
				if(counter == index)
				{
					mNodes.insert(it,tvn);
					break;
				}

				++counter;
			}
		}

		updateNodes();
	}

	void TreeView::addUserDefinedTreeViewEventHandler(TreeViewEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->treeview_userHandlers[EVENT] = handlerName;
	}

	void TreeView::addTreeViewEventHandler(TreeViewEvent EVENT, EventHandlerSlot* function)
	{
		mTreeViewEventHandlers[EVENT].push_back(function);
	}

	void TreeView::clearUserDefinedTreeViewEventHandler(TreeViewEvent EVENT)
	{
		mDesc->treeview_userHandlers[EVENT] = "";
	}


	TreeViewNode* TreeView::createNode(TreeViewNodeDesc* d, int index)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.Node." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->treeviewnode_index = index;

		TreeViewNode* newTreeViewNode = dynamic_cast<TreeViewNode*>(_createWidget(d));

		addChild(newTreeViewNode);

		return newTreeViewNode;
	}

	TreeViewNode* TreeView::createNode(int index)
	{
		return createNode("Node",index);
	}

	TreeViewNode* TreeView::createNode(const Ogre::UTFString& text, int index)
	{
		TreeViewNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewNodeDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		TreeViewNode* n = createNode(d,index);

		d->resetToDefault();

		return n;
	}

	TreeViewNode* TreeView::createNode(std::vector<TextSegment> segments, int index)
	{
		TreeViewNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewNodeDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		TreeViewNode* n = createNode(d,index);

		d->resetToDefault();

		return n;
	}

	TreeViewCheckBoxNode* TreeView::createCheckBoxNode(int index)
	{
		return createCheckBoxNode("CheckBoxNode",index);
	}

	TreeViewCheckBoxNode* TreeView::createCheckBoxNode(const Ogre::UTFString& text, int index)
	{
		TreeViewCheckBoxNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewCheckBoxNodeDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		TreeViewCheckBoxNode* n = dynamic_cast<TreeViewCheckBoxNode*>(createNode(d,index));

		d->resetToDefault();

		return n;
	}

	TreeViewCheckBoxNode* TreeView::createCheckBoxNode(std::vector<TextSegment> segments, int index)
	{
		TreeViewCheckBoxNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewCheckBoxNodeDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		TreeViewCheckBoxNode* n = dynamic_cast<TreeViewCheckBoxNode*>(createNode(d,index));

		d->resetToDefault();

		return n;
	}

	TreeViewRadioButtonNode* TreeView::createRadioButtonNode(int index)
	{
		return createRadioButtonNode("RadioButtonNode",index);
	}

	TreeViewRadioButtonNode* TreeView::createRadioButtonNode(const Ogre::UTFString& text, int index)
	{
		TreeViewRadioButtonNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewRadioButtonNodeDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		TreeViewRadioButtonNode* n = dynamic_cast<TreeViewRadioButtonNode*>(createNode(d,index));

		d->resetToDefault();

		return n;
	}

	TreeViewRadioButtonNode* TreeView::createRadioButtonNode(std::vector<TextSegment> segments, int index)
	{
		TreeViewRadioButtonNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewRadioButtonNodeDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		TreeViewRadioButtonNode* n = dynamic_cast<TreeViewRadioButtonNode*>(createNode(d,index));

		d->resetToDefault();

		return n;
	}

	bool TreeView::fireTreeViewEvent(TreeViewEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mTreeViewEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->treeview_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->treeview_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	Ogre::String TreeView::getClass()
	{
		return "TreeView";
	}

	bool TreeView::getDoubleClickToToggle()
	{
		return mDesc->treeview_doubleClickToToggle;
	}

	Ogre::String TreeView::getExpandSkinTypeName()
	{
		return mSkinType->getSkinReference(EXPAND)->typeName;
	}

	bool TreeView::getKeyNavigation()
	{
		return mDesc->treeview_keyNavigation;
	}

	float TreeView::getNodeHeight()
	{
		return mDesc->treeview_nodeHeight;
	}

	int TreeView::getNumberOfRootNodes()
	{
		return static_cast<int>(mNodes.size());
	}

	Ogre::String TreeView::getMinimizeSkinTypeName()
	{
		return mSkinType->getSkinReference(MINIMIZE)->typeName;
	}

	std::list<TreeViewNode*> TreeView::getRootNodes()
	{
		std::list<TreeViewNode*> list(mNodes);

		return list;
	}

	TreeViewNode* TreeView::getSelectedNode()
	{
		return mSelectedNode;
	}

	void TreeView::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);

		Rect clipRegion(mTexturePosition,Size(mWidgetDesc->widget_dimensions.size));
		Rect prevClipRegion = brush->getClipRegion();
		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		brush->setClipRegion(prevClipRegion);
	}

	void TreeView::onMouseDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			selectNode(NULL);
	}

	void TreeView::removeEventHandlers(void* obj)
	{
		ContainerWidget::removeEventHandlers(obj);

		for(int index = 0; index < TREEVIEW_EVENT_COUNT; ++index)
		{
			std::vector<EventHandlerSlot*> updatedList;
			std::vector<EventHandlerSlot*> listToCleanup;

			for(std::vector<EventHandlerSlot*>::iterator it = mTreeViewEventHandlers[index].begin(); it != mTreeViewEventHandlers[index].end(); ++it)
			{
				if((*it)->getClass() == obj)
					listToCleanup.push_back((*it));
				else
					updatedList.push_back((*it));
			}

			mTreeViewEventHandlers[index].clear();
			for(std::vector<EventHandlerSlot*>::iterator it = updatedList.begin(); it != updatedList.end(); ++it)
				mTreeViewEventHandlers[index].push_back((*it));

			for(std::vector<EventHandlerSlot*>::iterator it = listToCleanup.begin(); it != listToCleanup.end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void TreeView::setDoubleClickToToggle(bool enable)
	{
		mDesc->treeview_doubleClickToToggle = enable;
	}

	void TreeView::setKeyNavigation(bool enable)
	{
		mDesc->treeview_keyNavigation = enable;
	}

	void TreeView::selectNode(TreeViewNode* n)
	{
		mSelectedNode = n;
		mDesc->sheet->setKeyboardListener(n);

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->_selectNode(n);

		// Scroll node into view.  Node may be above or below current view,
		// so make sure both top and bottom are visible.
		if(n != NULL)
		{
			Point p = n->getPositionRelativeToTreeView();

			// scroll top into view
			scrollPointIntoView(p);

			// scroll bottom into view
			p.x += n->getWidth();
			p.y += n->getHeight();
			scrollPointIntoView(p);
		}

		redraw();

		WidgetEventArgs wea(this);
		fireTreeViewEvent(TREEVIEW_EVENT_SELECTION_CHANGED,wea);
	}

	void TreeView::setNodeHeight(float nodeHeightInPixels)
	{
		nodeHeightInPixels = Ogre::Math::Floor(nodeHeightInPixels + 0.5);

		mDesc->treeview_nodeHeight = nodeHeightInPixels;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->setHeight(nodeHeightInPixels);

		updateNodes();
	}

	void TreeView::updateNodes()
	{
		float x = 0;
		float y = 0;
		unsigned int counter = 0;
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			(*it)->setPosition(Point(x,y));
			(*it)->mDesc->treeviewnode_index = counter;
			(*it)->updateNodes();

			y += (*it)->getBranchHeight();
			++counter;
		}

		mTotalHeight = y;

		updateVirtualDimensions();
		// In case Nodes have been minimized, re-apply scrolling offsets
		_setScrollX(mHScrollBar->getPercentage());
		_setScrollY(mVScrollBar->getPercentage());
	}

	void TreeView::updateVirtualDimensions()
	{
		if(mDesc == NULL)
			return;

		if(!mDesc->containerwidget_supportScrollBars)
			return;

		mVirtualSize = mClientDimensions.size;

		// Farthest X is the distance from client origin.
		float farthestX = _getFarthestX();
		if(farthestX > mVirtualSize.width)
			mVirtualSize.width = farthestX;

		if(mTotalHeight > mVirtualSize.height)
			mVirtualSize.height = mTotalHeight;

		// Set Slider width/height
		mHScrollBar->setSliderWidth((mClientDimensions.size.width / mVirtualSize.width) * mHScrollBar->getSliderBounds().size.width);
		mVScrollBar->setSliderHeight((mClientDimensions.size.height / mVirtualSize.height) * mVScrollBar->getSliderBounds().size.height);

		// Now that sliders have changed in size, adjust scroll according to slider positions
		_setScrollX(mHScrollBar->getPercentage());
		_setScrollY(mVScrollBar->getPercentage());

		_determineScrollBarVisibility();
	}
}
