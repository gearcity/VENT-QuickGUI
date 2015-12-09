#include "MainForm.h"

#include "OgreMeshManager.h"

namespace QuickGUIEditor
{
	MainForm::MainForm(QuickGUIEditorApp* e, Ogre::RenderWindow* w) :
		Window(e,w),
		mSheet(NULL),
		mSelectedWidget(NULL),
		mLastExecutedHandler(NULL)
	{
		Root::getSingleton().setDefaultFontName("micross.12");

		SheetDesc* sd = DescManager::getSingleton().getDefaultSheetDesc();
		sd->widget_dimensions.size = Size(1024,768);

		mSheet = SheetManager::getSingleton().createSheet(sd);
		mGUIManager->setActiveSheet(mSheet);
		mMouseCursor = mGUIManager->getMouseCursor();

		FactoryManager::getSingleton().getWidgetFactory()->setNamePrefix("QuickGUIEditor.");
		_createContextMenu();
		_createPropertyWindow();
		_createOverwriteWindow();
		_createLoadWindow();
		_createSaveWindow();
		FactoryManager::getSingleton().getWidgetFactory()->setNamePrefix("");
	}

	MainForm::~MainForm()
	{
	}

	void MainForm::_createContextMenu()
	{
		mContextMenu = mSheet->createContextMenu("NULL",Size(150,100));
		mContextMenu->setSerialize(false);
		mContextMenu->setTitleBarDragable(false);

		MenuTextItem* mti = NULL;

		Menu* appMenu = mContextMenu->createSubMenu("Application");
		appMenu->setMaxMenuPanelHeight(250);
		
		mti = appMenu->createTextItem("New Sheet");
		
		mti = appMenu->createTextItem("Load...");
		mti->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&MainForm::onLoadPressed,this);
		
