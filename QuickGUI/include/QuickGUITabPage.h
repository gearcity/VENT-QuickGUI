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

#ifndef QUICKGUITABPAGE_H
#define QUICKGUITABPAGE_H

#include "QuickGUIComponentWidget.h"
#include "QuickGUIPanel.h"

namespace Ogre
{
	// forward declaration
	class Font;
}

namespace QuickGUI
{
	// forward declaration
	class TabControl;
	class Tab;

	class _QuickGUIExport TabPage :
		public Panel
	{
	public:
		// Skin Constants
		static const Ogre::String TAB;
		static const Ogre::String TAB_DISABLED;
		static const Ogre::String TAB_OVER;
		static const Ogre::String TAB_SELECTED;
		static const Ogre::String PAGE;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// TabControl sets the size of the TabPage, and position of the Tab
		friend class TabControl;
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		virtual void addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function);

		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, unsigned int queryFilter = -1, bool ignoreDisabled = true);

		/**
		* Returns the Border underneath the point p.
		* NOTE: Point is assumed to be relative to the Window.
		* NOTE: Exception is thrown if point is not over a border.
		*/
		virtual BorderSide getBorderSide(Point p);
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the tabpage_index of the ListItem in its owner List.
		*/
		int getIndex();
		/**
		* Returns the size of the Page's client area.
		*/
		Size getPageClientArea();

		/**
		* Returns true if this widget's position is managed by another widget.
		*/
		virtual bool isPositionManaged();
		/**
		* Returns true if this widget's size is managed by another widget.
		*/
		virtual bool isSizeManaged();
		/**
		* Returns true if the tab page has been selected, false otherwise.
		*/
		bool isSelected();

		/**
		* Returns true if the point is over a border, false otherwise.
		* NOTE: Point is assumed to be relative to the Window.
		*/
		virtual bool overBorderSide(Point p);

		/**
		* Removes a child widget from this container widget.
		*/
		virtual void removeChild(Widget* w);

		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Enabled Widgets receive mouse and keyboard events via injections to the GUIManager.
		* Disabled Widgets can only receive these events if they are manually fired.
		*/
		virtual void setEnabled(bool enabled);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets all characters of the text to the specified font.
		*/
		void setTabFont(const Ogre::String& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setTabFont(const Ogre::String& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setTabFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setTabFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setTabFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the height of this tab.
		*/
		void setTabHeight(float height);
		/**
		* Sets the text for this object.
		*/
		void setTabText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setTabText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setTabText(Ogre::UTFString s);
		/**
		* Sets whether the widget will receive mouse over events simply by having the mouse over
		* its texture widget_dimensions, or only when the cursor is over non transparent parts.
		*/
		virtual void setTransparencyPicking(bool transparencyPicking);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		TabPage(const Ogre::String& name);
		virtual ~TabPage();

		Tab* mTab;
		Panel* mPage;

		bool mSelected;

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		TabPageDesc* mDesc;

		// Number of Pixels Tab overlaps Page (Tab on top of Page)
		float mTabOverlap;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onMouseButtonDownOnTab(const EventArgs& args);
		void onMouseEnterTab(const EventArgs& args);
		void onMouseLeaveTab(const EventArgs& args);

		/**
		* Called internally by the parent TabControl.
		*/
		void deselect();
		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();
		/**
		* Called internally by the parent TabControl.
		*/
		void select();
		/**
		* Sets the listitem_index of the ListItem
		*/
		void setIndex(unsigned int index);

		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);

		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();

	private:
	};
}

#endif
