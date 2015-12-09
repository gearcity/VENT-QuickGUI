#include "QuickGUIRoot.h"
// Managers
#include "QuickGUIFactoryManager.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIScriptWriter.h"
#include "QuickGUISheetManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITimerManager.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIWidgetFactory.h"
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
#include "QuickGUIListImageItem.h"
#include "QuickGUIListPanelItem.h"
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

#include "OgreStringConverter.h"
#include "OgreViewport.h"

template<> QuickGUI::Root* Ogre::Singleton<QuickGUI::Root>::msSingleton = 0;

namespace QuickGUI
{
	Root::Root() :
		mGUIManagerCounter(-1),
		mDefaultHoverTime(3)
	{
		// Get the default font
		Ogre::ResourceManager::ResourceMapIterator it = Ogre::FontManager::getSingleton().getResourceIterator();
		if(it.hasMoreElements())
			mDefaultFont = static_cast<Ogre::FontPtr>(it.getNext());
		else
			throw Exception(Exception::ERR_INVALID_STATE,"No fonts have been found! At least one font must exist for QuickGUI use!","Root::Root");

		mDefaultColor = ColourValue::White;

		// Initialize all Singleton Manager classes
		OGRE_NEW Brush();
		OGRE_NEW EventHandlerManager();
		OGRE_NEW FactoryManager();
		OGRE_NEW SheetManager();
		OGRE_NEW SkinDefinitionManager();
		OGRE_NEW TimerManager();

		// Register default supported Widget types via FactoryManager
		WidgetFactory* widgetFactory = FactoryManager::getSingletonPtr()->getWidgetFactory();
		widgetFactory->registerClass<Button>("Button");
		widgetFactory->registerClass<CheckBox>("CheckBox");
		widgetFactory->registerClass<ColorPicker>("ColorPicker");
		widgetFactory->registerClass<ComboBox>("ComboBox");
		widgetFactory->registerClass<Console>("Console");
		widgetFactory->registerClass<ContextMenu>("ContextMenu");
		widgetFactory->registerClass<HScrollBar>("HScrollBar");
		widgetFactory->registerClass<Image>("Image");
		widgetFactory->registerClass<Label>("Label");
		widgetFactory->registerClass<List>("List");
		widgetFactory->registerClass<ListImageItem>("ListImageItem");
		widgetFactory->registerClass<ListPanelItem>("ListPanelItem");
		widgetFactory->registerClass<ListTextItem>("ListTextItem");
		widgetFactory->registerClass<Menu>("Menu");
		widgetFactory->registerClass<MenuImageItem>("MenuImageItem");
		widgetFactory->registerClass<MenuTextItem>("MenuTextItem");
		widgetFactory->registerClass<MenuPanel>("MenuPanel");
		widgetFactory->registerClass<ModalWindow>("ModalWindow");
		widgetFactory->registerClass<Panel>("Panel");
		widgetFactory->registerClass<ProgressBar>("ProgressBar");
		widgetFactory->registerClass<PropertyGrid>("PropertyGrid");
		widgetFactory->registerClass<PropertyGridSection>("PropertyGridSection");
		widgetFactory->registerClass<PropertyGridTextProperty>("PropertyGridTextProperty");
		widgetFactory->registerClass<PropertyGridBoolProperty>("PropertyGridBoolProperty");
		widgetFactory->registerClass<PropertyGridComboBoxProperty>("PropertyGridComboBoxProperty");
		widgetFactory->registerClass<RadioButton>("RadioButton");
		widgetFactory->registerClass<TabControl>("TabControl");
		widgetFactory->registerClass<Tab>("Tab");
		widgetFactory->registerClass<TabPage>("TabPage");
		widgetFactory->registerClass<TextArea>("TextArea");
		widgetFactory->registerClass<TextBox>("TextBox");
		widgetFactory->registerClass<TitleBar>("TitleBar");
		widgetFactory->registerClass<ToolBar>("ToolBar");
		widgetFactory->registerClass<TreeView>("TreeView");
		widgetFactory->registerClass<TreeViewNode>("TreeViewNode");
		widgetFactory->registerClass<TreeViewCheckBoxNode>("TreeViewCheckBoxNode");
		widgetFactory->registerClass<TreeViewRadioButtonNode>("TreeViewRadioButtonNode");
		widgetFactory->registerClass<VScrollBar>("VScrollBar");
		widgetFactory->registerClass<Window>("Window");

		// Register default supported Widget desc types
		DescFactory* descFactory = FactoryManager::getSingletonPtr()->getDescFactory();
		descFactory->registerClass<ButtonDesc>("ButtonDesc");
		descFactory->registerClass<CheckBoxDesc>("CheckBoxDesc");
		descFactory->registerClass<ColorPickerDesc>("ColorPickerDesc");
		descFactory->registerClass<ComboBoxDesc>("ComboBoxDesc");
		descFactory->registerClass<ConsoleDesc>("ConsoleDesc");
		descFactory->registerClass<ContextMenuDesc>("ContextMenuDesc");
		descFactory->registerClass<HScrollBarDesc>("HScrollBarDesc");
		descFactory->registerClass<ImageDesc>("ImageDesc");
		descFactory->registerClass<LabelDesc>("LabelDesc");
		descFactory->registerClass<ListDesc>("ListDesc");
		descFactory->registerClass<ListImageItemDesc>("ListImageItemDesc");
		descFactory->registerClass<ListPanelItemDesc>("ListPanelItemDesc");
		descFactory->registerClass<ListTextItemDesc>("ListTextItemDesc");
		descFactory->registerClass<MenuDesc>("MenuDesc");
		descFactory->registerClass<MenuImageItemDesc>("MenuImageItemDesc");
		descFactory->registerClass<MenuTextItemDesc>("MenuTextItemDesc");
		descFactory->registerClass<MenuPanelDesc>("MenuPanelDesc");
		descFactory->registerClass<ModalWindowDesc>("ModalWindowDesc");
		descFactory->registerClass<PanelDesc>("PanelDesc");
		descFactory->registerClass<ProgressBarDesc>("ProgressBarDesc");
		descFactory->registerClass<PropertyGridDesc>("PropertyGridDesc");
		descFactory->registerClass<PropertyGridSectionDesc>("PropertyGridSectionDesc");
		descFactory->registerClass<PropertyGridTextPropertyDesc>("PropertyGridTextPropertyDesc");
		descFactory->registerClass<PropertyGridBoolPropertyDesc>("PropertyGridBoolPropertyDesc");
		descFactory->registerClass<PropertyGridComboBoxPropertyDesc>("PropertyGridComboBoxPropertyDesc");
		descFactory->registerClass<RadioButtonDesc>("RadioButtonDesc");
		descFactory->registerClass<SheetDesc>("SheetDesc");
		descFactory->registerClass<TabControlDesc>("TabControlDesc");
		descFactory->registerClass<TabDesc>("TabDesc");
		descFactory->registerClass<TabPageDesc>("TabPageDesc");
		descFactory->registerClass<TextAreaDesc>("TextAreaDesc");
		descFactory->registerClass<TextBoxDesc>("TextBoxDesc");
		descFactory->registerClass<TitleBarDesc>("TitleBarDesc");
		descFactory->registerClass<ToolBarDesc>("ToolBarDesc");
		descFactory->registerClass<TreeViewDesc>("TreeViewDesc");
		descFactory->registerClass<TreeViewNodeDesc>("TreeViewNodeDesc");
		descFactory->registerClass<TreeViewCheckBoxNodeDesc>("TreeViewCheckBoxNodeDesc");
		descFactory->registerClass<TreeViewRadioButtonNodeDesc>("TreeViewRadioButtonNodeDesc");
		descFactory->registerClass<VScrollBarDesc>("VScrollBarDesc");
		descFactory->registerClass<WindowDesc>("WindowDesc");

		// Register Skin definitions
		MouseCursor::registerSkinDefinition();
		Button::registerSkinDefinition();
		CheckBox::registerSkinDefinition();
		ColorPicker::registerSkinDefinition();
		ComboBox::registerSkinDefinition();
		Console::registerSkinDefinition();
		ContextMenu::registerSkinDefinition();
		HScrollBar::registerSkinDefinition();
		Image::registerSkinDefinition();
		Label::registerSkinDefinition();
		List::registerSkinDefinition();
		ListImageItem::registerSkinDefinition();
		ListPanelItem::registerSkinDefinition();
		ListTextItem::registerSkinDefinition();
		Menu::registerSkinDefinition();
		MenuImageItem::registerSkinDefinition();
		MenuTextItem::registerSkinDefinition();
		MenuPanel::registerSkinDefinition();
		ModalWindow::registerSkinDefinition();
		Panel::registerSkinDefinition();
		ProgressBar::registerSkinDefinition();
		PropertyGrid::registerSkinDefinition();
		PropertyGridSection::registerSkinDefinition();
		PropertyGridTextProperty::registerSkinDefinition();
		PropertyGridBoolProperty::registerSkinDefinition();
		PropertyGridComboBoxProperty::registerSkinDefinition();
		RadioButton::registerSkinDefinition();
		Sheet::registerSkinDefinition();
		Tab::registerSkinDefinition();
		TabControl::registerSkinDefinition();
		TabPage::registerSkinDefinition();
		TextArea::registerSkinDefinition();
		TextBox::registerSkinDefinition();
		TextCursor::registerSkinDefinition();
		TitleBar::registerSkinDefinition();
		ToolBar::registerSkinDefinition();
		TreeView::registerSkinDefinition();
		TreeViewNode::registerSkinDefinition();
		TreeViewCheckBoxNode::registerSkinDefinition();
		TreeViewRadioButtonNode::registerSkinDefinition();
		VScrollBar::registerSkinDefinition();
		Window::registerSkinDefinition();

		// Initialize other Singleton classes
		OGRE_NEW ScriptWriter();
		OGRE_NEW SerialWriter();
		OGRE_NEW SerialReader();

		OGRE_NEW DescManager();
	}

