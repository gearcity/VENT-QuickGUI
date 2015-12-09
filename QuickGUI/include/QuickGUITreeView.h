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

#ifndef QUICKGUITREEVIEW_H
#define QUICKGUITREEVIEW_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIText.h"

#include <list>

namespace QuickGUI
{
	// forward declarations
	class TreeViewNode;
	class TreeViewCheckBoxNode;
	class TreeViewRadioButtonNode;

	class TreeViewNodeDesc;

	class _QuickGUIExport TreeViewDesc :
		public ContainerWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		TreeViewDesc();
		virtual ~TreeViewDesc() {}
	public:

		bool treeview_doubleClickToToggle;
		bool treeview_keyNavigation;
		float treeview_nodeHeight;
		Ogre::String treeview_userHandlers[TREEVIEW_EVENT_COUNT];

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TreeViewDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TreeView"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TreeView :
		public ContainerWidget
	{
	public:
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String EXPAND;
		static const Ogre::String MINIMIZE;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Adds a user-defined event handler to this widget.  The difference between this function and the addWidgetEventHandler
		* is that the handlerName gets serialized with the Widget. This means you can defined handlers outside the creation of the
		* GUI Layout, and when the sheet is loaded from disk, all of the handlers will work properly.
		* NOTE: Only 1 user defined Event Handler can be assigned to an Event. Old values will be overwritten.
		* NOTE: A handler of "" essentially removes the handler.
		* NOTE: If no eventHandler exists with the name provided, nothing will happen.
		*/
		void addUserDefinedTreeViewEventHandler(TreeViewEvent EVENT, const Ogre::String& handlerName);
		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WIDGET_EVENT_SIZE_CHANGED, WIDGET_EVENT_MOUSE_WHEEL, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WIDGET_EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addTreeViewEventHandler(TreeViewEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addTreeViewEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addTreeViewEventHandler(TreeViewEvent EVENT, EventHandlerSlot* function);

		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedTreeViewEventHandler(TreeViewEvent EVENT);
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
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireTreeViewEvent(TreeViewEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns true if double clicking on a node will toggle its state, false otherwise.
		*/
		bool getDoubleClickToToggle();
		/**
		* Returns the name of the skinType used to draw nodes in expanded form.
		*/
		Ogre::String getExpandSkinTypeName();
		/**
		* Returns true if up/down keys will select previous/next node and left/right expands/minimizes node, false otherwise.
		*/
		bool getKeyNavigation();
		/**
		* Returns the name of the skinType used to draw nodes in minimized form.
		*/
		Ogre::String getMinimizeSkinTypeName();
		/**
		* Gets the height of every created or existing Node in this TreeView.
		*/
		float getNodeHeight();
		/**
		* Returns the number of root nodes in this TreeView.
		*/
		int getNumberOfRootNodes();
		/**
		* Returns a list of root nodes in this tree.
		*/
		std::list<TreeViewNode*> getRootNodes();
		/**
		* Returns the currently selected Node, or NULL if no node is selected.
		*/
		TreeViewNode* getSelectedNode();

		/**
		* Removes all Event Handlers registered by the given object.
		*/
		virtual void removeEventHandlers(void* obj);
		
		/**
		* Sets whether double clicking on a node will toggle is state.
		*/
		void setDoubleClickToToggle(bool enable);
		/**
		* If set, up/down keys will select previous/next node and left/right expands/minimizes node.
		*/
		void setKeyNavigation(bool enable);
		/**
		* Selects a Node in the tree.
		*/
		void selectNode(TreeViewNode* n);
		/**
		* Sets the height of every created or existing Node in this TreeView.
		*/
		void setNodeHeight(float nodeHeightInPixels);

		/**
		* Recursively updates node names and positions.
		*/
		void updateNodes();
		/**
		* Recalculate Virtual dimensions, the minimum size required to encapsulate the client area and all Child widgets.
		*/
		virtual void updateVirtualDimensions();

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::drag;
		using Widget::resize;
		using Widget::setConsumeKeyboardEvents;
		using Widget::setDimensions;
		using Widget::setDragable;
		using Widget::setHeight;
		using Widget::setHorizontalAnchor;
		using Widget::setMaxSize;
		using Widget::setMinSize;
		using Widget::setPosition;
		using Widget::setPositionRelativeToParentClientDimensions;
		using Widget::setQueryFlags;
		using Widget::setResizeFromAllSides;
		using Widget::setResizeFromBottom;
		using Widget::setResizeFromLeft;
		using Widget::setResizeFromRight;
		using Widget::setResizeFromTop;
		using Widget::setScrollable;
		using Widget::setSerialize;
		using Widget::setSize;
		using Widget::setTransparencyPicking;
		using Widget::setVerticalAnchor;
		using Widget::setVisible;
		using Widget::setWidth;

		using ComponentWidget::setClipComponentsToDimensions;

		using ContainerWidget::setClipChildrenToDimensions;		

	protected:
		TreeView(const Ogre::String& name);
		virtual ~TreeView();

		TreeViewDesc* mDesc;

		int mAutoNameCounter;

		float mTotalHeight;

		std::list<TreeViewNode*> mNodes;

		TreeViewNode* mSelectedNode;

		// TreeViewRadioButtonNode specific event handlers
		std::vector<EventHandlerSlot*> mTreeViewEventHandlers[TREEVIEW_EVENT_COUNT];

		/**
		* Recursively calls children and returns the largest relative x position of any child nodes.
		*/
		float _getFarthestX();

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Adds a TreeViewNode to the List at the index given.
		* NOTE: if the index is not valid, the ListItem is added to the end of the list.
		*/
		TreeViewNode* createNode(TreeViewNodeDesc* d, int index = -1);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* Clears selection.
		*/
		void onMouseDown(const EventArgs& args);

	private:
	};
}

#endif
