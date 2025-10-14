#include "QuickGUIDescManager.h"
// Default provided Widgets
#include "QuickGUIButton.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIColorPicker.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIConsole.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIImage.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUIListPanelItem.h"
#include "QuickGUIListImageItem.h"
#include "QuickGUIMenu.h"
#include "QuickGUIMenuImageItem.h"
#include "QuickGUIMenuTextItem.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIModalWindow.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUIPropertyGridSection.h"
#include "QuickGUIPropertyGridTextProperty.h"
#include "QuickGUIPropertyGridBoolProperty.h"
#include "QuickGUIPropertyGridComboBoxProperty.h"
#include "QuickGUIRadioButton.h"
#include "QuickGUISheet.h"
#include "QuickGUITab.h"
#include "QuickGUITabControl.h"
#include "QuickGUITabPage.h"
#include "QuickGUITextArea.h"
#include "QuickGUITextBox.h"
#include "QuickGUITitleBar.h"
#include "QuickGUIToolBar.h"
#include "QuickGUITreeView.h"
#include "QuickGUITreeViewCheckBoxNode.h"
#include "QuickGUITreeViewNode.h"
#include "QuickGUITreeViewRadioButtonNode.h"
#include "QuickGUIVScrollBar.h"

template<> QuickGUI::DescManager* Ogre::Singleton<QuickGUI::DescManager>::msSingleton = 0;

namespace QuickGUI
{
	DescManager::DescManager()
	{
		DescFactory* f = FactoryManager::getSingleton().getDescFactory();

		mDefaultButtonDesc = f->createInstance<ButtonDesc>("ButtonDesc");
		mDefaultCheckBoxDesc = f->createInstance<CheckBoxDesc>("CheckBoxDesc");
		mDefaultColorPickerDesc = f->createInstance<ColorPickerDesc>("ColorPickerDesc");
		mDefaultComboBoxDesc = f->createInstance<ComboBoxDesc>("ComboBoxDesc");
		mDefaultConsoleDesc = f->createInstance<ConsoleDesc>("ConsoleDesc");
		mDefaultContextMenuDesc = f->createInstance<ContextMenuDesc>("ContextMenuDesc");
		mDefaultHScrollBarDesc = f->createInstance<HScrollBarDesc>("HScrollBarDesc");
		mDefaultImageDesc = f->createInstance<ImageDesc>("ImageDesc");
		mDefaultLabelDesc = f->createInstance<LabelDesc>("LabelDesc");
		mDefaultListDesc = f->createInstance<ListDesc>("ListDesc");
		mDefaultListImageItemDesc = f->createInstance<ListImageItemDesc>("ListImageItemDesc");
		mDefaultListPanelItemDesc = f->createInstance<ListPanelItemDesc>("ListPanelItemDesc");
		mDefaultListTextItemDesc = f->createInstance<ListTextItemDesc>("ListTextItemDesc");
		mDefaultMenuDesc = f->createInstance<MenuDesc>("MenuDesc");
		mDefaultMenuImageItemDesc = f->createInstance<MenuImageItemDesc>("MenuImageItemDesc");
		mDefaultMenuTextItemDesc = f->createInstance<MenuTextItemDesc>("MenuTextItemDesc");
		mDefaultMenuPanelDesc = f->createInstance<MenuPanelDesc>("MenuPanelDesc");
		mDefaultModalWindowDesc = f->createInstance<ModalWindowDesc>("ModalWindowDesc");
		mDefaultPanelDesc = f->createInstance<PanelDesc>("PanelDesc");
		mDefaultProgressBarDesc = f->createInstance<ProgressBarDesc>("ProgressBarDesc");
		mDefaultPropertyGridDesc = f->createInstance<PropertyGridDesc>("PropertyGridDesc");
		mDefaultPropertyGridSectionDesc = f->createInstance<PropertyGridSectionDesc>("PropertyGridSectionDesc");
		mDefaultPropertyGridTextPropertyDesc = f->createInstance<PropertyGridTextPropertyDesc>("PropertyGridTextPropertyDesc");
		mDefaultPropertyGridBoolPropertyDesc = f->createInstance<PropertyGridBoolPropertyDesc>("PropertyGridBoolPropertyDesc");
		mDefaultPropertyGridComboBoxPropertyDesc = f->createInstance<PropertyGridComboBoxPropertyDesc>("PropertyGridComboBoxPropertyDesc");
		mDefaultRadioButtonDesc = f->createInstance<RadioButtonDesc>("RadioButtonDesc");
		mDefaultSheetDesc = f->createInstance<SheetDesc>("SheetDesc");
		mDefaultTabControlDesc = f->createInstance<TabControlDesc>("TabControlDesc");
		mDefaultTabDesc = f->createInstance<TabDesc>("TabDesc");
		mDefaultTabPageDesc = f->createInstance<TabPageDesc>("TabPageDesc");
		mDefaultTextAreaDesc = f->createInstance<TextAreaDesc>("TextAreaDesc");
		mDefaultTextBoxDesc = f->createInstance<TextBoxDesc>("TextBoxDesc");
		mDefaultTitleBarDesc = f->createInstance<TitleBarDesc>("TitleBarDesc");
		mDefaultToolBarDesc = f->createInstance<ToolBarDesc>("ToolBarDesc");
		mDefaultTreeViewDesc = f->createInstance<TreeViewDesc>("TreeViewDesc");
		mDefaultTreeViewNodeDesc = f->createInstance<TreeViewNodeDesc>("TreeViewNodeDesc");
		mDefaultTreeViewCheckBoxNodeDesc = f->createInstance<TreeViewCheckBoxNodeDesc>("TreeViewCheckBoxNodeDesc");
		mDefaultTreeViewRadioButtonNodeDesc = f->createInstance<TreeViewRadioButtonNodeDesc>("TreeViewRadioButtonNodeDesc");
		mDefaultVScrollBarDesc = f->createInstance<VScrollBarDesc>("VScrollBarDesc");
		mDefaultWindowDesc = f->createInstance<WindowDesc>("WindowDesc");
	}