		mti = appMenu->createTextItem("Save");
		mti->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&MainForm::onSavePressed,this);
		
		mti = appMenu->createTextItem("Options");
		
		mti = appMenu->createTextItem("Exit");
		mti->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&MainForm::onExitPressed,this);

		Menu* createMenu = mContextMenu->createSubMenu("Create");
		createMenu->setMaxMenuPanelHeight(250);
		createMenu->setMenuPanelWidth(150);
		
		mCreateButtonItem = createMenu->createTextItem("Button");
		mCreateButtonItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateButtonHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateButton,this);
		mCreateButtonItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateButtonHandler);
		
		mCreateComboBoxItem = createMenu->createTextItem("ComboBox");
		mCreateComboBoxItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateComboBoxHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateComboBox,this);
		mCreateComboBoxItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateComboBoxHandler);
		
		mCreateConsoleItem = createMenu->createTextItem("Console");
		mCreateConsoleItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateConsoleHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateConsole,this);
		mCreateConsoleItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateConsoleHandler);
		
		mCreateContextMenuItem = createMenu->createTextItem("ContextMenu");
		mCreateContextMenuItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateContextMenuHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateContextMenu,this);
		mCreateContextMenuItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateContextMenuHandler);
		
		mCreateHScrollBarItem = createMenu->createTextItem("HScrollBar");
		mCreateHScrollBarItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateHScrollBarHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateHScrollBar,this);
		mCreateHScrollBarItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateHScrollBarHandler);
		
		mCreateImageItem = createMenu->createTextItem("Image");
		mCreateImageItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateImageHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateImage,this);
		mCreateImageItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateImageHandler);
		
		mCreateLabelItem = createMenu->createTextItem("Label");
		mCreateLabelItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateLabelHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateLabel,this);
		mCreateLabelItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateLabelHandler);
		
		mCreateListItem = createMenu->createTextItem("List");
		mCreateListItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateListHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateList,this);
		mCreateListItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateListHandler);

		mCreateListImageItemItem = createMenu->createTextItem("ListImageItem");
		mCreateListImageItemItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateListImageItemHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateListImageItem,this);
		mCreateListImageItemItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateListImageItemHandler);

		mCreateListTextItemItem = createMenu->createTextItem("ListTextItem");
		mCreateListTextItemItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateListTextItemHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateListTextItem,this);
		mCreateListTextItemItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateListTextItemHandler);
		
		mCreateMenuItem = createMenu->createTextItem("Menu");
		mCreateMenuItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateMenuHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateMenu,this);
		mCreateMenuItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateMenuHandler);

		mCreateMenuImageItemItem = createMenu->createTextItem("MenuImageItem");
		mCreateMenuImageItemItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateMenuImageItemHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateMenuImageItem,this);
		mCreateMenuImageItemItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateMenuImageItemHandler);

		mCreateMenuTextItemItem = createMenu->createTextItem("MenuTextItem");
		mCreateMenuTextItemItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateMenuTextItemHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateMenuTextItem,this);
		mCreateMenuTextItemItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateMenuTextItemHandler);
		
		mCreatePanelItem = createMenu->createTextItem("Panel");
		mCreatePanelItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreatePanelHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreatePanel,this);
		mCreatePanelItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreatePanelHandler);
		
		mCreateProgressBarItem = createMenu->createTextItem("ProgressBar");
		mCreateProgressBarItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateProgressBarHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateProgressBar,this);
		mCreateProgressBarItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateProgressBarHandler);
		
		mCreateRadioButtonItem = createMenu->createTextItem("RadioButton");
		mCreateRadioButtonItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateRadioButtonHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateRadioButton,this);
		mCreateRadioButtonItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateRadioButtonHandler);
		
		mCreateTabControlItem = createMenu->createTextItem("TabControl");
		mCreateTabControlItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateTabControlHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateTabControl,this);
		mCreateTabControlItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateTabControlHandler);
		
		mCreateTextAreaItem = createMenu->createTextItem("TextArea");
		mCreateTextAreaItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateTextAreaHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateTextArea,this);
		mCreateTextAreaItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateTextAreaHandler);

		mCreateTextBoxItem = createMenu->createTextItem("TextBox");
		mCreateTextBoxItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateTextBoxHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateTextBox,this);
		mCreateTextBoxItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateTextBoxHandler);

		mCreateToolBarItem = createMenu->createTextItem("ToolBar");
		mCreateToolBarItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateToolBarHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateToolBar,this);
		mCreateToolBarItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateToolBarHandler);
		
		mCreateTreeViewItem = createMenu->createTextItem("TreeView");
		mCreateTreeViewItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateTreeViewHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateTreeView,this);
		mCreateTreeViewItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateTreeViewHandler);
		
		mCreateVScrollBarItem = createMenu->createTextItem("VScrollBar");
		mCreateVScrollBarItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateVScrollBarHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateVScrollBar,this);
		mCreateVScrollBarItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateVScrollBarHandler);
		
		mCreateWindowItem = createMenu->createTextItem("Window");
		mCreateWindowItem->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mCreateWindowHandler = OGRE_NEW_T(EventHandlerPointer<MainForm>,Ogre::MEMCATEGORY_GENERAL)(&MainForm::onCreateWindow,this);
		mCreateWindowItem->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mCreateWindowHandler);
		
		mti = mContextMenu->createTextItem("Properties");
		mti->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&MainForm::onPropertyPressed,this);

		mti = mContextMenu->createTextItem("Delete");
		mti->addWidgetEventHandler(WIDGET_EVENT_ENABLED_CHANGED,&MainForm::onMenuItemEnableChanged,this);
		mti->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&MainForm::onDeletePressed,this);
	}

	void MainForm::hideCreateMenuItems()
	{
		mCreateButtonItem->setVisible(false);
		mCreateComboBoxItem->setVisible(false);
		mCreateConsoleItem->setVisible(false);
		mCreateContextMenuItem->setVisible(false);
		mCreateHScrollBarItem->setVisible(false);
		mCreateImageItem->setVisible(false);
		mCreateLabelItem->setVisible(false);
		mCreateListItem->setVisible(false);
		mCreateListImageItemItem->setVisible(false);
		mCreateListTextItemItem->setVisible(false);
		mCreateMenuItem->setVisible(false);
		mCreateMenuImageItemItem->setVisible(false);
		mCreateMenuTextItemItem->setVisible(false);
		mCreatePanelItem->setVisible(false);
		mCreateProgressBarItem->setVisible(false);
		mCreateRadioButtonItem->setVisible(false);
		mCreateTabControlItem->setVisible(false);
		mCreateTextAreaItem->setVisible(false);
		mCreateTextBoxItem->setVisible(false);
		mCreateToolBarItem->setVisible(false);
		mCreateTreeViewItem->setVisible(false);
		mCreateVScrollBarItem->setVisible(false);
		mCreateWindowItem->setVisible(false);
	}

	void MainForm::showCreateMenuItems(Widget* w)
	{
		hideCreateMenuItems();

		if(isA<List>(w) || isA<ComboBox>(w))
		{
			mCreateListImageItemItem->setVisible(true);
			mCreateListTextItemItem->setVisible(true);
		}

		if(isA<Panel>(w))
		{
			mCreateButtonItem->setVisible(true);
			mCreateComboBoxItem->setVisible(true);
			mCreateConsoleItem->setVisible(true);
			mCreateHScrollBarItem->setVisible(true);
			mCreateImageItem->setVisible(true);
			mCreateLabelItem->setVisible(true);
			mCreateListItem->setVisible(true);
			mCreatePanelItem->setVisible(true);
			mCreateProgressBarItem->setVisible(true);
			mCreateRadioButtonItem->setVisible(true);
			mCreateTabControlItem->setVisible(true);
			mCreateTextAreaItem->setVisible(true);
			mCreateTextBoxItem->setVisible(true);
			mCreateToolBarItem->setVisible(true);
			mCreateTreeViewItem->setVisible(true);
			mCreateVScrollBarItem->setVisible(true);
		}

		if(isA<Sheet>(w))
		{
			mCreateWindowItem->setVisible(true);
		}

		if(isA<ToolBar>(w))
		{
			mCreateMenuItem->setVisible(true);
		}

		if(isA<Menu>(w))
		{
			mCreateMenuItem->setVisible(true);
			mCreateMenuImageItemItem->setVisible(true);
			mCreateMenuTextItemItem->setVisible(true);
		}
	}

	void MainForm::onPropertyPressed(const EventArgs& args)
	{
		mPropertyWindow->setVisible(true);
	}

	void MainForm::onPW_SheetButtonPressed(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			selectWidget(mSheet);
	}

	void MainForm::onPW_DeleteButtonPressed(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if((mSelectedWidget != NULL) && (mSelectedWidget != mSheet))
			{
				mSelectedWidget->destroy();

				selectWidget(mSheet);
			}
		}
	}

	void MainForm::_createPropertyGrid(TabPage* p)
	{
		Root::getSingleton().setDefaultFontName("micross.14");

		PropertyGrid* propertyGrid = p->createPropertyGrid(Rect(Point::ZERO,p->getPageClientArea()));
		propertyGrid->setSkinType("QuickGUIEditor");
		propertyGrid->setTransparencyPicking(false);
		propertyGrid->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		propertyGrid->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);

		PropertyGridSection* s;
		PropertyGridComboBoxProperty* pgcbp;

		// Widget Section
		{
			s = propertyGrid->createSection("Widget");

			s->createBoolProperty("Enabled");
			
			s->createTextProperty("Disabled Skin Type");
			
			s->createBoolProperty("Dragable");
			
			pgcbp = s->createComboBoxProperty("Horizontal Anchor");
			pgcbp->setDropDownWidth(300);
			pgcbp->createTextItem("ANCHOR_HORIZONTAL_CENTER");
			pgcbp->createTextItem("ANCHOR_HORIZONTAL_CENTER_DYNAMIC");
			pgcbp->createTextItem("ANCHOR_HORIZONTAL_LEFT");
			pgcbp->createTextItem("ANCHOR_HORIZONTAL_RIGHT");
			pgcbp->createTextItem("ANCHOR_HORIZONTAL_LEFT_RIGHT");
			pgcbp->createTextItem("ANCHOR_HORIZONTAL_NONE");

			s->createTextProperty("Hover Time");
			
			s->createBoolProperty("Inherit Opacity");
			
			s->createTextProperty("Name");

			{
				PropertyGridSection* positionSection = s->createSection("Position");

				positionSection->createTextProperty("X");
			
				positionSection->createTextProperty("Y");

				positionSection->createBoolProperty("Relative To Parent Client Area");
			}
			
			s->createTextProperty("Relative Opacity");

			{
				PropertyGridSection* resizeSection = s->createSection("Resizable");

				resizeSection->createBoolProperty("Bottom");
			
				resizeSection->createBoolProperty("Left");
				
				resizeSection->createBoolProperty("Right");
				
				resizeSection->createBoolProperty("Top");
			}
			
			s->createBoolProperty("Scrollable");
			
			s->createBoolProperty("Serialize");

			{
				PropertyGridSection* sizeSection = s->createSection("Size");

				sizeSection->createTextProperty("Width");
			
				sizeSection->createTextProperty("Height");

				sizeSection->createTextProperty("Max Width");

				sizeSection->createTextProperty("Min Width");
			
				sizeSection->createTextProperty("Max Height");
				
				sizeSection->createTextProperty("Min Height");
			}
			
			s->createTextProperty("Skin Type");
			
			s->createBoolProperty("Transparency Picking");
			
			pgcbp = s->createComboBoxProperty("Vertical Anchor");
			pgcbp->setDropDownWidth(300);
			pgcbp->createTextItem("ANCHOR_VERTICAL_CENTER");
			pgcbp->createTextItem("ANCHOR_VERTICAL_CENTER_DYNAMIC");
			pgcbp->createTextItem("ANCHOR_VERTICAL_TOP");
			pgcbp->createTextItem("ANCHOR_VERTICAL_BOTTOM");
			pgcbp->createTextItem("ANCHOR_VERTICAL_TOP_BOTTOM");
			pgcbp->createTextItem("ANCHOR_VERTICAL_NONE");
			
			s->createBoolProperty("Visible");
		}

		s = propertyGrid->createSection("Button");

		s = propertyGrid->createSection("CheckBox");

		Root::getSingleton().setDefaultFontName("micross.12");
	}

	void MainForm::_createPropertyWindow()
	{
		WindowDesc* wd = DescManager::getSingletonPtr()->getDefaultWindowDesc();
		wd->resetToDefault();
		wd->textDesc.horizontalTextAlignment = TEXT_ALIGNMENT_HORIZONTAL_CENTER;
		wd->widget_visible = false;
		wd->widget_dimensions = Rect(0,0,325,450);
		wd->widget_minSize = Size(200,450);
		wd->widget_relativeOpacity = 0.8;
		mPropertyWindow = mSheet->createWindow(wd);
		mPropertyWindow->center();

		Rect r = mPropertyWindow->getClientDimensions();

		float tabControlHeight = 250;

		TabControl* tc = mPropertyWindow->createTabControl(Rect(0,0,r.size.width,tabControlHeight));
		tc->setSkinType("editor");
		tc->setTransparencyPicking(false);
		tc->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		tc->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		TabPage* propertyPage = tc->createTabPage("Properties");
		propertyPage->setSkinType("editor");
		TabPage* eventPage = tc->createTabPage("Events");
		eventPage->setSkinType("editor");
		tc->selectTabPage(0);

		_createPropertyGrid(propertyPage);
		
		PropertyGrid* eventGrid = eventPage->createPropertyGrid(Rect(Point::ZERO,eventPage->getPageClientArea()));
		eventGrid->setSkinType("QuickGUIEditor");
		eventGrid->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		eventGrid->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);

		Panel* navigationPanel = mPropertyWindow->createPanel(Rect(0,tabControlHeight,r.size.width,100));
		navigationPanel->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		navigationPanel->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);
		Rect panelClientDimensions = navigationPanel->getClientDimensions();

		float comboboxHeight = static_cast<int>(panelClientDimensions.size.height / 4.0);

		ComboBox* childComboBox = navigationPanel->createComboBox(Rect(0,0,panelClientDimensions.size.width,comboboxHeight));
		childComboBox->setText("Children...");
		childComboBox->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);

		ComboBox* windowsComboBox = navigationPanel->createComboBox(Rect(0,comboboxHeight,panelClientDimensions.size.width,comboboxHeight));
		windowsComboBox->setText("Windows...");
		windowsComboBox->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);

		ComboBox* modalWindowsComboBox = navigationPanel->createComboBox(Rect(0,(comboboxHeight * 2),panelClientDimensions.size.width,comboboxHeight));
		modalWindowsComboBox->setText("Modal Windows...");
		modalWindowsComboBox->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);

		Button* sheetButton = navigationPanel->createButton("Sheet",Rect(0,(comboboxHeight * 3),panelClientDimensions.size.width,comboboxHeight));
		sheetButton->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		sheetButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&MainForm::onPW_SheetButtonPressed,this);

		mPropertyWindow->createLabel("Navigation", navigationPanel->getPosition() + Point(0,-5))->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);

		Rect remainingArea;
		remainingArea.position.y = navigationPanel->getPosition().y + navigationPanel->getHeight();
		remainingArea.size.height = r.size.height - remainingArea.position.y;
		remainingArea.size.width = r.size.width;

		Button* deleteSelfButton = mPropertyWindow->createButton("Delete This Widget",Rect(remainingArea.position,Size(remainingArea.size.width,remainingArea.size.height / 2.0)));
		deleteSelfButton->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		deleteSelfButton->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);
		deleteSelfButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&MainForm::onPW_DeleteButtonPressed,this);

		Button* deleteChildrenButton = mPropertyWindow->createButton("Delete Children...",Rect(Point(remainingArea.position.x,remainingArea.position.y + (remainingArea.size.height / 2.0)),Size(remainingArea.size.width,remainingArea.size.height / 2.0)));
		deleteChildrenButton->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		deleteChildrenButton->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);
	}

	void MainForm::_createLoadWindow()
	{
		ModalWindowDesc* mwd = DescManager::getSingletonPtr()->getDefaultModalWindowDesc();
		mwd->resetToDefault();
		mwd->window_titleBarCloseButton = false;
		mwd->window_titleBarDragable = false;
		mwd->textDesc.horizontalTextAlignment = TEXT_ALIGNMENT_HORIZONTAL_CENTER;
		mwd->textDesc.segments.push_back(TextSegment("Load Sheet from File"));
		mwd->widget_visible = false;
		mwd->widget_resizeFromBottom = false;
		mwd->widget_resizeFromLeft = false;
		mwd->widget_resizeFromRight = false;
		mwd->widget_resizeFromTop = false;
		mwd->widget_dimensions.size = Size(300,200);
		mwd->widget_relativeOpacity = 0.8;
		mLoadWindow = mSheet->createModalWindow(mwd);

		Rect r = mLoadWindow->getClientDimensions();

		// Set allotted width so Horizontal text centering occurs
		mLoadWindow->setTitleBarTextAllottedWidth(r.size.width);

		// Create the Save/Cancel buttons
		float buttonWidth = r.size.width / 3.0;
		float buttonHeight = 30;
		float buttonYPos = r.size.height - (buttonHeight * 2);

		Button* loadButton = mLoadWindow->createButton("Load",Rect(buttonWidth / 3.0,buttonYPos,buttonWidth,buttonHeight));
		loadButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onLoadWindowLoadPressed,this);

		Button* cancelButton = mLoadWindow->createButton("Cancel",Rect((buttonWidth * 2.0) - (buttonWidth / 3.0),buttonYPos,buttonWidth,buttonHeight));
		cancelButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onLoadWindowCancelPressed,this);

		// Create Label and TextBox
		float labelXPos = 25;
		float labelYPos = 25;
		Label* labelTB = mLoadWindow->createLabel("File Name:",Point(labelXPos,labelYPos));

		float textBoxHeight = 25;
		float textBoxWidth = r.size.width - (labelTB->getPosition().x + labelTB->getWidth()) - 50;
		float textBoxXPos = (labelTB->getPosition().x + labelTB->getWidth()) + 25;
		mLoadWindowFileName = mLoadWindow->createTextBox(Rect(textBoxXPos,labelYPos - ((textBoxHeight - labelTB->getHeight()) / 2.0),textBoxWidth,25));
		mLoadWindowFileName->addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&MainForm::onKeyDownInTextBox,this);

		mLoadWindowErrorMessage = mLoadWindow->createLabel(Rect(0,mLoadWindowFileName->getPosition().y + mLoadWindowFileName->getHeight(),r.size.width,50));
		mLoadWindowErrorMessage->setTextColor(ColourValue::Red);
		mLoadWindowErrorMessage->setVisible(false);
	}

	void MainForm::_createOverwriteWindow()
	{
		ModalWindowDesc* mwd = DescManager::getSingleton().getDefaultModalWindowDesc();
		mwd->resetToDefault();
		mwd->window_titleBarCloseButton = false;
		mwd->window_titleBarDragable = false;
		mwd->textDesc.segments.push_back(TextSegment("Warning"));
		mwd->widget_visible = false;
		mwd->widget_resizeFromBottom = false;
		mwd->widget_resizeFromLeft = false;
		mwd->widget_resizeFromRight = false;
		mwd->widget_resizeFromTop = false;
		mwd->widget_dimensions.size = Size(275,175);
		mOverwriteWindow = mSheet->createModalWindow(mwd);

		Rect r = mOverwriteWindow->getClientDimensions();

		// Create the Save/Cancel buttons
		float buttonWidth = r.size.width / 3.0;
		float buttonHeight = 30;
		float buttonYPos = r.size.height - (buttonHeight * 2);

		Button* yesButton = mOverwriteWindow->createButton("Yes",Rect(buttonWidth / 3.0,buttonYPos,buttonWidth,buttonHeight));
		yesButton->addWidgetEventHandler(WIDGET_EVENT_KEY_UP,&MainForm::onOverwriteWindowYesPressed,this);

		Button* noButton = mOverwriteWindow->createButton("No",Rect((buttonWidth * 2.0) - (buttonWidth / 3.0),buttonYPos,buttonWidth,buttonHeight));
		noButton->addWidgetEventHandler(WIDGET_EVENT_KEY_UP,&MainForm::onOverwriteWindowNoPressed,this);
	}

	void MainForm::_createSaveWindow()
	{
		ModalWindowDesc* mwd = DescManager::getSingleton().getDefaultModalWindowDesc();
		mwd->resetToDefault();
		mwd->window_titleBarCloseButton = false;
		mwd->window_titleBarDragable = false;
		mwd->textDesc.horizontalTextAlignment = TEXT_ALIGNMENT_HORIZONTAL_CENTER;
		mwd->textDesc.segments.push_back(TextSegment("Save Sheet to File"));
		mwd->widget_visible = false;
		mwd->widget_resizeFromBottom = false;
		mwd->widget_resizeFromLeft = false;
		mwd->widget_resizeFromRight = false;
		mwd->widget_resizeFromTop = false;
		mwd->widget_dimensions.size = Size(300,200);
		mwd->widget_relativeOpacity = 0.8;
		mSaveWindow = mSheet->createModalWindow(mwd);

		Rect r = mSaveWindow->getClientDimensions();

		// Set allotted width so Horizontal text centering occurs
		mSaveWindow->setTitleBarTextAllottedWidth(r.size.width);

		// Create the Save/Cancel buttons
		float buttonWidth = r.size.width / 3.0;
		float buttonHeight = 30;
		float buttonYPos = r.size.height - (buttonHeight * 2);

		Button* saveButton = mSaveWindow->createButton("Save",Rect(buttonWidth / 3.0,buttonYPos,buttonWidth,buttonHeight));
		saveButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onSaveWindowSavePressed,this);

		Button* cancelButton = mSaveWindow->createButton("Cancel",Rect((buttonWidth * 2.0) - (buttonWidth / 3.0),buttonYPos,buttonWidth,buttonHeight));
		cancelButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onSaveWindowCancelPressed,this);

		// Create Label and TextBox
		float labelXPos = 25;
		float labelYPos = 25;
		Label* labelTB = mSaveWindow->createLabel("File Name:",Point(labelXPos,labelYPos));

		float textBoxHeight = 25;
		float textBoxWidth = r.size.width - (labelTB->getPosition().x + labelTB->getWidth()) - 50;
		float textBoxXPos = (labelTB->getPosition().x + labelTB->getWidth()) + 25;
		mSaveWindowFileName = mSaveWindow->createTextBox(Rect(textBoxXPos,labelYPos - ((textBoxHeight - labelTB->getHeight()) / 2.0),textBoxWidth,25));
	}

	Widget* MainForm::getBaseWidget(Widget* widget)
	{
		if(widget == NULL)
			return mSheet;

		Widget* w = widget->getBaseWidget();

		// This shouldn't be needed, but I want to make sure this function will not return NULL.
		if(w == NULL)
			return mSheet;

		return w;
	}

	bool MainForm::areModalDialogsVisible()
	{
		if(mOverwriteWindow->getVisible())
			return true;

		if(mSaveWindow->getVisible())
			return true;

		if(mLoadWindow->getVisible())
			return true;

		return false;
	}

	bool MainForm::defaultInputValidator(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText)
	{
		// Allow periods
		if(cp == 46)
			return true;

		// Allow digits 0 through 9
		if((cp >= 48) && (cp <= 57))
			return true;

		// Allow spaces
		if(cp == 32)
			return true;
		
		// Allow [azAZ]

		return false;
	}

	Point MainForm::getScreenPosition(Widget* w)
	{
		if(w == NULL)
			return Point::ZERO;

		Point p = w->getTexturePosition();

		QuickGUI::Window* win = w->getWindow();
		if(win != mSheet)
		{
			p = ((win->getPosition() + w->getClientDimensions().position) - p);
		}

		return p;
	}

	bool MainForm::isEditorWidget(Widget* widget)
	{
		if(widget == NULL)
			return false;

		if((widget == mContextMenu) || widget->isChildOf(mContextMenu))
			return true;

		if((widget == mPropertyWindow) || widget->isChildOf(mPropertyWindow))
			return true;

		if((widget == mOverwriteWindow) || widget->isChildOf(mOverwriteWindow))
			return true;

		if((widget == mSaveWindow) || widget->isChildOf(mSaveWindow))
			return true;

		if((widget == mLoadWindow) || widget->isChildOf(mLoadWindow))
			return true;

		return false;
	}

	bool MainForm::keyPressed(const OIS::KeyEvent &arg)
	{
		KeyCode kc = static_cast<QuickGUI::KeyCode>(arg.key);

		if((kc == KC_DELETE) && 
			(mSelectedWidget != NULL) &&
			(getBaseWidget(mGUIManager->getLastClickedWidget()) == mSelectedWidget) &&
			(mSelectedWidget != mSheet))
		{
			mSelectedWidget->destroy();

			selectWidget(mSheet);
			
			// Make sure ContextMenu is hidden
			mContextMenu->setVisible(false);
		}
		else if((mSheet->getKeyboardListener() != NULL) && isEditorWidget(mSheet->getKeyboardListener()))
		{
			mGUIManager->injectChar(static_cast<Ogre::UTFString::unicode_char>(arg.text));
			mGUIManager->injectKeyDown(kc);
		}

		return true;
	}

	void MainForm::loadSheet(const Ogre::String& sheetFileName)
	{
		try
		{
			throw QuickGUI::Exception(0,"","");
		}
		catch(QuickGUI::Exception ex)
		{
			mLoadWindowErrorMessage->setText("HI");
		}

		// Create the Sheet without the SheetManager.  Otherwise there would be problems loading sheets with the same name as the 
		// currently shown sheet, since it has not yet been deleted. (delete it AFTER successful loading of the new sheet
		Sheet* newSheet = SheetManager::getSingleton().createSheet(sheetFileName,true);
		
		mLoadWindow->setVisible(false);
		mLoadWindowFileName->clearText();

		mSheet->_setSheet(NULL);

		// Remove our Editor Windows from the Sheet
		mSheet->removeModalWindow(mLoadWindow);
		mSheet->removeModalWindow(mSaveWindow);
		mSheet->removeModalWindow(mOverwriteWindow);
		mSheet->removeWindow(mContextMenu);
		mSheet->removeWindow(mPropertyWindow);

		mGUIManager->setActiveSheet(newSheet);
		delete mSheet;

		mSheet = newSheet;
		mSelectedWidget = mSheet;

		// Add our Editor Windows to the Sheet
		mSheet->addModalWindow(mLoadWindow);
		mSheet->addModalWindow(mSaveWindow);
		mSheet->addModalWindow(mOverwriteWindow);
		mSheet->addWindow(mContextMenu);

		mLoadWindowErrorMessage->setVisible(false);
		mLoadWindowErrorMessage->clearText();
	}

	bool MainForm::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		Widget* w = getBaseWidget(mGUIManager->getWidgetUnderMouseCursor());

		// If right mouse button pressed, select widget and show context menu
		if(id == OIS::MB_Right)
		{
			if(!areModalDialogsVisible() && !isEditorWidget(w))
			{
				if(mSelectedWidget != w)
					selectWidget(w);

				mContextMenu->setTitleBarText(w->getName());
				mContextMenu->show(Point(arg.state.X.abs,arg.state.Y.abs));
			}
		}
		// Else if left mouse button pressed
		else if(id == OIS::MB_Left)
		{
			if(!isEditorWidget(w))
			{
				if(mSelectedWidget != w)
					selectWidget(w);
			}
			
			mGUIManager->injectMouseButtonDown(static_cast<QuickGUI::MouseButtonID>(id));
		}
		else if(id == OIS::MB_Middle)
		{
			if(mLastExecutedHandler != NULL)
			{
				EventArgs args;
				mLastExecutedHandler->execute(args);
			}
		}

		return true;
	}

	bool MainForm::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		mGUIManager->injectMouseButtonUp(static_cast<QuickGUI::MouseButtonID>(id));

		// Inject mouse up against mSelectedWidget if we are over it.  The inject above will cause the DROPPED event to be fired, but
		// we also want it to receive MOUSE_BUTTON_RELEASED events.
		if((mSelectedWidget != NULL) && (getBaseWidget(mGUIManager->getWidgetUnderMouseCursor()) == mSelectedWidget))
		{
			mGUIManager->injectMouseButtonUp(static_cast<QuickGUI::MouseButtonID>(id));
		}

		return true;
	}

	void MainForm::onCreateButton(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
				
			overloadWidgetAttributes(p->createButton(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateButtonHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateComboBox(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
				
			overloadWidgetAttributes(p->createComboBox(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateComboBoxHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateConsole(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
				
			overloadWidgetAttributes(p->createConsole(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateConsoleHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateContextMenu(const QuickGUI::EventArgs& args)
	{
		Sheet* s = dynamic_cast<Sheet*>(mSelectedWidget);

		if(s != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(s->createContextMenu(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateContextMenuHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateHScrollBar(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createHScrollBar(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateHScrollBarHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateImage(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createImage(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateImageHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateLabel(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createLabel(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateLabelHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateList(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createList(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateListHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateListImageItem(const EventArgs& args)
	{
		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);

		List* l = dynamic_cast<List*>(mSelectedWidget);

		if(l != NULL)
		{
			l->createImageItem();
			return;
		}

		ComboBox* c = dynamic_cast<ComboBox*>(mSelectedWidget);

		if(c != NULL)
		{
			c->createImageItem();
			return;
		}
	}

	void MainForm::onCreateListTextItem(const EventArgs& args)
	{
		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);

		List* l = dynamic_cast<List*>(mSelectedWidget);

		if(l != NULL)
		{
			l->createTextItem();
			return;
		}

		ComboBox* c = dynamic_cast<ComboBox*>(mSelectedWidget);

		if(c != NULL)
		{
			c->createTextItem();
			return;
		}
	}

	void MainForm::onCreateMenu(const QuickGUI::EventArgs& args)
	{
		Ogre::String className = mSelectedWidget->getClass();
		if(className == "Menu")
		{
			overloadWidgetAttributes(dynamic_cast<Menu*>(mSelectedWidget)->createSubMenu());
		}
		else if(className == "ContextMenu")
		{
			overloadWidgetAttributes(dynamic_cast<ContextMenu*>(mSelectedWidget)->createSubMenu());
		}
		else if(className == "ToolBar")
		{
			overloadWidgetAttributes(dynamic_cast<ToolBar*>(mSelectedWidget)->createMenu());
		}

		mLastExecutedHandler = mCreateMenuHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateMenuImageItem(const QuickGUI::EventArgs& args)
	{
		Ogre::String className = mSelectedWidget->getClass();
		if(className == "Menu")
		{
			overloadWidgetAttributes(dynamic_cast<Menu*>(mSelectedWidget)->createImageItem());
		}
		else if(className == "ContextMenu")
		{
			overloadWidgetAttributes(dynamic_cast<ContextMenu*>(mSelectedWidget)->createImageItem());
		}

		mLastExecutedHandler = mCreateMenuTextItemHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateMenuTextItem(const QuickGUI::EventArgs& args)
	{
		Ogre::String className = mSelectedWidget->getClass();
		if(className == "Menu")
		{
			overloadWidgetAttributes(dynamic_cast<Menu*>(mSelectedWidget)->createTextItem());
		}
		else if(className == "ContextMenu")
		{
			overloadWidgetAttributes(dynamic_cast<ContextMenu*>(mSelectedWidget)->createTextItem());
		}

		mLastExecutedHandler = mCreateMenuTextItemHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreatePanel(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createPanel(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreatePanelHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateProgressBar(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createProgressBar(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateProgressBarHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateRadioButton(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createRadioButton(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateRadioButtonHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateTabControl(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createTabControl(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateTabControlHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateTextBox(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createTextBox(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateTextAreaHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateTextArea(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createTextArea(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateTextBoxHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateToolBar(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createToolBar(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateToolBarHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateTreeView(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createTreeView(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateTreeViewHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateVScrollBar(const QuickGUI::EventArgs& args)
	{
		Panel* p = dynamic_cast<Panel*>(mSelectedWidget);

		if(p != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(p->createVScrollBar(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateVScrollBarHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onCreateWindow(const QuickGUI::EventArgs& args)
	{
		Sheet* s = dynamic_cast<Sheet*>(mSelectedWidget);

		if(s != NULL)
		{
			Point position = mContextMenu->getPosition();
			if(!mContextMenu->getVisible())
				position = mMouseCursor->getPosition();
			
			overloadWidgetAttributes(s->createWindow(position - getScreenPosition(mSelectedWidget)));
		}

		mLastExecutedHandler = mCreateWindowHandler;

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onDeletePressed(const EventArgs& args)
	{
		if((mSelectedWidget != NULL) && (mSelectedWidget != mSheet))
		{
			mSelectedWidget->destroy();

			selectWidget(mSheet);
		}

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onKeyDownInTextBox(const EventArgs& args)
	{
		const KeyEventArgs& kea = dynamic_cast<const KeyEventArgs&>(args);

		if(kea.scancode == KC_RETURN)
		{
			if(mLoadWindowFileName->getText() != "")
				loadSheet(".\\user\\sheets\\" + mLoadWindowFileName->getText());
		}
	}

	void MainForm::onLoadWindowLoadPressed(const EventArgs& args)
	{
		if(mLoadWindowFileName->getText() != "")
		{
			loadSheet(".\\user\\sheets\\" + mLoadWindowFileName->getText());
		}
	}

	void MainForm::onLoadWindowCancelPressed(const EventArgs& args)
	{
		mLoadWindow->setVisible(false);

		mLoadWindowFileName->clearText();

		mLoadWindowErrorMessage->setVisible(false);
		mLoadWindowErrorMessage->clearText();
	}

	void MainForm::onMenuItemEnableChanged(const QuickGUI::EventArgs& args)
	{
		const WidgetEventArgs* wea = dynamic_cast<const WidgetEventArgs*>(&args);

		if(wea->widget != NULL)
		{
			if(!wea->widget->getEnabled())
				dynamic_cast<MenuTextItem*>(wea->widget)->setTextColor(ColourValue(0.5,0.5,0.5));
			else
				dynamic_cast<MenuTextItem*>(wea->widget)->setTextColor(ColourValue::White);
		}
	}

	void MainForm::onExitPressed(const QuickGUI::EventArgs& args)
	{
		mEditor->exit();
	}

	void MainForm::onOverwriteWindowYesPressed(const EventArgs& args)
	{
	}

	void MainForm::onOverwriteWindowNoPressed(const EventArgs& args)
	{
	}

	void MainForm::onLoadPressed(const EventArgs& args)
	{
		mLoadWindow->center();
		mLoadWindow->setVisible(true);
		mSheet->setKeyboardListener(mLoadWindowFileName);

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onSavePressed(const EventArgs& args)
	{
		mSaveWindow->center();
		mSaveWindow->setVisible(true);

		// Make sure ContextMenu is hidden
		mContextMenu->setVisible(false);
	}

	void MainForm::onSaveWindowSavePressed(const EventArgs& args)
	{
		if(mSaveWindowFileName->getText() != "")
		{
			mSheet->saveToDisk(".\\user\\sheets\\" + mSaveWindowFileName->getText());

			mSaveWindow->setVisible(false);

			mSaveWindowFileName->clearText();
		}
	}

	void MainForm::onSaveWindowCancelPressed(const EventArgs& args)
	{
		mSaveWindow->setVisible(false);

		mSaveWindowFileName->clearText();
	}

	void MainForm::overloadWidgetAttributes(Widget* w)
	{
		if(mWidgetOverloads.find(w) != mWidgetOverloads.end())
			return;

		if(w->isComponentOfAWidget() || 
			w->isPositionManaged() ||
			w->isSizeManaged())
			return;

		OverloadedWidgetAttributes a;
		a.dragable = w->getDragable();
		a.enabled = w->getEnabled();
		a.moveBaseWidgetOnDrag = w->getMoveBaseWidgetOnDrag();
		a.resizeBottom = w->getResizeFromBottom();
		a.resizeLeft = w->getResizeFromLeft();
		a.resizeRight = w->getResizeFromRight();
		a.resizeTop = w->getResizeFromTop();

		mWidgetOverloads[w] = a;

		w->setDragable(true);
		w->setEnabled(true);
		w->setMoveBaseWidgetOnDrag(true);
		w->setResizeFromBottom(true);
		w->setResizeFromLeft(true);
		w->setResizeFromRight(true);
		w->setResizeFromTop(true);
	}

	void MainForm::overloadWidgetAttributes()
	{
		for(std::map<Widget*,OverloadedWidgetAttributes>::iterator it = mWidgetOverloads.begin(); it != mWidgetOverloads.end(); ++it)
		{
			Widget* w = (*it).first;

			w->setDragable(true);
			w->setEnabled(true);
			w->setMoveBaseWidgetOnDrag(true);
			w->setResizeFromBottom(true);
			w->setResizeFromLeft(true);
			w->setResizeFromRight(true);
			w->setResizeFromTop(true);
		}
	}

	void MainForm::restoreWidgetAttributes()
	{
		for(std::map<Widget*,OverloadedWidgetAttributes>::iterator it = mWidgetOverloads.begin(); it != mWidgetOverloads.end(); ++it)
		{
			Widget* w = (*it).first;
			OverloadedWidgetAttributes a = (*it).second;

			w->setDragable(a.dragable);
			w->setEnabled(a.enabled);
			w->setMoveBaseWidgetOnDrag(a.moveBaseWidgetOnDrag);
			w->setResizeFromBottom(a.resizeBottom);
			w->setResizeFromLeft(a.resizeLeft);
			w->setResizeFromRight(a.resizeRight);
			w->setResizeFromTop(a.resizeTop);
		}
	}

	void MainForm::selectWidget(QuickGUI::Widget* w)
	{
		mContextMenu->hide();

		if(mSelectedWidget != NULL)
		{
			// restore properties
			mSelectedWidget->setVisible(mIsVisible);
		}

		mSelectedWidget = w;

		if(mSelectedWidget != NULL)
		{
			mPropertyWindow->setTitleBarText(mSelectedWidget->getName());

			showCreateMenuItems(mSelectedWidget);

			// store properties
			mIsVisible = mSelectedWidget->getVisible();
		}
	}

	void MainForm::windowClosed(Ogre::RenderWindow* rw)
	{
		mEditor->exit();
	}

	void MainForm::writeSheetToDisk(const Ogre::String& fileName)
	{
		restoreWidgetAttributes();
		mSheet->saveToDisk(fileName);
		overloadWidgetAttributes();
	}
}
