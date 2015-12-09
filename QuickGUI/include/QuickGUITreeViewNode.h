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

#ifndef QUICKGUITREEVIEWNODE_H
#define QUICKGUITREEVIEWNODE_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUITextUser.h"

#include <list>

namespace QuickGUI
{
	// Forward declaration
	class Button;
	class TreeView;
	class TreeViewCheckBoxNode;
	class TreeViewRadioButtonNode;

	class _QuickGUIExport TreeViewNodeDesc :
		public ContainerWidgetDesc,
		public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		TreeViewNodeDesc();
		virtual ~TreeViewNodeDesc() {}
	public:

		int treeviewnode_index;
		bool treeviewnode_selected;

		// Nodes should store a pointer to the Tree they belong to
		TreeView* treeView;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TreeViewNodeDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TreeViewNode"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TreeViewNode :
		public ContainerWidget,
		public TextUser
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String ICON;
		static const Ogre::String SELECTED;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// TreeView has to set positions, toggle visibility to manage ListItems
		friend class TreeView;

		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Adds a TreeViewNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		*/
		TreeViewNode* createNode(int index = -1);
		TreeViewNode* createNode(const Ogre::UTFString& text, int index = -1);
		TreeViewNode* createNode(std::vector<TextSegment> segments, int index = -1);
		/**
		* Adds a TreeViewCheckBoxNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		*/
		TreeViewCheckBoxNode* createCheckBoxNode(int index = -1);
		TreeViewCheckBoxNode* createCheckBoxNode(const Ogre::UTFString& text, int index = -1);
		TreeViewCheckBoxNode* createCheckBoxNode(std::vector<TextSegment> segments, int index = -1);
		/**
		* Adds a TreeViewCheckBoxNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		*/
		TreeViewRadioButtonNode* createRadioButtonNode(int index = -1);
		TreeViewRadioButtonNode* createRadioButtonNode(const Ogre::UTFString& text, int index = -1);
		TreeViewRadioButtonNode* createRadioButtonNode(std::vector<TextSegment> segments, int index = -1);

		/**
		* Expands the node, showing all immediate children. 
		* NOTE: Any parents that aren't visible will be shown.
		*/
		void expand();

		/**
		* Returns any child or component widgets under the position specified, or NULL if
		* no child or component widgets are under the position.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, unsigned int queryFilter = -1, bool ignoreDisabled = true);

		/**
		* Returns the total height of this branch.  Only visible Nodes are used to
		* calculate the height.
		*/
		float getBranchHeight();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the index of this node.
		*/
		int getIndex();
		/**
		* Returns node below this one at the same level, or NULL if this is the last node.
		*/
		TreeViewNode* getNextSibling();
		/**
		* Returns the number of nodes belonging to this node.
		*/
		int getNumberOfSiblings();
		/**
		* Recursive function that returns the last visible node found.
		*/
		TreeViewNode* getLastVisibleNode();
		/**
		* Returns node above this one at the same level, or NULL if this is the first node.
		*/
		TreeViewNode* getPreviousSibling();
		/**
		* Returns all sibling nodes on the same level.
		* NOTE: This node is also included in the list.
		*/
		std::list<TreeViewNode*> getSiblings();
		/**
		* Returns the position of the node relative to the TreeView.
		*/
		Point getPositionRelativeToTreeView();

		/**
		* Returns true if all child nodes are marked visible, false otherwise.
		* NOTE: Even if nodes are visible, they may not be drawn on the scene, for example if their parent is not visible.
		*/
		bool isExpanded();
		/**
		* Returns true if this widget's position is managed by another widget.
		*/
		virtual bool isPositionManaged();
		/**
		* Returns true if this widget's size is managed by another widget.
		*/
		virtual bool isSizeManaged();
		/**
		* Returns true if this node is selected, false otherwise.
		*/
		bool isSelected();

		/**
		* Minimizes the node, hiding all immediate children.
		*/
		void minimize();

		/**
		* Selects the node.
		*/
		void select();
		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);

		/**
		* If the Node is minimized it becomes expanded, and vice versa.
		*/
		void toggle();

		/**
		* Recalculate Client widget_dimensions, relative to Widget's actual widget_dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recursively updates node names and positions.
		*/
		void updateNodes();

	protected:
		TreeViewNode(const Ogre::String& name);
		virtual ~TreeViewNode();

		TreeViewNodeDesc* mDesc;

		Button* mStateButton;
		void createStateButtonIfNotExists();

		int mAutoNameCounter;

		std::list<TreeViewNode*> mNodes;

		bool mExpanded;

		/**
		* Recursively calls children and returns the largest relative x position of any child nodes.
		*/
		float _getFarthestX();

		void _selectNode(TreeViewNode* n);

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Adds a TreeViewNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		* NOTE: Names are auto-generated in the form of <TreeViewName>+<Index>.
		*/
		TreeViewNode* createNode(TreeViewNodeDesc* d, int index = -1);

		/**
		* Uses Brush to draw lines to parent node.
		*/
		void drawLineToParent();
		/**
		* Recursive call telling nodes to draw lines to parent node.
		*/
		void drawLines();

		/**
		* Updates the TreeView Link;
		*/
		virtual void notifyTreeViewParent(TreeView* t);

		/**
		* If enabled, toggles the node.
		*/
		void onDoubleClick(const EventArgs& args);
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* If enabled, navigates to nodes or expands/minimizes currently selected node.
		*/
		void onKeyDown(const EventArgs& args);
		/**
		* Selects the node. (LMB only)
		*/
		void onMouseDown(const EventArgs& args);
		/**
		* Expands/Minimizes node.
		*/
		void onStateButtonClicked(const EventArgs& args);

	private:
	};
}

#endif