	DescManager::~DescManager()
	{
		DescFactory* f = FactoryManager::getSingleton().getDescFactory();

		f->destroyInstance(mDefaultButtonDesc);
		f->destroyInstance(mDefaultCheckBoxDesc);
		f->destroyInstance(mDefaultColorPickerDesc);
		f->destroyInstance(mDefaultComboBoxDesc);
		f->destroyInstance(mDefaultConsoleDesc);
		f->destroyInstance(mDefaultContextMenuDesc);
		f->destroyInstance(mDefaultHScrollBarDesc);
		f->destroyInstance(mDefaultImageDesc);
		f->destroyInstance(mDefaultLabelDesc);
		f->destroyInstance(mDefaultListDesc);
		f->destroyInstance(mDefaultListImageItemDesc);
		f->destroyInstance(mDefaultListTextItemDesc);
		f->destroyInstance(mDefaultMenuDesc);
		f->destroyInstance(mDefaultMenuImageItemDesc);
		f->destroyInstance(mDefaultMenuTextItemDesc);
		f->destroyInstance(mDefaultMenuPanelDesc);
		f->destroyInstance(mDefaultModalWindowDesc);
		f->destroyInstance(mDefaultPanelDesc);
		f->destroyInstance(mDefaultProgressBarDesc);
		f->destroyInstance(mDefaultPropertyGridDesc);
		f->destroyInstance(mDefaultPropertyGridSectionDesc);
		f->destroyInstance(mDefaultPropertyGridTextPropertyDesc);
		f->destroyInstance(mDefaultPropertyGridBoolPropertyDesc);
		f->destroyInstance(mDefaultPropertyGridComboBoxPropertyDesc);
		f->destroyInstance(mDefaultRadioButtonDesc);
		f->destroyInstance(mDefaultSheetDesc);
		f->destroyInstance(mDefaultTabControlDesc);
		f->destroyInstance(mDefaultTabDesc);
		f->destroyInstance(mDefaultTabPageDesc);
		f->destroyInstance(mDefaultTextAreaDesc);
		f->destroyInstance(mDefaultTextBoxDesc);
		f->destroyInstance(mDefaultTitleBarDesc);
		f->destroyInstance(mDefaultToolBarDesc);
		f->destroyInstance(mDefaultTreeViewDesc);
		f->destroyInstance(mDefaultTreeViewNodeDesc);
		f->destroyInstance(mDefaultTreeViewCheckBoxNodeDesc);
		f->destroyInstance(mDefaultTreeViewRadioButtonNodeDesc);
		f->destroyInstance(mDefaultVScrollBarDesc);
		f->destroyInstance(mDefaultWindowDesc);
	}

	DescManager* DescManager::getSingletonPtr(void) 
	{ 
		return msSingleton; 
	}

	DescManager& DescManager::getSingleton(void) 
	{ 
		assert( msSingleton );  
		return ( *msSingleton ); 
	}

	void DescManager::destroyDesc(Desc* d)
	{
		FactoryManager::getSingleton().getDescFactory()->destroyInstance(d);
	}

	ButtonDesc* DescManager::getDefaultButtonDesc()
	{
		return mDefaultButtonDesc;
	}

	CheckBoxDesc* DescManager::getDefaultCheckBoxDesc()
	{
		return mDefaultCheckBoxDesc;
	}

	ColorPickerDesc* DescManager::getDefaultColorPickerDesc()
	{
		return mDefaultColorPickerDesc;
	}

	ComboBoxDesc* DescManager::getDefaultComboBoxDesc()
	{
		return mDefaultComboBoxDesc;
	}

	ConsoleDesc* DescManager::getDefaultConsoleDesc()
	{
		return mDefaultConsoleDesc;
	}

