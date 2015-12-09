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

#ifndef QUICKGUIPROPERTYGRIDPROPERTY_H
#define QUICKGUIPROPERTYGRIDPROPERTY_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUITextUser.h"

namespace QuickGUI
{
	// Forward declaration
	class PropertyGrid;

	class _QuickGUIExport PropertyGridItemDesc :
		public ContainerWidgetDesc,
		public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		PropertyGridItemDesc();
		virtual ~PropertyGridItemDesc() {}
	public:

		int propertygriditem_index;
		bool propertygriditem_selected;

		// Properties should store a pointer to the Grid they belong to
		PropertyGrid* propertyGrid;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridItem"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridItem :
		public ContainerWidget,
		public TextUser
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String ICON;
		static const Ogre::String SELECTED;
	public:
		// PropertyGrid has to set positions, toggle visibility to manage ListItems
		friend class PropertyGrid;
		// PropertyGridSection adjusts the width of the property and field
		friend class PropertyGridSection;

		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

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
		virtual float getBranchHeight();
		/**
		* Returns the index of this Property.
		*/
		int getIndex();
		/**
		* Returns node below this one at the same level, or NULL if this is the last node.
		*/
		PropertyGridItem* getNextSibling();
		/**
		* Returns the number of nodes belonging to this node.
		*/
		int getNumberOfSiblings();
		/**
		* Recursive function that returns the last visible item found.
		*/
		virtual PropertyGridItem* getLastVisibleItem();
		/**
		* Returns node above this one at the same level, or NULL if this is the first node.
		*/
		PropertyGridItem* getPreviousSibling();
		/**
		* Returns all sibling nodes on the same level.
		* NOTE: This node is also included in the list.
		*/
		std::list<PropertyGridItem*> getSiblings();
		/**
		* Returns the position of the node relative to the TreeView.
		*/
		Point getPositionRelativeToPropertyGrid();

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
		* Selects the node.
		*/
		void select();

	protected:
		PropertyGridItem(const Ogre::String& name);
		virtual ~PropertyGridItem();

		PropertyGridItemDesc* mDesc;

		// Properties consist of two part: Label and Field.
		// The field is where users can get/set data. (ie TextBox, ComboBox)
		Widget* mFieldWidget;

		// Pixel Buffer to pad the width of the text as used to generate the largest label area.
		float mLabelBuffer;

		/**
		* Recursively calls children and returns the largest relative x position of any child nodes.
		*/
		virtual float _getFarthestX();

		virtual void _selectProperty(PropertyGridItem* pgp);

		/**
		* Updates the PropertyGrid Link;
		*/
		virtual void notifyPropertyGridParent(PropertyGrid* g);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* If enabled, navigates to nodes or expands/minimizes currently selected node.
		*/
		virtual void onKeyDown(const EventArgs& args);
		/**
		* Selects the node. (LMB only)
		*/
		void onMouseDown(const EventArgs& args);
		/**
		* Called when Text changes, allowing Widgets to easily override and use this notification.
		*/
		virtual void onTextChanged();

		/**
		* Notifies Item that parent PropertyGrid has changed item height.
		*/
		virtual void notifyHeightUpdated();
		/**
		* Notifies Item that parent PropertyGrid has changed item width.
		*/
		virtual void notifyWidthUpdated();

		/**
		* Does nothing here.  Section class updates Child Item positions and indices
		*/
		virtual void updateItems();

		virtual void updateFieldSkin();

	private:
	};
}

#endif