	Root::~Root()
	{
		OGRE_DELETE SheetManager::getSingletonPtr();

		for(std::map<std::string,GUIManager*>::iterator it = mGUIManagers.begin(); it != mGUIManagers.end(); ++it)
			OGRE_DELETE_T((*it).second,GUIManager,Ogre::MEMCATEGORY_GENERAL);
		mGUIManagers.clear();

		OGRE_DELETE ScriptReader::getSingletonPtr();
		OGRE_DELETE ScriptWriter::getSingletonPtr();
		OGRE_DELETE SerialWriter::getSingletonPtr();
		OGRE_DELETE SerialReader::getSingletonPtr();

		OGRE_DELETE DescManager::getSingletonPtr();
		// Destroying Sheets will place all widgets and descs in the factory freelist.  Its important
		// To destroy the factories before destroying the TimerManager, as some Widgets may use Timers.
		OGRE_DELETE FactoryManager::getSingletonPtr();
		OGRE_DELETE TimerManager::getSingletonPtr();
		OGRE_DELETE EventHandlerManager::getSingletonPtr();
		OGRE_DELETE Brush::getSingletonPtr();
		OGRE_DELETE SkinDefinitionManager::getSingletonPtr();
	}

	Root* Root::getSingletonPtr(void) 
	{ 
		return msSingleton; 
	}