	ContextMenuDesc* DescManager::getDefaultContextMenuDesc()
	{
		return mDefaultContextMenuDesc;
	}

	HScrollBarDesc* DescManager::getDefaultHScrollBarDesc()
	{
		return mDefaultHScrollBarDesc;
	}

	ImageDesc* DescManager::getDefaultImageDesc()
	{
		return mDefaultImageDesc;
	}

	LabelDesc* DescManager::getDefaultLabelDesc()
	{
		return mDefaultLabelDesc;
	}

	ListDesc* DescManager::getDefaultListDesc()
	{
		return mDefaultListDesc;
	}

	ListImageItemDesc* DescManager::getDefaultListImageItemDesc()
	{
		return mDefaultListImageItemDesc;
	}

	ListPanelItemDesc* DescManager::getDefaultListPanelItemDesc()
	{
		return mDefaultListPanelItemDesc;
	}

	ListTextItemDesc* DescManager::getDefaultListTextItemDesc()
	{
		return mDefaultListTextItemDesc;
	}

	MenuDesc* DescManager::getDefaultMenuDesc()
	{
		return mDefaultMenuDesc;
	}

	MenuImageItemDesc* DescManager::getDefaultMenuImageItemDesc()
	{
		return mDefaultMenuImageItemDesc;
	}

	MenuTextItemDesc* DescManager::getDefaultMenuTextItemDesc()
	{
		return mDefaultMenuTextItemDesc;
	}

	MenuPanelDesc* DescManager::getDefaultMenuPanelDesc()
	{
		return mDefaultMenuPanelDesc;
	}

	ModalWindowDesc* DescManager::getDefaultModalWindowDesc()
	{
		return mDefaultModalWindowDesc;
	}

	PanelDesc* DescManager::getDefaultPanelDesc()
	{
		return mDefaultPanelDesc;
	}

	ProgressBarDesc* DescManager::getDefaultProgressBarDesc()
	{
		return mDefaultProgressBarDesc;
	}

	PropertyGridDesc* DescManager::getDefaultPropertyGridDesc()
	{
		return mDefaultPropertyGridDesc;
	}

	PropertyGridSectionDesc* DescManager::getDefaultPropertyGridSectionDesc()
	{
		return mDefaultPropertyGridSectionDesc;
	}

	PropertyGridTextPropertyDesc* DescManager::getDefaultPropertyGridTextPropertyDesc()
	{
		return mDefaultPropertyGridTextPropertyDesc;
	}

	PropertyGridBoolPropertyDesc* DescManager::getDefaultPropertyGridBoolPropertyDesc()
	{
		return mDefaultPropertyGridBoolPropertyDesc;
	}

	PropertyGridComboBoxPropertyDesc* DescManager::getDefaultPropertyGridComboBoxPropertyDesc()
	{
		return mDefaultPropertyGridComboBoxPropertyDesc;
	}

	RadioButtonDesc* DescManager::getDefaultRadioButtonDesc()
	{
		return mDefaultRadioButtonDesc;
	}

	SheetDesc* DescManager::getDefaultSheetDesc()
	{
		return mDefaultSheetDesc;
	}

	TabControlDesc* DescManager::getDefaultTabControlDesc()
	{
		return mDefaultTabControlDesc;
	}

	TabDesc* DescManager::getDefaultTabDesc()
	{
		return mDefaultTabDesc;
	}

	TabPageDesc* DescManager::getDefaultTabPageDesc()
	{
		return mDefaultTabPageDesc;
	}

	TextAreaDesc* DescManager::getDefaultTextAreaDesc()
	{
		return mDefaultTextAreaDesc;
	}

	TextBoxDesc* DescManager::getDefaultTextBoxDesc()
	{
		return mDefaultTextBoxDesc;
	}

	TitleBarDesc* DescManager::getDefaultTitleBarDesc()
	{
		return mDefaultTitleBarDesc;
	}

	ToolBarDesc* DescManager::getDefaultToolBarDesc()
	{
		return mDefaultToolBarDesc;
	}

	TreeViewDesc* DescManager::getDefaultTreeViewDesc()
	{
		return mDefaultTreeViewDesc;
	}

	TreeViewNodeDesc* DescManager::getDefaultTreeViewNodeDesc()
	{
		return mDefaultTreeViewNodeDesc;
	}

	TreeViewCheckBoxNodeDesc* DescManager::getDefaultTreeViewCheckBoxNodeDesc()
	{
		return mDefaultTreeViewCheckBoxNodeDesc;
	}

	TreeViewRadioButtonNodeDesc* DescManager::getDefaultTreeViewRadioButtonNodeDesc()
	{
		return mDefaultTreeViewRadioButtonNodeDesc;
	}

	VScrollBarDesc* DescManager::getDefaultVScrollBarDesc()
	{
		return mDefaultVScrollBarDesc;
	}

	WindowDesc* DescManager::getDefaultWindowDesc()
	{
		return mDefaultWindowDesc;
	}
}
