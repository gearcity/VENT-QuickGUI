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

#include "QuickGUIListPanelItem.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUITabControl.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIDescManager.h"
// Widgets;
#include "QuickGUIButton.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIColorPicker.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIConsole.h"
#include "QuickGUIForwardDeclaredDesc.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIImage.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIPanel.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUIRadioButton.h"
#include "QuickGUITextArea.h"
#include "QuickGUITextBox.h"
#include "QuickGUIToolBar.h"
#include "QuickGUITreeView.h"
#include "QuickGUIVScrollBar.h"

namespace QuickGUI
{
	void ListPanelItem::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ListPanelItem");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ListPanelItem",d);
	}

	ListPanelItemDesc::ListPanelItemDesc() :
		ListItemDesc()
	{
		resetToDefault();
	}

	void ListPanelItemDesc::resetToDefault()
	{
		ListItemDesc::resetToDefault();
	}

	void ListPanelItemDesc::serialize(SerialBase* b)
	{
		ListItemDesc::serialize(b);
	}

	ListPanelItem::ListPanelItem(const Ogre::String& name) :
		ListItem(name)
	{
	}

	ListPanelItem::~ListPanelItem()
	{
	}

	void ListPanelItem::_initialize(WidgetDesc* d)
	{
		ListItem::_initialize(d);

		mDesc = dynamic_cast<ListPanelItemDesc*>(mWidgetDesc);

		ListPanelItemDesc* lpid = dynamic_cast<ListPanelItemDesc*>(d);

		setSkinType(lpid->widget_skinTypeName);
	}

	Widget* ListPanelItem::createCustomWidget(const Ogre::String& className, WidgetDesc* d)
	{
		if((className == "TitleBar") || (className == "Window") || (className == "Sheet"))
			throw Exception(Exception::ERR_UNSUPPORTED_WIDGET,"This widget does not support creation of widget's of class \"" + className + "\".","ListPanelItem::createCustomWidget");

		Widget* newCustomWidget = _createWidget(d);

		addChild(newCustomWidget);

		return newCustomWidget;
	}

	Button* ListPanelItem::createButton(ButtonDesc* d)
	{
		Button* newButton = dynamic_cast<Button*>(_createWidget(d));

		addChild(newButton);

		return newButton;
	}

	Button* ListPanelItem::createButton()
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		return createButton(bd);
	}

	Button* ListPanelItem::createButton(const Point& position)
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		bd->widget_dimensions.position = position;

		return createButton(bd);
	}

	Button* ListPanelItem::createButton(const Rect& dimensions)
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		bd->widget_dimensions = dimensions;

		return createButton(bd);
	}

	Button* ListPanelItem::createButton(const Ogre::UTFString& text, const Rect& dimensions)
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		bd->textDesc.segments.push_back(TextSegment(text));
		bd->widget_dimensions = dimensions;

		return createButton(bd);
	}

	Button* ListPanelItem::createButton(std::vector<TextSegment> segments, const Rect& dimensions)
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		bd->textDesc.segments = segments;
		bd->widget_dimensions = dimensions;

		return createButton(bd);
	}

	CheckBox* ListPanelItem::createCheckBox(CheckBoxDesc* d)
	{
		CheckBox* newCheckBox = dynamic_cast<CheckBox*>(_createWidget(d));

		addChild(newCheckBox);

		return newCheckBox;
	}

	CheckBox* ListPanelItem::createCheckBox()
	{
		CheckBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultCheckBoxDesc();
		cd->resetToDefault();

		return createCheckBox(cd);
	}

	CheckBox* ListPanelItem::createCheckBox(const Point& position)
	{
		CheckBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultCheckBoxDesc();
		cd->resetToDefault();

		cd->widget_dimensions.position = position;

		return createCheckBox(cd);
	}

	CheckBox* ListPanelItem::createCheckBox(const Rect& dimensions)
	{
		CheckBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultCheckBoxDesc();
		cd->resetToDefault();

		cd->widget_dimensions = dimensions;

		return createCheckBox(cd);
	}

	ColorPicker* ListPanelItem::createColorPicker(ColorPickerDesc* d)
	{
		ColorPicker* newColorPicker = dynamic_cast<ColorPicker*>(_createWidget(d));

		addChild(newColorPicker);

		return newColorPicker;
	}

	ColorPicker* ListPanelItem::createColorPicker()
	{
		ColorPickerDesc* cd = DescManager::getSingletonPtr()->getDefaultColorPickerDesc();
		cd->resetToDefault();

		return createColorPicker(cd);
	}

	ColorPicker* ListPanelItem::createColorPicker(const Point& position)
	{
		ColorPickerDesc* cd = DescManager::getSingletonPtr()->getDefaultColorPickerDesc();
		cd->resetToDefault();

		cd->widget_dimensions.position = position;

		return createColorPicker(cd);
	}

	ColorPicker* ListPanelItem::createColorPicker(const Rect& dimensions)
	{
		ColorPickerDesc* cd = DescManager::getSingletonPtr()->getDefaultColorPickerDesc();
		cd->resetToDefault();

		cd->widget_dimensions = dimensions;

		return createColorPicker(cd);
	}

	ComboBox* ListPanelItem::createComboBox(ComboBoxDesc* d)
	{
		ComboBox* newComboBox = dynamic_cast<ComboBox*>(_createWidget(d));

		addChild(newComboBox);

		return newComboBox;
	}

	ComboBox* ListPanelItem::createComboBox()
	{
		ComboBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cd->resetToDefault();

		return createComboBox(cd);
	}

	ComboBox* ListPanelItem::createComboBox(const Point& position)
	{
		ComboBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cd->resetToDefault();

		cd->widget_dimensions.position = position;

		return createComboBox(cd);
	}

	ComboBox* ListPanelItem::createComboBox(const Rect& dimensions)
	{
		ComboBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cd->resetToDefault();

		cd->widget_dimensions = dimensions;

		return createComboBox(cd);
	}

	Console* ListPanelItem::createConsole(ConsoleDesc* d)
	{
		Console* newConsole = dynamic_cast<Console*>(_createWidget(d));

		addChild(newConsole);

		return newConsole;
	}

	Console* ListPanelItem::createConsole()
	{
		ConsoleDesc* cd = DescManager::getSingletonPtr()->getDefaultConsoleDesc();
		cd->resetToDefault();

		return createConsole(cd);
	}

	Console* ListPanelItem::createConsole(const Point& position)
	{
		ConsoleDesc* cd = DescManager::getSingletonPtr()->getDefaultConsoleDesc();
		cd->resetToDefault();

		cd->widget_dimensions.position = position;

		return createConsole(cd);
	}

	Console* ListPanelItem::createConsole(const Rect& dimensions)
	{
		ConsoleDesc* cd = DescManager::getSingletonPtr()->getDefaultConsoleDesc();
		cd->resetToDefault();

		cd->widget_dimensions = dimensions;

		return createConsole(cd);
	}

	HScrollBar* ListPanelItem::createHScrollBar(HScrollBarDesc* d)
	{
		HScrollBar* newHScrollBar = dynamic_cast<HScrollBar*>(_createWidget(d));

		addChild(newHScrollBar);

		return newHScrollBar;
	}

	HScrollBar* ListPanelItem::createHScrollBar()
	{
		HScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultHScrollBarDesc();
		sd->resetToDefault();

		return createHScrollBar(sd);
	}

	HScrollBar* ListPanelItem::createHScrollBar(const Point& position)
	{
		HScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultHScrollBarDesc();
		sd->resetToDefault();

		sd->widget_dimensions.position = position;

		return createHScrollBar(sd);
	}

	HScrollBar* ListPanelItem::createHScrollBar(const Rect& dimensions)
	{
		HScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultHScrollBarDesc();
		sd->resetToDefault();

		sd->widget_dimensions = dimensions;

		return createHScrollBar(sd);
	}

	Image* ListPanelItem::createImage(ImageDesc* d)
	{
		Image* newImage = dynamic_cast<Image*>(_createWidget(d));

		addChild(newImage);

		return newImage;
	}

	Image* ListPanelItem::createImage()
	{
		ImageDesc* id = DescManager::getSingletonPtr()->getDefaultImageDesc();
		id->resetToDefault();

		return createImage(id);
	}

	Image* ListPanelItem::createImage(const Point& position)
	{
		ImageDesc* id = DescManager::getSingletonPtr()->getDefaultImageDesc();
		id->resetToDefault();

		id->widget_dimensions.position = position;

		return createImage(id);
	}

	Image* ListPanelItem::createImage(const Rect& dimensions)
	{
		ImageDesc* id = DescManager::getSingletonPtr()->getDefaultImageDesc();
		id->resetToDefault();

		id->widget_dimensions = dimensions;

		return createImage(id);
	}

	Label* ListPanelItem::createLabel(LabelDesc* d)
	{
		Label* newLabel = dynamic_cast<Label*>(_createWidget(d));

		addChild(newLabel);

		return newLabel;
	}

	Label* ListPanelItem::createLabel()
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment("Label"));

		return createLabel(ld);
	}

	Label* ListPanelItem::createLabel(const Point& position)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment("Label"));
		ld->widget_dimensions.position = position;

		return createLabel(ld);
	}

	Label* ListPanelItem::createLabel(const Rect& dimensions)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment("Label"));
		ld->widget_dimensions = dimensions;

		return createLabel(ld);
	}

	Label* ListPanelItem::createLabel(const Ogre::UTFString& text, const Point& position)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment(text));
		ld->widget_dimensions.position = position;

		return createLabel(ld);
	}

	Label* ListPanelItem::createLabel(std::vector<TextSegment> segments, const Point& position)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments = segments;
		ld->widget_dimensions.position = position;

		return createLabel(ld);
	}

	Label* ListPanelItem::createLabel(const Ogre::UTFString& text, const Rect& dimensions)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment(text));
		ld->widget_dimensions = dimensions;

		return createLabel(ld);
	}

	Label* ListPanelItem::createLabel(std::vector<TextSegment> segments, const Rect& dimensions)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments = segments;
		ld->widget_dimensions = dimensions;

		return createLabel(ld);
	}

	List* ListPanelItem::createList(ListDesc* d)
	{
		List* newList = dynamic_cast<List*>(_createWidget(d));

		addChild(newList);

		return newList;
	}

	List* ListPanelItem::createList()
	{
		ListDesc* ld = DescManager::getSingletonPtr()->getDefaultListDesc();
		ld->resetToDefault();

		return createList(ld);
	}

	List* ListPanelItem::createList(const Point& position)
	{
		ListDesc* ld = DescManager::getSingletonPtr()->getDefaultListDesc();
		ld->resetToDefault();

		ld->widget_dimensions.position = position;

		return createList(ld);
	}

	List* ListPanelItem::createList(const Rect& dimensions)
	{
		ListDesc* ld = DescManager::getSingletonPtr()->getDefaultListDesc();
		ld->resetToDefault();

		ld->widget_dimensions = dimensions;

		return createList(ld);
	}

	Panel* ListPanelItem::createPanel(PanelDesc* d)
	{
		Panel* newPanel = dynamic_cast<Panel*>(_createWidget(d));
		
		addChild(newPanel);

		return newPanel;
	}

	Panel* ListPanelItem::createPanel()
	{
		PanelDesc* pd = DescManager::getSingletonPtr()->getDefaultPanelDesc();
		pd->resetToDefault();

		return createPanel(pd);
	}

	Panel* ListPanelItem::createPanel(const Point& position)
	{
		PanelDesc* pd = DescManager::getSingletonPtr()->getDefaultPanelDesc();
		pd->resetToDefault();

		pd->widget_dimensions.position = position;

		return createPanel(pd);
	}

	Panel* ListPanelItem::createPanel(const Rect& dimensions)
	{
		PanelDesc* pd = DescManager::getSingletonPtr()->getDefaultPanelDesc();
		pd->resetToDefault();

		pd->widget_dimensions = dimensions;

		return createPanel(pd);
	}

	ProgressBar* ListPanelItem::createProgressBar(ProgressBarDesc* d)
	{
		ProgressBar* newProgressBar = dynamic_cast<ProgressBar*>(_createWidget(d));

		addChild(newProgressBar);

		return newProgressBar;
	}

	ProgressBar* ListPanelItem::createProgressBar()
	{
		ProgressBarDesc* pd = DescManager::getSingletonPtr()->getDefaultProgressBarDesc();
		pd->resetToDefault();

		return createProgressBar(pd);
	}

	ProgressBar* ListPanelItem::createProgressBar(const Point& position)
	{
		ProgressBarDesc* pd = DescManager::getSingletonPtr()->getDefaultProgressBarDesc();
		pd->resetToDefault();

		pd->widget_dimensions.position = position;

		return createProgressBar(pd);
	}

	ProgressBar* ListPanelItem::createProgressBar(const Rect& dimensions)
	{
		ProgressBarDesc* pd = DescManager::getSingletonPtr()->getDefaultProgressBarDesc();
		pd->resetToDefault();

		pd->widget_dimensions = dimensions;

		return createProgressBar(pd);
	}

	PropertyGrid* ListPanelItem::createPropertyGrid(PropertyGridDesc* d)
	{
		PropertyGrid* newPropertyGrid = dynamic_cast<PropertyGrid*>(_createWidget(d));

		addChild(newPropertyGrid);

		return newPropertyGrid;
	}

	PropertyGrid* ListPanelItem::createPropertyGrid()
	{
		PropertyGridDesc* pd = DescManager::getSingletonPtr()->getDefaultPropertyGridDesc();
		pd->resetToDefault();

		return createPropertyGrid(pd);
	}

	PropertyGrid* ListPanelItem::createPropertyGrid(const Point& position)
	{
		PropertyGridDesc* pd = DescManager::getSingletonPtr()->getDefaultPropertyGridDesc();
		pd->resetToDefault();

		pd->widget_dimensions.position = position;

		return createPropertyGrid(pd);
	}

	PropertyGrid* ListPanelItem::createPropertyGrid(const Rect& dimensions)
	{
		PropertyGridDesc* pd = DescManager::getSingletonPtr()->getDefaultPropertyGridDesc();
		pd->resetToDefault();

		pd->widget_dimensions = dimensions;

		return createPropertyGrid(pd);
	}

	RadioButton* ListPanelItem::createRadioButton(RadioButtonDesc* d)
	{
		RadioButton* newRadioButton = dynamic_cast<RadioButton*>(_createWidget(d));

		addChild(newRadioButton);

		return newRadioButton;
	}

	RadioButton* ListPanelItem::createRadioButton()
	{
		RadioButtonDesc* rd = DescManager::getSingletonPtr()->getDefaultRadioButtonDesc();
		rd->resetToDefault();

		return createRadioButton(rd);
	}

	RadioButton* ListPanelItem::createRadioButton(const Point& position)
	{
		RadioButtonDesc* rd = DescManager::getSingletonPtr()->getDefaultRadioButtonDesc();
		rd->resetToDefault();

		rd->widget_dimensions.position = position;

		return createRadioButton(rd);
	}

	RadioButton* ListPanelItem::createRadioButton(const Rect& dimensions)
	{
		RadioButtonDesc* rd = DescManager::getSingletonPtr()->getDefaultRadioButtonDesc();
		rd->resetToDefault();

		rd->widget_dimensions = dimensions;

		return createRadioButton(rd);
	}

	TabControl* ListPanelItem::createTabControl(TabControlDesc* d)
	{
		TabControl* newTabControl = dynamic_cast<TabControl*>(_createWidget(d));

		addChild(newTabControl);

		return newTabControl;
	}

	TabControl* ListPanelItem::createTabControl()
	{
		TabControlDesc* td = DescManager::getSingletonPtr()->getDefaultTabControlDesc();
		td->resetToDefault();

		return createTabControl(td);
	}

	TabControl* ListPanelItem::createTabControl(const Point& position)
	{
		TabControlDesc* td = DescManager::getSingletonPtr()->getDefaultTabControlDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createTabControl(td);
	}

	TabControl* ListPanelItem::createTabControl(const Rect& dimensions)
	{
		TabControlDesc* td = DescManager::getSingletonPtr()->getDefaultTabControlDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createTabControl(td);
	}

	TextArea* ListPanelItem::createTextArea(TextAreaDesc* d)
	{
		TextArea* newTextArea = dynamic_cast<TextArea*>(_createWidget(d));

		addChild(newTextArea);

		return newTextArea;
	}

	TextArea* ListPanelItem::createTextArea()
	{
		TextAreaDesc* td = DescManager::getSingletonPtr()->getDefaultTextAreaDesc();
		td->resetToDefault();

		return createTextArea(td);
	}

	TextArea* ListPanelItem::createTextArea(const Point& position)
	{
		TextAreaDesc* td = DescManager::getSingletonPtr()->getDefaultTextAreaDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createTextArea(td);
	}

	TextArea* ListPanelItem::createTextArea(const Rect& dimensions)
	{
		TextAreaDesc* td = DescManager::getSingletonPtr()->getDefaultTextAreaDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createTextArea(td);
	}

	TextBox* ListPanelItem::createTextBox(TextBoxDesc* d)
	{
		TextBox* newTextBox = dynamic_cast<TextBox*>(_createWidget(d));

		addChild(newTextBox);

		return newTextBox;
	}

	TextBox* ListPanelItem::createTextBox()
	{
		TextBoxDesc* td = DescManager::getSingletonPtr()->getDefaultTextBoxDesc();
		td->resetToDefault();

		return createTextBox(td);
	}

	TextBox* ListPanelItem::createTextBox(const Point& position)
	{
		TextBoxDesc* td = DescManager::getSingletonPtr()->getDefaultTextBoxDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createTextBox(td);
	}

	TextBox* ListPanelItem::createTextBox(const Rect& dimensions)
	{
		TextBoxDesc* td = DescManager::getSingletonPtr()->getDefaultTextBoxDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createTextBox(td);
	}

	ToolBar* ListPanelItem::createToolBar(ToolBarDesc* d)
	{
		ToolBar* newToolBar = dynamic_cast<ToolBar*>(_createWidget(d));

		addChild(newToolBar);

		return newToolBar;
	}

	ToolBar* ListPanelItem::createToolBar()
	{
		ToolBarDesc* td = DescManager::getSingletonPtr()->getDefaultToolBarDesc();
		td->resetToDefault();

		return createToolBar(td);
	}

	ToolBar* ListPanelItem::createToolBar(const Point& position)
	{
		ToolBarDesc* td = DescManager::getSingletonPtr()->getDefaultToolBarDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createToolBar(td);
	}

	ToolBar* ListPanelItem::createToolBar(const Rect& dimensions)
	{
		ToolBarDesc* td = DescManager::getSingletonPtr()->getDefaultToolBarDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createToolBar(td);
	}

	TreeView* ListPanelItem::createTreeView(TreeViewDesc* d)
	{
		TreeView* newTreeView = dynamic_cast<TreeView*>(_createWidget(d));

		addChild(newTreeView);

		return newTreeView;
	}

	TreeView* ListPanelItem::createTreeView()
	{
		TreeViewDesc* td = DescManager::getSingletonPtr()->getDefaultTreeViewDesc();
		td->resetToDefault();

		return createTreeView(td);
	}

	TreeView* ListPanelItem::createTreeView(const Point& position)
	{
		TreeViewDesc* td = DescManager::getSingletonPtr()->getDefaultTreeViewDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createTreeView(td);
	}

	TreeView* ListPanelItem::createTreeView(const Rect& dimensions)
	{
		TreeViewDesc* td = DescManager::getSingletonPtr()->getDefaultTreeViewDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createTreeView(td);
	}

	VScrollBar* ListPanelItem::createVScrollBar(VScrollBarDesc* d)
	{
		VScrollBar* newVScrollBar = dynamic_cast<VScrollBar*>(_createWidget(d));

		addChild(newVScrollBar);

		return newVScrollBar;
	}

	VScrollBar* ListPanelItem::createVScrollBar()
	{
		VScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultVScrollBarDesc();
		sd->resetToDefault();

		return createVScrollBar(sd);
	}

	VScrollBar* ListPanelItem::createVScrollBar(const Point& position)
	{
		VScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultVScrollBarDesc();
		sd->resetToDefault();

		sd->widget_dimensions.position = position;

		return createVScrollBar(sd);
	}

	VScrollBar* ListPanelItem::createVScrollBar(const Rect& dimensions)
	{
		VScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultVScrollBarDesc();
		sd->resetToDefault();

		sd->widget_dimensions = dimensions;

		return createVScrollBar(sd);
	}

	Ogre::String ListPanelItem::getClass()
	{
		return "ListPanelItem";
	}

	void ListPanelItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}
}
