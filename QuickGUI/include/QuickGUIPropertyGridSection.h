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

#ifndef QUICKGUIPROPERTYGRIDSECTION_H
#define QUICKGUIPROPERTYGRIDSECTION_H

#include "QuickGUIPropertyGridItem.h"

namespace QuickGUI
{
	// forward declarations
	class Button;
	class PropertyGridBoolProperty;
	class PropertyGridComboBoxProperty;
	class PropertyGridTextProperty;

	class PropertyGridBoolPropertyDesc;
	class PropertyGridComboBoxPropertyDesc;
	class PropertyGridTextPropertyDesc;

	class _QuickGUIExport PropertyGridSectionDesc :
		public PropertyGridItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		PropertyGridSectionDesc();
		virtual ~PropertyGridSectionDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridSectionDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridSection"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridSection :
		public PropertyGridItem
	{
	public:
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// PropertyGrid sets Property width and field width
		friend class PropertyGrid;
		// PropertyGrid item accesses mLabelWidth
		friend class PropertyGridItem;

		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Creates a Boolean Property for this Section at the given index.  A ComboBox with values "True" and "False" are used
		* to get and set a property value.
		* NOTE: if the index is not valid, the Property is added to the end of the Section.
		*/
		PropertyGridBoolProperty* createBoolProperty(int index = -1);
		PropertyGridBoolProperty* createBoolProperty(const Ogre::UTFString& text, int index = -1);
		PropertyGridBoolProperty* createBoolProperty(std::vector<TextSegment> segments, int index = -1);
		/**
		* Creates a ComboBox Property for this Section at the given index.  A ComboBox is used to get and set a property value.
		* NOTE: if the index is not valid, the Property is added to the end of the Section.
		*/
		PropertyGridComboBoxProperty* createComboBoxProperty(int index = -1);
		PropertyGridComboBoxProperty* createComboBoxProperty(const Ogre::UTFString& text, int index = -1);
		PropertyGridComboBoxProperty* createComboBoxProperty(std::vector<TextSegment> segments, int index = -1);
		/**
		* Creates a Text Property for this Section at the given index.  A TextBox is used to get and set a property value.
		* NOTE: if the index is not valid, the Property is added to the end of the Section.
		*/
		PropertyGridTextProperty* createTextProperty(int index = -1);
		PropertyGridTextProperty* createTextProperty(const Ogre::UTFString& text, int index = -1);
		PropertyGridTextProperty* createTextProperty(std::vector<TextSegment> segments, int index = -1);
		/**
		* Creates a child Section, at the given index.
		* NOTE: if the index is not valid, the Property is added to the end of the Section.
		*/
		PropertyGridSection* createSection(int index = -1);
		PropertyGridSection* createSection(const Ogre::UTFString& text, int index = -1);
		PropertyGridSection* createSection(std::vector<TextSegment> segments, int index = -1);

		/**
		* Expands the node, showing all immediate children. 
		* NOTE: Any parents that aren't visible will be shown.
		*/
		void expand();

		/**
		* Returns the total height of this branch.  Only visible Nodes are used to
		* calculate the height.
		*/
		virtual float getBranchHeight();
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the immediate items belonging to this section.
		*/
		std::list<PropertyGridItem*> getItems();
		/**
		* Recursive function that returns the last visible item found.
		*/
		virtual PropertyGridItem* getLastVisibleItem();
		/**
		* Returns the number of properties in this section.
		*/
		int getNumberOfProperties();

		/**
		* Minimizes the node, hiding all immediate children.
		*/
		void minimize();

		/**
		* If the Node is minimized it becomes expanded, and vice versa.
		*/
		void toggle();

		/**
		* Recalculate Client widget_dimensions, relative to Widget's actual widget_dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		PropertyGridSection(const Ogre::String& name);
		virtual ~PropertyGridSection();

		PropertyGridSectionDesc* mDesc;

		std::list<PropertyGridItem*> mItems;

		bool mExpanded;
		Button* mStateButton;
		void createStateButtonIfNotExists();

		/**
		* Recursively calls children and returns the largest relative x position of any child nodes.
		*/
		virtual float _getFarthestX();

		virtual void _selectProperty(PropertyGridItem* pgp);

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Adds an Item to the Section at the index given.
		* NOTE: if the index is not valid, the Item is added to the end.
		*/
		PropertyGridItem* createItem(PropertyGridItemDesc* d, int index = -1);

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
		* Expands/Minimizes node.
		*/
		void onStateButtonClicked(const EventArgs& args);

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

		void updatePropertySkin();

		float mLabelWidth;
		// If width given is larger than label width, width is updated
		void updateLabelWidth(float width);

	private:
	};
}

#endif
