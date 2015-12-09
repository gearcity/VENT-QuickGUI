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

#include "QuickGUITreeViewNode.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUITreeView.h"
#include "QuickGUITreeViewCheckBoxNode.h"
#include "QuickGUITreeViewRadioButtonNode.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIButton.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	const Ogre::String TreeViewNode::BACKGROUND = "background";
	const Ogre::String TreeViewNode::ICON = "icon";
	const Ogre::String TreeViewNode::SELECTED = "selected";

	void TreeViewNode::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TreeViewNode");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TreeViewNode",d);
	}

	TreeViewNodeDesc::TreeViewNodeDesc() :
		ContainerWidgetDesc(),
		TextUserDesc(),
		treeView(NULL)
	{
		resetToDefault();
	}

	void TreeViewNodeDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		TextUserDesc::resetToDefault();

		containerwidget_clipChildrenToDimensions = false;
		containerwidget_supportScrollBars = false;

		treeviewnode_index = -1;
		treeviewnode_selected = false;
	}

	void TreeViewNodeDesc::serialize(SerialBase* b)
	{
		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		TreeViewNodeDesc* defaultValues = DescManager::getSingleton().createDesc<TreeViewNodeDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("Index",		&treeviewnode_index,	defaultValues->treeviewnode_index);
		b->IO("Selected",	&treeviewnode_selected, defaultValues->treeviewnode_selected);

		DescManager::getSingleton().destroyDesc(defaultValues);

		TextUserDesc::serialize(b);
	}

	TreeViewNode::TreeViewNode(const Ogre::String& name) :
		ContainerWidget(name),
		TextUser(),
		mExpanded(false),
		mStateButton(NULL),
		mAutoNameCounter(0)
	{
		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&TreeViewNode::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TreeViewNode::onMouseDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_DOUBLE,&TreeViewNode::onDoubleClick,this);
	}

	TreeViewNode::~TreeViewNode()
	{
	}

	float TreeViewNode::_getFarthestX()
	{
		if(!mDesc->widget_visible)
			return 0;

		float farthestX = getPositionRelativeToTreeView().x + mWidgetDesc->widget_dimensions.size.width;
		float childX = 0;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			childX = (*it)->_getFarthestX();
			if(childX > farthestX)
				farthestX = childX;
		}

		return farthestX;
	}

	void TreeViewNode::_initialize(WidgetDesc* d)
	{		
		TreeViewNodeDesc* tvnd = dynamic_cast<TreeViewNodeDesc*>(d);
		tvnd->widget_consumeKeyboardEvents = true;

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<TreeViewNodeDesc*>(mWidgetDesc);

		mDesc->treeviewnode_index = tvnd->treeviewnode_index;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = tvnd->textDesc;

		TextUser::_initialize(this,mDesc);

		setSkinType(d->widget_skinTypeName);

		if(tvnd->treeviewnode_selected)
			select();
	}

	void TreeViewNode::_selectNode(TreeViewNode* n)
	{
		if(n == this)
			mDesc->treeviewnode_selected = true;
		else
			mDesc->treeviewnode_selected = false;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->_selectNode(n);
	}

	void TreeViewNode::addChild(Widget* w)
	{
		// Try to be efficient and only create the button once we have child nodes.
		createStateButtonIfNotExists();

		ContainerWidget::addChild(w);

		TreeViewNode* tvn = dynamic_cast<TreeViewNode*>(w);

		// Set link to TreeView
		tvn->notifyTreeViewParent(mDesc->treeView);

		tvn->setPositionRelativeToParentClientDimensions(false);
		tvn->setVisible(false);

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

	TreeViewNode* TreeViewNode::createNode(TreeViewNodeDesc* d, int index)
	{
		d->treeviewnode_index = index;

		TreeViewNode* newTreeViewNode = dynamic_cast<TreeViewNode*>(_createWidget(d));

		addChild(newTreeViewNode);

		return newTreeViewNode;
	}

	TreeViewNode* TreeViewNode::createNode(int index) 
	{
		return createNode("Node",index);
	}
	
	TreeViewNode* TreeViewNode::createNode(const Ogre::UTFString& text, int index) 
	{
		TreeViewNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewNodeDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		TreeViewNode* n = createNode(d,index);

		d->resetToDefault();

		return n;
	}

	TreeViewNode* TreeViewNode::createNode(std::vector<TextSegment> segments, int index) 
	{
		TreeViewNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewNodeDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		TreeViewNode* n = createNode(d,index);

		d->resetToDefault();

		return n;
	}

	TreeViewCheckBoxNode* TreeViewNode::createCheckBoxNode(int index) 
	{
		return createCheckBoxNode("CheckBoxNode",index);
	}

	TreeViewCheckBoxNode* TreeViewNode::createCheckBoxNode(const Ogre::UTFString& text, int index) 
	{
		TreeViewCheckBoxNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewCheckBoxNodeDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		TreeViewCheckBoxNode* n = dynamic_cast<TreeViewCheckBoxNode*>(createNode(d,index));

		d->resetToDefault();

		return n;
	}

	TreeViewCheckBoxNode* TreeViewNode::createCheckBoxNode(std::vector<TextSegment> segments, int index) 
	{
		TreeViewCheckBoxNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewCheckBoxNodeDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		TreeViewCheckBoxNode* n = dynamic_cast<TreeViewCheckBoxNode*>(createNode(d,index));

		d->resetToDefault();

		return n;
	}

	TreeViewRadioButtonNode* TreeViewNode::createRadioButtonNode(int index) 
	{
		return createRadioButtonNode("RadioButtonNode",index);
	}

	TreeViewRadioButtonNode* TreeViewNode::createRadioButtonNode(const Ogre::UTFString& text, int index) 
	{
		TreeViewRadioButtonNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewRadioButtonNodeDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		TreeViewRadioButtonNode* n = dynamic_cast<TreeViewRadioButtonNode*>(createNode(d,index));

		d->resetToDefault();

		return n;
	}

	TreeViewRadioButtonNode* TreeViewNode::createRadioButtonNode(std::vector<TextSegment> segments, int index) 
	{
		TreeViewRadioButtonNodeDesc* d = DescManager::getSingletonPtr()->getDefaultTreeViewRadioButtonNodeDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		TreeViewRadioButtonNode* n = dynamic_cast<TreeViewRadioButtonNode*>(createNode(d,index));

		d->resetToDefault();

		return n;
	}

	void TreeViewNode::createStateButtonIfNotExists()
	{
		if(mStateButton != NULL)
			return;

		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();
		bd->widget_dimensions.size.width = mWidgetDesc->widget_dimensions.size.height;
		bd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;

		mStateButton = dynamic_cast<Button*>(_createWidget(bd));
		mStateButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TreeViewNode::onStateButtonClicked,this);
		addComponent("StateButton",mStateButton);

		if(mDesc->treeView != NULL)
		{
			if(mExpanded)
				mStateButton->setSkinType(mDesc->treeView->getExpandSkinTypeName());
			else
				mStateButton->setSkinType(mDesc->treeView->getMinimizeSkinTypeName());
		}
	}

	void TreeViewNode::drawLineToParent()
	{
		if(mParentWidget == NULL)
			return;

		if(mParentWidget->getClass() == "TreeView")
			return;

		if(!mWidgetDesc->widget_visible)
			return;

		Brush* brush = Brush::getSingletonPtr();

		Point p1(mTexturePosition);
		p1.x += mWidgetDesc->widget_dimensions.size.height;
		p1.y += (mWidgetDesc->widget_dimensions.size.height / 2.0);
		p1.round();
		
		Point p2(p1);
		p2.x -= (mWidgetDesc->widget_dimensions.size.height / 2.0);
		p2.round();

		Point p3(p2);
		p3.y = mParentWidget->getScreenPosition().y + mParentWidget->getHeight();

		ColourValue oldCV = brush->getColour();
		brush->setColor(ColourValue::White);

		brush->drawLine(p1,p2);
		brush->drawLine(p2,p3);

		brush->setColor(oldCV);
	}

	void TreeViewNode::drawLines()
	{
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->drawLines();

		drawLineToParent();
	}

	void TreeViewNode::expand()
	{
		// Iterate through parents and make sure they're visible
		Widget* parent = mParentWidget;
		while((parent != NULL) && (parent->getClass() != "TreeView"))
		{
			parent->setVisible(true);

			parent = parent->getParentWidget();
		}

		if(!mNodes.empty())
		{
			for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
				(*it)->setVisible(true);

			mExpanded = true;

			if(mDesc->treeView != NULL)
				mStateButton->setSkinType(mDesc->treeView->getExpandSkinTypeName());
		}

		// Update Node positions
		if(mDesc->treeView != NULL)
			mDesc->treeView->updateNodes();
	}

	Widget* TreeViewNode::findWidgetAtPoint(const Point& p, unsigned int queryFilter, bool ignoreDisabled)
	{
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			Widget* w = (*it).second->findWidgetAtPoint(p,queryFilter,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		for(std::list<Widget*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
		{
			Widget* w = (*it)->findWidgetAtPoint(p,queryFilter,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		return Widget::findWidgetAtPoint(p,ignoreDisabled);
	}

	float TreeViewNode::getBranchHeight()
	{
		float height = 0;

		// If invisible, return 0 height
		if(!mDesc->widget_visible)
			return height;

		// If we made it here we know we're visible, so at the very least include our height
		height += mDesc->widget_dimensions.size.height;

		// Add height of child branches (recursive)
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			height += (*it)->getBranchHeight();

		return height;
	}

	Ogre::String TreeViewNode::getClass()
	{
		return "TreeViewNode";
	}

	int TreeViewNode::getIndex()
	{
		return mDesc->treeviewnode_index;
	}

	TreeViewNode* TreeViewNode::getNextSibling()
	{
		if(mParentWidget == NULL)
			return NULL;

		std::list<TreeViewNode*> list;

		if(mParentWidget->getClass() == "TreeView")
			list = dynamic_cast<TreeView*>(mParentWidget)->getRootNodes();
		else
			list = dynamic_cast<TreeViewNode*>(mParentWidget)->getSiblings();

		bool returnNext = false;
		for(std::list<TreeViewNode*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if(returnNext)
				return (*it);

			if((*it) == this)
				returnNext = true;
		}

		return NULL;
	}

	int TreeViewNode::getNumberOfSiblings()
	{
		return static_cast<int>(mNodes.size());
	}

	TreeViewNode* TreeViewNode::getLastVisibleNode()
	{
		if(!mDesc->widget_visible)
			return NULL;

		// If we have no child nodes or are minimized, return this
		if(!mExpanded || mNodes.empty())
			return this;

		// If there are visible child nodes, make recursive call using last node in list
		return mNodes.back()->getLastVisibleNode();
	}

	TreeViewNode* TreeViewNode::getPreviousSibling()
	{
		if(mParentWidget == NULL)
			return NULL;

		std::list<TreeViewNode*> list;

		if(mParentWidget->getClass() == "TreeView")
			list = dynamic_cast<TreeView*>(mParentWidget)->getRootNodes();
		else
			list = dynamic_cast<TreeViewNode*>(mParentWidget)->getSiblings();

		TreeViewNode* n = NULL;
		for(std::list<TreeViewNode*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if((*it) == this)
				return n;

			n = (*it);
		}

		return NULL;
	}

	std::list<TreeViewNode*> TreeViewNode::getSiblings()
	{
		std::list<TreeViewNode*> list(mNodes);

		return list;
	}

	Point TreeViewNode::getPositionRelativeToTreeView()
	{
		Point p(mWidgetDesc->widget_dimensions.position);

		Widget* w = mParentWidget;
		while((w != NULL) && (w->getClass() != "TreeView"))
		{
			p.translate(w->getPosition());

			w = w->getParentWidget();
		}

		return p;
	}

	bool TreeViewNode::isExpanded()
	{
		return mExpanded;
	}

	bool TreeViewNode::isPositionManaged()
	{
		return true;
	}

	bool TreeViewNode::isSizeManaged()
	{
		return true;
	}

	void TreeViewNode::notifyTreeViewParent(TreeView* t)
	{
		mDesc->treeView = t;

		if(t != NULL)
		{
			float nodeHeight = t->getNodeHeight();

			setHeight(nodeHeight);

			if(mStateButton != NULL)
			{
				if(mExpanded)
					mStateButton->setSkinType(mDesc->treeView->getExpandSkinTypeName());
				else
					mStateButton->setSkinType(mDesc->treeView->getMinimizeSkinTypeName());
			}
		}

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->notifyTreeViewParent(t);
	}

	void TreeViewNode::minimize()
	{
		if(mNodes.empty())
			return;

		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
			(*it)->setVisible(false);

		mExpanded = false;

		if(mDesc->treeView != NULL)
		{
			mStateButton->setSkinType(mDesc->treeView->getMinimizeSkinTypeName());
			mDesc->treeView->updateNodes();
		}
	}

	void TreeViewNode::onDoubleClick(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if((mDesc->treeView != NULL) && (mDesc->treeView->getDoubleClickToToggle()))
				toggle();
		}
	}

	void TreeViewNode::onDraw()
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
		Point iconPosition(mTexturePosition.x + mWidgetDesc->widget_dimensions.size.height,mTexturePosition.y);
		brush->drawSkinElement(Rect(iconPosition,Size(mWidgetDesc->widget_dimensions.size.height)),mSkinType->getSkinElement(ICON));

		// Clip to client dimensions
		Rect clipRegion(mTexturePosition,Size(mWidgetDesc->widget_dimensions.size.width - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height));
		clipRegion.translate(Point(mWidgetDesc->widget_dimensions.size.height * 2,0));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		Point textPosition = clipRegion.position;

		mText->draw(textPosition);

		// Restore Clipping
		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void TreeViewNode::onKeyDown(const EventArgs& args)
	{
		if((mDesc->treeView != NULL) && (!mDesc->treeView->getKeyNavigation()))
			return;

		const KeyEventArgs& kea = dynamic_cast<const KeyEventArgs&>(args);

		switch(kea.scancode)
		{
		case KC_LEFT:
			if(mExpanded)
				minimize();
			else
			{
				if((mParentWidget != NULL) && (mParentWidget->getClass() != "TreeView"))
					mDesc->treeView->selectNode(dynamic_cast<TreeViewNode*>(mParentWidget));
			}
			break;
		case KC_RIGHT:
			if(!mExpanded)
				expand();
			else
			{
				if(static_cast<int>(mNodes.size()) > 0)
					mDesc->treeView->selectNode(mNodes.front());
			}
			break;
		case KC_UP:
			// If we are the first child node
			if(mDesc->treeviewnode_index == 0) 
			{
				// If we are not root nodes, select our parent
				if(mParentWidget->getClass() != "TreeView")
					mDesc->treeView->selectNode(dynamic_cast<TreeViewNode*>(mParentWidget));
			}
			else
				mDesc->treeView->selectNode(getPreviousSibling()->getLastVisibleNode());
			break;
		case KC_DOWN:
			{
				if(mExpanded)
					mDesc->treeView->selectNode(mNodes.front());
				else
				{
					// Determine if we are the last sibling
				
					int numSiblings = 0;
					if(mParentWidget != NULL)
					{
						if(mParentWidget->getClass() == "TreeView")
							numSiblings = dynamic_cast<TreeView*>(mParentWidget)->getNumberOfRootNodes();
						else
							numSiblings = dynamic_cast<TreeViewNode*>(mParentWidget)->getNumberOfSiblings();
					}

					// If we are the last child node, iterate up through parents and select the next sibling if exists
					if(mDesc->treeviewnode_index == (numSiblings - 1))
					{
						Widget* w = mParentWidget;
						while(w->getClass() != "TreeView")
						{
							TreeViewNode* n = dynamic_cast<TreeViewNode*>(w)->getNextSibling();

							if(n != NULL)
							{
								mDesc->treeView->selectNode(n);
								break;
							}

							w = w->getParentWidget();
						}
					}
					// If there are siblings below us, just select the next sibling
					else
						mDesc->treeView->selectNode(getNextSibling());
				}
			}
			break;
		case KC_SPACE:
			if(getClass() == "TreeViewCheckBoxNode")
			{
				TreeViewCheckBoxNode* tvcbn = dynamic_cast<TreeViewCheckBoxNode*>(this);
				tvcbn->setChecked(!tvcbn->getChecked());
			}
			else if(getClass() == "TreeViewRadioButtonNode")
				dynamic_cast<TreeViewRadioButtonNode*>(this)->selectRadioButton();
			break;
		}
	}

	void TreeViewNode::onMouseDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			select();
	}

	void TreeViewNode::onStateButtonClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if((mea.button == MB_Left) && !(mea.autoRepeat))
			toggle();
	}

	void TreeViewNode::select()
	{
		if(mDesc->treeView != NULL)
			mDesc->treeView->selectNode(this);
	}

	void TreeViewNode::setHeight(float pixelHeight)
	{
		ContainerWidget::setHeight(pixelHeight);

		// Determine width, based on the type and height
		Ogre::String className = getClass();

		// For TreeViewNode our width will be: [State][Icon][Text][5 pixel buffer]
		if(className == "TreeViewNode")
			setWidth((pixelHeight * 2) + getTextWidth() + 5);
		// For TreeViewNode our width will be: [State][CheckBox][Icon][Text][5 pixel buffer]
		else if(className == "TreeViewCheckBoxNode")
			setWidth((pixelHeight * 3) + getTextWidth() + 5);
		// For TreeViewNode our width will be: [State][RadioButton][Icon][Text][5 pixel buffer]
		else if(className == "TreeViewRadioButtonNode")
			setWidth((pixelHeight * 3) + getTextWidth() + 5);
	}

	void TreeViewNode::toggle()
	{
		if(mExpanded)
			minimize();
		else
			expand();
	}

	void TreeViewNode::updateClientDimensions()
	{
		ContainerWidget::updateClientDimensions();

		if(mStateButton != NULL)
		{
			mStateButton->setWidth(mDesc->widget_dimensions.size.height);
			mStateButton->setHeight(mDesc->widget_dimensions.size.height);
		}

		if(mText != NULL)
			mText->setAllottedHeight(mClientDimensions.size.height);
	}

	void TreeViewNode::updateNodes()
	{
		float x = mDesc->widget_dimensions.size.height;
		float y = mDesc->widget_dimensions.size.height;
		unsigned int counter = 0;
		for(std::list<TreeViewNode*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
		{
			(*it)->setPosition(Point(x,y));
			(*it)->mDesc->treeviewnode_index = counter;
			(*it)->updateNodes();

			y += (*it)->getBranchHeight();
			++counter;
		}
	}
}