	Root& Root::getSingleton(void) 
	{ 
		assert( msSingleton );  
		return ( *msSingleton ); 
	}

	GUIManager* Root::createGUIManager(GUIManagerDesc& d)
	{
		++mGUIManagerCounter;

		Ogre::String previousName = d.name;

		// If name string is empty, generate a name for the GUIManager.
		if(d.name == "")
			d.name = "GUIManager." + Ogre::StringConverter::toString(mGUIManagerCounter);
		else
		{
			// make sure name does not already exist.
			if(mGUIManagers.find(d.name) != mGUIManagers.end())
				throw Ogre::Exception(Ogre::Exception::ERR_DUPLICATE_ITEM,"A GUIManager with name \"" + d.name + "\" already exists!","Root::createGUIManager");
		}

		GUIManager* newGUIManager = OGRE_NEW_T(GUIManager,Ogre::MEMCATEGORY_GENERAL)(d);
		mGUIManagers[d.name] = newGUIManager;

		// restore name
		d.name = previousName;

		return newGUIManager;
	}

	void Root::destroyGUIManager(GUIManager* gm)
	{
		if(gm == NULL)
			return;

		destroyGUIManager(gm->getName());
	}

	void Root::destroyGUIManager(const std::string& name)
	{
	
		GUIManager* gm = mGUIManagers[name];
		mGUIManagers.erase(mGUIManagers.find(name));
		OGRE_DELETE_T(gm,GUIManager,Ogre::MEMCATEGORY_GENERAL);
	}

	GUIManager* Root::getGUIManager(const std::string& name)
	{
		if(mGUIManagers.find(name) == mGUIManagers.end())
			return NULL;

		return mGUIManagers[name];
	}

	ColourValue Root::getDefaultColor()
	{
		return mDefaultColor;
	}

	Ogre::String Root::getDefaultFontName()
	{
		if(mDefaultFont.isNull())
			return "";
		else
			return mDefaultFont->getName();
	}

	float Root::getDefaultHoverTime()
	{
		return mDefaultHoverTime;
	}

	void Root::notifySheetDestroyed(Sheet* sheet)
	{
		notifyWidgetDestroyed(sheet);

		for(std::map<std::string,GUIManager*>::iterator it = mGUIManagers.begin(); it != mGUIManagers.end(); ++it)
		{
			if((*it).second->getActiveSheet() == sheet)
				(*it).second->setActiveSheet(NULL);
		}
	}

	void Root::notifyWidgetDestroyed(Widget* w)
	{
		for(std::map<std::string,GUIManager*>::iterator it = mGUIManagers.begin(); it != mGUIManagers.end(); ++it)
		{
			if(((*it).second->getWidgetUnderMouseCursor() != NULL) && (w->findWidget((*it).second->getWidgetUnderMouseCursor()->getName()) != NULL))
				(*it).second->clearWidgetUnderMouseCursorReference();
			if(((*it).second->getLastClickedWidget() != NULL) && (w->findWidget((*it).second->getLastClickedWidget()->getName()) != NULL))
				(*it).second->clearLastClickedWidgetReference();
			if(((*it).second->getActiveSheet() != NULL) && ((*it).second->getActiveSheet()->getKeyboardListener() == w))
				(*it).second->getActiveSheet()->mKeyboardListener = NULL;
		}
	}

	void Root::setDefaultColor(const ColourValue& cv)
	{
		mDefaultColor = cv;
	}

	void Root::setDefaultFontName(const Ogre::String& fontName)
	{
		mDefaultFont = Text::getFont(fontName);
	}

	void Root::setDefaultHoverTime(float seconds)
	{
		mDefaultHoverTime = seconds;
	}
}
