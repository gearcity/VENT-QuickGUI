#include "QuickGUIPanel.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITabControl.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
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
	const Ogre::String Panel::BACKGROUND = "background";

	void Panel::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Panel");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Panel",d);
	}

	Panel::Panel(const Ogre::String& name) :
		ContainerWidget(name)
	{
	}

	Panel::~Panel()
	{
	}

	void Panel::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		mPanelDesc = dynamic_cast<PanelDesc*>(mWidgetDesc);

		if(d->getWidgetClass() == "Panel")
			setSkinType(d->widget_skinTypeName);
	}

	Widget* Panel::createCustomWidget(const Ogre::String& className, WidgetDesc* d)
	{
		if((className == "TitleBar") || (className == "Window") || (className == "Sheet"))
			throw Exception(Exception::ERR_UNSUPPORTED_WIDGET,"This widget does not support creation of widget's of class \"" + className + "\".","Panel::createCustomWidget");

		Widget* newCustomWidget = _createWidget(d);

		addChild(newCustomWidget);

		return newCustomWidget;
	}

	Button* Panel::createButton(ButtonDesc* d)
	{
		Button* newButton = dynamic_cast<Button*>(_createWidget(d));

		addChild(newButton);

		return newButton;
	}

	Button* Panel::createButton()
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		return createButton(bd);
	}

	Button* Panel::createButton(const Point& position)
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		bd->widget_dimensions.position = position;

		return createButton(bd);
	}

	Button* Panel::createButton(const Rect& dimensions)
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		bd->widget_dimensions = dimensions;

		return createButton(bd);
	}

	Button* Panel::createButton(const Ogre::UTFString& text, const Rect& dimensions)
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		bd->textDesc.segments.push_back(TextSegment(text));
		bd->widget_dimensions = dimensions;

		return createButton(bd);
	}

	Button* Panel::createButton(std::vector<TextSegment> segments, const Rect& dimensions)
	{
		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();

		bd->textDesc.segments = segments;
		bd->widget_dimensions = dimensions;

		return createButton(bd);
	}

	CheckBox* Panel::createCheckBox(CheckBoxDesc* d)
	{
		CheckBox* newCheckBox = dynamic_cast<CheckBox*>(_createWidget(d));

		addChild(newCheckBox);

		return newCheckBox;
	}

	CheckBox* Panel::createCheckBox()
	{
		CheckBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultCheckBoxDesc();
		cd->resetToDefault();

		return createCheckBox(cd);
	}

	CheckBox* Panel::createCheckBox(const Point& position)
	{
		CheckBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultCheckBoxDesc();
		cd->resetToDefault();

		cd->widget_dimensions.position = position;

		return createCheckBox(cd);
	}

	CheckBox* Panel::createCheckBox(const Rect& dimensions)
	{
		CheckBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultCheckBoxDesc();
		cd->resetToDefault();

		cd->widget_dimensions = dimensions;

		return createCheckBox(cd);
	}

	ColorPicker* Panel::createColorPicker(ColorPickerDesc* d)
	{
		ColorPicker* newColorPicker = dynamic_cast<ColorPicker*>(_createWidget(d));

		addChild(newColorPicker);

		return newColorPicker;
	}

	ColorPicker* Panel::createColorPicker()
	{
		ColorPickerDesc* cd = DescManager::getSingletonPtr()->getDefaultColorPickerDesc();
		cd->resetToDefault();

		return createColorPicker(cd);
	}

	ColorPicker* Panel::createColorPicker(const Point& position)
	{
		ColorPickerDesc* cd = DescManager::getSingletonPtr()->getDefaultColorPickerDesc();
		cd->resetToDefault();

		cd->widget_dimensions.position = position;

		return createColorPicker(cd);
	}

	ColorPicker* Panel::createColorPicker(const Rect& dimensions)
	{
		ColorPickerDesc* cd = DescManager::getSingletonPtr()->getDefaultColorPickerDesc();
		cd->resetToDefault();

		cd->widget_dimensions = dimensions;

		return createColorPicker(cd);
	}

	ComboBox* Panel::createComboBox(ComboBoxDesc* d)
	{
		ComboBox* newComboBox = dynamic_cast<ComboBox*>(_createWidget(d));

		addChild(newComboBox);

		return newComboBox;
	}

	ComboBox* Panel::createComboBox()
	{
		ComboBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cd->resetToDefault();

		return createComboBox(cd);
	}

	ComboBox* Panel::createComboBox(const Point& position)
	{
		ComboBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cd->resetToDefault();

		cd->widget_dimensions.position = position;

		return createComboBox(cd);
	}

	ComboBox* Panel::createComboBox(const Rect& dimensions)
	{
		ComboBoxDesc* cd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cd->resetToDefault();

		cd->widget_dimensions = dimensions;

		return createComboBox(cd);
	}

	Console* Panel::createConsole(ConsoleDesc* d)
	{
		Console* newConsole = dynamic_cast<Console*>(_createWidget(d));

		addChild(newConsole);

		return newConsole;
	}

	Console* Panel::createConsole()
	{
		ConsoleDesc* cd = DescManager::getSingletonPtr()->getDefaultConsoleDesc();
		cd->resetToDefault();

		return createConsole(cd);
	}

	Console* Panel::createConsole(const Point& position)
	{
		ConsoleDesc* cd = DescManager::getSingletonPtr()->getDefaultConsoleDesc();
		cd->resetToDefault();

		cd->widget_dimensions.position = position;

		return createConsole(cd);
	}

	Console* Panel::createConsole(const Rect& dimensions)
	{
		ConsoleDesc* cd = DescManager::getSingletonPtr()->getDefaultConsoleDesc();
		cd->resetToDefault();

		cd->widget_dimensions = dimensions;

		return createConsole(cd);
	}

	HScrollBar* Panel::createHScrollBar(HScrollBarDesc* d)
	{
		HScrollBar* newHScrollBar = dynamic_cast<HScrollBar*>(_createWidget(d));

		addChild(newHScrollBar);

		return newHScrollBar;
	}

	HScrollBar* Panel::createHScrollBar()
	{
		HScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultHScrollBarDesc();
		sd->resetToDefault();

		return createHScrollBar(sd);
	}

	HScrollBar* Panel::createHScrollBar(const Point& position)
	{
		HScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultHScrollBarDesc();
		sd->resetToDefault();

		sd->widget_dimensions.position = position;

		return createHScrollBar(sd);
	}

	HScrollBar* Panel::createHScrollBar(const Rect& dimensions)
	{
		HScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultHScrollBarDesc();
		sd->resetToDefault();

		sd->widget_dimensions = dimensions;

		return createHScrollBar(sd);
	}

	Image* Panel::createImage(ImageDesc* d)
	{
		Image* newImage = dynamic_cast<Image*>(_createWidget(d));

		addChild(newImage);

		return newImage;
	}

	Image* Panel::createImage()
	{
		ImageDesc* id = DescManager::getSingletonPtr()->getDefaultImageDesc();
		id->resetToDefault();

		return createImage(id);
	}

	Image* Panel::createImage(const Point& position)
	{
		ImageDesc* id = DescManager::getSingletonPtr()->getDefaultImageDesc();
		id->resetToDefault();

		id->widget_dimensions.position = position;

		return createImage(id);
	}

	Image* Panel::createImage(const Rect& dimensions)
	{
		ImageDesc* id = DescManager::getSingletonPtr()->getDefaultImageDesc();
		id->resetToDefault();

		id->widget_dimensions = dimensions;

		return createImage(id);
	}

	Label* Panel::createLabel(LabelDesc* d)
	{
		Label* newLabel = dynamic_cast<Label*>(_createWidget(d));

		addChild(newLabel);

		return newLabel;
	}

	Label* Panel::createLabel()
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment("Label"));

		return createLabel(ld);
	}

	Label* Panel::createLabel(const Point& position)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment("Label"));
		ld->widget_dimensions.position = position;

		return createLabel(ld);
	}

	Label* Panel::createLabel(const Rect& dimensions)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment("Label"));
		ld->widget_dimensions = dimensions;

		return createLabel(ld);
	}

	Label* Panel::createLabel(const Ogre::UTFString& text, const Point& position)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment(text));
		ld->widget_dimensions.position = position;

		return createLabel(ld);
	}

	Label* Panel::createLabel(std::vector<TextSegment> segments, const Point& position)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments = segments;
		ld->widget_dimensions.position = position;

		return createLabel(ld);
	}

	Label* Panel::createLabel(const Ogre::UTFString& text, const Rect& dimensions)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments.push_back(TextSegment(text));
		ld->widget_dimensions = dimensions;

		return createLabel(ld);
	}

	Label* Panel::createLabel(std::vector<TextSegment> segments, const Rect& dimensions)
	{
		LabelDesc* ld = DescManager::getSingletonPtr()->getDefaultLabelDesc();
		ld->resetToDefault();

		ld->textDesc.segments = segments;
		ld->widget_dimensions = dimensions;

		return createLabel(ld);
	}

	List* Panel::createList(ListDesc* d)
	{
		List* newList = dynamic_cast<List*>(_createWidget(d));

		addChild(newList);

		return newList;
	}

	List* Panel::createList()
	{
		ListDesc* ld = DescManager::getSingletonPtr()->getDefaultListDesc();
		ld->resetToDefault();

		return createList(ld);
	}

	List* Panel::createList(const Point& position)
	{
		ListDesc* ld = DescManager::getSingletonPtr()->getDefaultListDesc();
		ld->resetToDefault();

		ld->widget_dimensions.position = position;

		return createList(ld);
	}

	List* Panel::createList(const Rect& dimensions)
	{
		ListDesc* ld = DescManager::getSingletonPtr()->getDefaultListDesc();
		ld->resetToDefault();

		ld->widget_dimensions = dimensions;

		return createList(ld);
	}

	Panel* Panel::createPanel(PanelDesc* d)
	{
		Panel* newPanel = dynamic_cast<Panel*>(_createWidget(d));
		
		addChild(newPanel);

		return newPanel;
	}

	Panel* Panel::createPanel()
	{
		PanelDesc* pd = DescManager::getSingletonPtr()->getDefaultPanelDesc();
		pd->resetToDefault();

		return createPanel(pd);
	}

	Panel* Panel::createPanel(const Point& position)
	{
		PanelDesc* pd = DescManager::getSingletonPtr()->getDefaultPanelDesc();
		pd->resetToDefault();

		pd->widget_dimensions.position = position;

		return createPanel(pd);
	}

	Panel* Panel::createPanel(const Rect& dimensions)
	{
		PanelDesc* pd = DescManager::getSingletonPtr()->getDefaultPanelDesc();
		pd->resetToDefault();

		pd->widget_dimensions = dimensions;

		return createPanel(pd);
	}

	ProgressBar* Panel::createProgressBar(ProgressBarDesc* d)
	{
		ProgressBar* newProgressBar = dynamic_cast<ProgressBar*>(_createWidget(d));

		addChild(newProgressBar);

		return newProgressBar;
	}

	ProgressBar* Panel::createProgressBar()
	{
		ProgressBarDesc* pd = DescManager::getSingletonPtr()->getDefaultProgressBarDesc();
		pd->resetToDefault();

		return createProgressBar(pd);
	}

	ProgressBar* Panel::createProgressBar(const Point& position)
	{
		ProgressBarDesc* pd = DescManager::getSingletonPtr()->getDefaultProgressBarDesc();
		pd->resetToDefault();

		pd->widget_dimensions.position = position;

		return createProgressBar(pd);
	}

	ProgressBar* Panel::createProgressBar(const Rect& dimensions)
	{
		ProgressBarDesc* pd = DescManager::getSingletonPtr()->getDefaultProgressBarDesc();
		pd->resetToDefault();

		pd->widget_dimensions = dimensions;

		return createProgressBar(pd);
	}

	PropertyGrid* Panel::createPropertyGrid(PropertyGridDesc* d)
	{
		PropertyGrid* newPropertyGrid = dynamic_cast<PropertyGrid*>(_createWidget(d));

		addChild(newPropertyGrid);

		return newPropertyGrid;
	}

	PropertyGrid* Panel::createPropertyGrid()
	{
		PropertyGridDesc* pd = DescManager::getSingletonPtr()->getDefaultPropertyGridDesc();
		pd->resetToDefault();

		return createPropertyGrid(pd);
	}

	PropertyGrid* Panel::createPropertyGrid(const Point& position)
	{
		PropertyGridDesc* pd = DescManager::getSingletonPtr()->getDefaultPropertyGridDesc();
		pd->resetToDefault();

		pd->widget_dimensions.position = position;

		return createPropertyGrid(pd);
	}

	PropertyGrid* Panel::createPropertyGrid(const Rect& dimensions)
	{
		PropertyGridDesc* pd = DescManager::getSingletonPtr()->getDefaultPropertyGridDesc();
		pd->resetToDefault();

		pd->widget_dimensions = dimensions;

		return createPropertyGrid(pd);
	}

	RadioButton* Panel::createRadioButton(RadioButtonDesc* d)
	{
		RadioButton* newRadioButton = dynamic_cast<RadioButton*>(_createWidget(d));

		addChild(newRadioButton);

		return newRadioButton;
	}

	RadioButton* Panel::createRadioButton()
	{
		RadioButtonDesc* rd = DescManager::getSingletonPtr()->getDefaultRadioButtonDesc();
		rd->resetToDefault();

		return createRadioButton(rd);
	}

	RadioButton* Panel::createRadioButton(const Point& position)
	{
		RadioButtonDesc* rd = DescManager::getSingletonPtr()->getDefaultRadioButtonDesc();
		rd->resetToDefault();

		rd->widget_dimensions.position = position;

		return createRadioButton(rd);
	}

	RadioButton* Panel::createRadioButton(const Rect& dimensions)
	{
		RadioButtonDesc* rd = DescManager::getSingletonPtr()->getDefaultRadioButtonDesc();
		rd->resetToDefault();

		rd->widget_dimensions = dimensions;

		return createRadioButton(rd);
	}

	TabControl* Panel::createTabControl(TabControlDesc* d)
	{
		TabControl* newTabControl = dynamic_cast<TabControl*>(_createWidget(d));

		addChild(newTabControl);

		return newTabControl;
	}

	TabControl* Panel::createTabControl()
	{
		TabControlDesc* td = DescManager::getSingletonPtr()->getDefaultTabControlDesc();
		td->resetToDefault();

		return createTabControl(td);
	}

	TabControl* Panel::createTabControl(const Point& position)
	{
		TabControlDesc* td = DescManager::getSingletonPtr()->getDefaultTabControlDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createTabControl(td);
	}

	TabControl* Panel::createTabControl(const Rect& dimensions)
	{
		TabControlDesc* td = DescManager::getSingletonPtr()->getDefaultTabControlDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createTabControl(td);
	}

	TextArea* Panel::createTextArea(TextAreaDesc* d)
	{
		TextArea* newTextArea = dynamic_cast<TextArea*>(_createWidget(d));

		addChild(newTextArea);

		return newTextArea;
	}

	TextArea* Panel::createTextArea()
	{
		TextAreaDesc* td = DescManager::getSingletonPtr()->getDefaultTextAreaDesc();
		td->resetToDefault();

		return createTextArea(td);
	}

	TextArea* Panel::createTextArea(const Point& position)
	{
		TextAreaDesc* td = DescManager::getSingletonPtr()->getDefaultTextAreaDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createTextArea(td);
	}

	TextArea* Panel::createTextArea(const Rect& dimensions)
	{
		TextAreaDesc* td = DescManager::getSingletonPtr()->getDefaultTextAreaDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createTextArea(td);
	}

	TextBox* Panel::createTextBox(TextBoxDesc* d)
	{
		TextBox* newTextBox = dynamic_cast<TextBox*>(_createWidget(d));

		addChild(newTextBox);

		return newTextBox;
	}

	TextBox* Panel::createTextBox()
	{
		TextBoxDesc* td = DescManager::getSingletonPtr()->getDefaultTextBoxDesc();
		td->resetToDefault();

		return createTextBox(td);
	}

	TextBox* Panel::createTextBox(const Point& position)
	{
		TextBoxDesc* td = DescManager::getSingletonPtr()->getDefaultTextBoxDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createTextBox(td);
	}

	TextBox* Panel::createTextBox(const Rect& dimensions)
	{
		TextBoxDesc* td = DescManager::getSingletonPtr()->getDefaultTextBoxDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createTextBox(td);
	}

	ToolBar* Panel::createToolBar(ToolBarDesc* d)
	{
		ToolBar* newToolBar = dynamic_cast<ToolBar*>(_createWidget(d));

		addChild(newToolBar);

		return newToolBar;
	}

	ToolBar* Panel::createToolBar()
	{
		ToolBarDesc* td = DescManager::getSingletonPtr()->getDefaultToolBarDesc();
		td->resetToDefault();

		return createToolBar(td);
	}

	ToolBar* Panel::createToolBar(const Point& position)
	{
		ToolBarDesc* td = DescManager::getSingletonPtr()->getDefaultToolBarDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createToolBar(td);
	}

	ToolBar* Panel::createToolBar(const Rect& dimensions)
	{
		ToolBarDesc* td = DescManager::getSingletonPtr()->getDefaultToolBarDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createToolBar(td);
	}

	TreeView* Panel::createTreeView(TreeViewDesc* d)
	{
		TreeView* newTreeView = dynamic_cast<TreeView*>(_createWidget(d));

		addChild(newTreeView);

		return newTreeView;
	}

	TreeView* Panel::createTreeView()
	{
		TreeViewDesc* td = DescManager::getSingletonPtr()->getDefaultTreeViewDesc();
		td->resetToDefault();

		return createTreeView(td);
	}

	TreeView* Panel::createTreeView(const Point& position)
	{
		TreeViewDesc* td = DescManager::getSingletonPtr()->getDefaultTreeViewDesc();
		td->resetToDefault();

		td->widget_dimensions.position = position;

		return createTreeView(td);
	}

	TreeView* Panel::createTreeView(const Rect& dimensions)
	{
		TreeViewDesc* td = DescManager::getSingletonPtr()->getDefaultTreeViewDesc();
		td->resetToDefault();

		td->widget_dimensions = dimensions;

		return createTreeView(td);
	}

	VScrollBar* Panel::createVScrollBar(VScrollBarDesc* d)
	{
		VScrollBar* newVScrollBar = dynamic_cast<VScrollBar*>(_createWidget(d));

		addChild(newVScrollBar);

		return newVScrollBar;
	}

	VScrollBar* Panel::createVScrollBar()
	{
		VScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultVScrollBarDesc();
		sd->resetToDefault();

		return createVScrollBar(sd);
	}

	VScrollBar* Panel::createVScrollBar(const Point& position)
	{
		VScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultVScrollBarDesc();
		sd->resetToDefault();

		sd->widget_dimensions.position = position;

		return createVScrollBar(sd);
	}

	VScrollBar* Panel::createVScrollBar(const Rect& dimensions)
	{
		VScrollBarDesc* sd = DescManager::getSingletonPtr()->getDefaultVScrollBarDesc();
		sd->resetToDefault();

		sd->widget_dimensions = dimensions;

		return createVScrollBar(sd);
	}

	Ogre::String Panel::getClass()
	{
		return "Panel";
	}

	void Panel::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void Panel::clearChildren()
	{
		destroyChildren();
		redraw();
	}
}
