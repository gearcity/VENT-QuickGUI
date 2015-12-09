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

#ifndef QUICKGUILISTPANELITEM_H
#define QUICKGUILISTPANELITEM_H

#include "QuickGUIListItem.h"
#include "QuickGUIText.h"
#include "QuickGUIForwardDeclaredDesc.h"

namespace QuickGUI
{
	// forward declarations
	class Button;
	class CheckBox;
	class ColorPicker;
	class ComboBox;
	class Console;
	class HScrollBar;
	class Image;
	class Label;
	class List;
	class Panel;
	class ProgressBar;
	class PropertyGrid;
	class RadioButton;
	class TabControl;
	class TextArea;
	class TextBox;
	class ToolBar;
	class TreeView;
	class VScrollBar;

	class ButtonDesc;
	class CheckBoxDesc;
	class ColorPickerDesc;
	class ComboBoxDesc;
	class ConsoleDesc;
	class HScrollBarDesc;
	class ImageDesc;
	class LabelDesc;
	class ListDesc;
	class PanelDesc;
	class ProgressBarDesc;
	class PropertyGridDesc;
	class RadioButtonDesc;
	class TextAreaDesc;
	class TextBoxDesc;
	class ToolBarDesc;
	class TreeViewDesc;
	class VScrollBarDesc;

	class _QuickGUIExport ListPanelItemDesc :
		public ListItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		ListPanelItemDesc();
		virtual ~ListPanelItemDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListPanelItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ListPanelItem"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ListPanelItem :
		public ListItem
	{
	public:
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
		* Creates a user defined custom widget.
		*/
		Widget* createCustomWidget(const Ogre::String& className, WidgetDesc* d);

		/**
		* Creates a child Button.
		*/
		Button* createButton(ButtonDesc* d);
		Button* createButton();
		Button* createButton(const Point& position);
		Button* createButton(const Rect& dimensions);
		Button* createButton(const Ogre::UTFString& text, const Rect& dimensions);
		Button* createButton(std::vector<TextSegment> segments, const Rect& dimensions);
		/**
		* Creates a child CheckBox.
		*/
		CheckBox* createCheckBox(CheckBoxDesc* d);
		CheckBox* createCheckBox();
		CheckBox* createCheckBox(const Point& position);
		CheckBox* createCheckBox(const Rect& dimensions);
		/**
		* Creates a child ColorPicker.
		*/
		ColorPicker* createColorPicker(ColorPickerDesc* d);
		ColorPicker* createColorPicker();
		ColorPicker* createColorPicker(const Point& position);
		ColorPicker* createColorPicker(const Rect& dimensions);
		/**
		* Creates a child Console.
		*/
		Console* createConsole(ConsoleDesc* d);
		Console* createConsole();
		Console* createConsole(const Point& position);
		Console* createConsole(const Rect& dimensions);
		/**
		* Creates a child ComboBox.
		*/
		ComboBox* createComboBox(ComboBoxDesc* d);
		ComboBox* createComboBox();
		ComboBox* createComboBox(const Point& position);
		ComboBox* createComboBox(const Rect& dimensions);
		/**
		* Creates a child HScrollBar.
		*/
		HScrollBar* createHScrollBar(HScrollBarDesc* d);
		HScrollBar* createHScrollBar();
		HScrollBar* createHScrollBar(const Point& position);
		HScrollBar* createHScrollBar(const Rect& dimensions);
		/**
		* Creates a child Image.
		*/
		Image* createImage(ImageDesc* d);
		Image* createImage();
		Image* createImage(const Point& position);
		Image* createImage(const Rect& dimensions);
		/**
		* Creates a child Label.
		*/
		Label* createLabel(LabelDesc* d);
		Label* createLabel();
		Label* createLabel(const Point& position);
		Label* createLabel(const Rect& dimensions);
		Label* createLabel(const Ogre::UTFString& text, const Point& position);
		Label* createLabel(std::vector<TextSegment> segments, const Point& position);
		Label* createLabel(const Ogre::UTFString& text, const Rect& dimensions);
		Label* createLabel(std::vector<TextSegment> segments, const Rect& dimensions);
		/**
		* Creates a child List.
		*/
		List* createList(ListDesc* d);
		List* createList();
		List* createList(const Point& position);
		List* createList(const Rect& dimensions);
		/**
		* Creates a child Panel.
		*/
		Panel* createPanel(PanelDesc* d);
		Panel* createPanel();
		Panel* createPanel(const Point& position);
		Panel* createPanel(const Rect& dimensions);
		/**
		* Creates a child ProgressBar.
		*/
		ProgressBar* createProgressBar(ProgressBarDesc* d);
		ProgressBar* createProgressBar();
		ProgressBar* createProgressBar(const Point& position);
		ProgressBar* createProgressBar(const Rect& dimensions);
		/**
		* Creates a child PropertyGrid.
		*/
		PropertyGrid* createPropertyGrid(PropertyGridDesc* d);
		PropertyGrid* createPropertyGrid();
		PropertyGrid* createPropertyGrid(const Point& position);
		PropertyGrid* createPropertyGrid(const Rect& dimensions);
		/**
		* Creates a child RadioButton.
		*/
		RadioButton* createRadioButton(RadioButtonDesc* d);
		RadioButton* createRadioButton();
		RadioButton* createRadioButton(const Point& position);
		RadioButton* createRadioButton(const Rect& dimensions);
		/**
		* Creates a child TabControl.
		*/
		TabControl* createTabControl(TabControlDesc* d);
		TabControl* createTabControl();
		TabControl* createTabControl(const Point& position);
		TabControl* createTabControl(const Rect& dimensions);
		/**
		* Creates a child TextArea.
		*/
		TextArea* createTextArea(TextAreaDesc* d);
		TextArea* createTextArea();
		TextArea* createTextArea(const Point& position);
		TextArea* createTextArea(const Rect& dimensions);
		/**
		* Creates a child TextBox.
		*/
		TextBox* createTextBox(TextBoxDesc* d);
		TextBox* createTextBox();
		TextBox* createTextBox(const Point& position);
		TextBox* createTextBox(const Rect& dimensions);
		/**
		* Creates a child ToolBar.
		*/
		ToolBar* createToolBar(ToolBarDesc* d);
		ToolBar* createToolBar();
		ToolBar* createToolBar(const Point& position);
		ToolBar* createToolBar(const Rect& dimensions);
		/**
		* Creates a child TreeView.
		*/
		TreeView* createTreeView(TreeViewDesc* d);
		TreeView* createTreeView();
		TreeView* createTreeView(const Point& position);
		TreeView* createTreeView(const Rect& dimensions);
		/**
		* Creates a child VScrollBar.
		*/
		VScrollBar* createVScrollBar(VScrollBarDesc* d);
		VScrollBar* createVScrollBar();
		VScrollBar* createVScrollBar(const Point& position);
		VScrollBar* createVScrollBar(const Rect& dimensions);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::setConsumeKeyboardEvents;
		using Widget::setScrollable;
		using Widget::setSerialize;
		using Widget::setTransparencyPicking;
		
		using ContainerWidget::destroyChild;	

	protected:
		ListPanelItem(const Ogre::String& name);
		virtual ~ListPanelItem();

		ListPanelItemDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
