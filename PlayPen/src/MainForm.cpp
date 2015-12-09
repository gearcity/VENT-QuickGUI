#include "MainForm.h"

#include "OgreMeshManager.h"

MainForm::MainForm(PlayPen* e, Ogre::RenderWindow* w) :
	Window(e,w),
	mSheetFromFile(NULL)
{
	// I got tired of typing "QuickGUI::" in front of everything..
	using namespace QuickGUI;

	Root::getSingleton().setDefaultFontName("micross.12");

	bool loadFromDisk = 0;

	if(loadFromDisk)
	{
		mSheetFromFile = SheetManager::getSingleton().createSheet("resources\\Test.sheet",true);

		mGUIManager->setActiveSheet(mSheetFromFile);
	}
	else
	{
		// Create Sheet
		SheetDesc* sd = DescManager::getSingleton().getDefaultSheetDesc();
		sd->widget_dimensions.size = Size(800,600);
		mSheetFromFile = SheetManager::getSingleton().createSheet(sd);
		mGUIManager->setActiveSheet(mSheetFromFile);

		
		TabControl* tc = mGUIManager->getActiveSheet()->createTabControl(Rect(275,75,250,250));
		tc->setTabReordering(true);
		Root::getSingleton().setDefaultColor(ColourValue::Black);
		tc->createTabPage("Page 2");
		tc->createTabPage("Page 1",0);
		tc->createTabPageWithIcon("qgui.image.png","Page 3",0);
		tc->createTabPageWithIcon("qgui.image.png",0);
		Root::getSingleton().setDefaultColor(ColourValue::White);

		PropertyGrid* propertyGrid = mGUIManager->getActiveSheet()->createPropertyGrid(Rect(550,75,250,300));
		PropertyGridSection* section1 = propertyGrid->createSection();
		section1->createTextProperty();
		PropertyGridSection* section2 = section1->createSection();
		section2->createTextProperty();
		section1->createBoolProperty();
		
		TreeView* treeView = mGUIManager->getActiveSheet()->createTreeView(Rect(0,75,200,300));
		TreeViewNode* level0Node = treeView->createNode("Node 0");

		for(int i = 1; i < 25; ++i)
		{
			treeView->createNode("Node " + Ogre::StringConverter::toString(i));
		}

		TreeViewNode* level1Node = level0Node->createNode("Node 2");
		TreeViewNode* level2Node = level1Node->createNode("Node 3");
		TreeViewNode* tvn = level0Node->createNode("Node 4");
		treeView->createNode("Node 5");

		TreeViewCheckBoxNode* tvcbn = treeView->createCheckBoxNode("Node 6");
		tvn = tvcbn->createNode("Node 7");
		tvn->createRadioButtonNode("Node 8");
		tvn->createRadioButtonNode("Node 9");
		
		// Create ToolBar with some Menus
		ToolBar* tb = mGUIManager->getActiveSheet()->createToolBar(Rect(0,560,200,20));

		tb->createMenu("File");
		tb->createMenu();
		Menu* m1 = tb->createMenu("Format");
		Menu* m2 = m1->createSubMenu("Word Wrap");
		m1->createTextItem();
		Menu* m3 = m2->createSubMenu();
		m3->createTextItem();
		m3->setTextColor(ColourValue::Green);

		// Create ComboBox with some items
		ComboBox* comboBox = mGUIManager->getActiveSheet()->createComboBox(Rect(325,50,150,20));
		comboBox->setMaxDropDownHeight(60);
		comboBox->setDropDownWidth(150);

		comboBox->createTextItem("CB Item 1");
		comboBox->createTextItem("CB Item 2");
		comboBox->createTextItem("CB Item 3",0);
		comboBox->createImageItem("qgui.image.png");

		// Create List with some items
		List* list = mGUIManager->getActiveSheet()->createList(Rect(300,450,200,100));
		list->setMultiSelect(true);

		list->createTextItem("Item 3");
		list->createTextItem("Item 2",0);
		list->createTextItem("Item 1",0);
		list->createTextItem("Item 5");
		list->createTextItem("Item 4",3);
		list->createImageItem("qgui.image.png");

//		cpd->resetToDefault();
//		cpd->widget_dimensions.position = Point(50,375);
//		mGUIManager->getActiveSheet()->createColorPicker(cpd);
		

		if(!loadFromDisk)
			mGUIManager->getActiveSheet()->saveToDisk(".\\resources\\Test.sheet");
/*
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshManager::getSingleton().createPlane("ground",
           Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
           1500,1500,20,20,true,1,5,5,Ogre::Vector3::UNIT_Z);
		Ogre::Entity* ent = mSceneManager->createEntity("GroundEntity", "ground");
		mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
		ent->setCastShadows(false);

		ent = mSceneManager->createEntity("Ninja", "ninja.mesh");
        ent->setCastShadows(true);
        mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

		Ogre::Light* light = mSceneManager->createLight("Light1");
		light->setType(Ogre::Light::LT_POINT);
		light->setPosition(Ogre::Vector3(0, 150, 250));

		mCamera->setPosition(0, 100, 500); 
		mCamera->lookAt(0,0,0);

		mSceneManager->setAmbientLight(Ogre::ColourValue(0, 0, 0));
*/		//mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	}
}

void MainForm::test1(const QuickGUI::EventArgs& args)
{
	dynamic_cast<QuickGUI::ComboBox*>(mSheetFromFile->findWidget("CB"))->createTextItem();
	/*
	using namespace QuickGUI;

	SheetDesc* sd = DescManager::getSingleton().getDefaultSheetDesc();
	sd->resetToDefault();
	Sheet* newSheet = new Sheet(sd);
	newSheet->setConsumeKeyboardEvents(true);
	newSheet->setTransparencyPicking(false);

	newSheet->addWidgetEventHandler(WIDGET_EVENT_KEY_UP,&MainForm::test2,this);

	mGUIManager->setActiveSheet(newSheet);
	*/
}

void MainForm::test2(const QuickGUI::EventArgs& args)
{
	//mGUIManager->setActiveSheet(mSheetFromFile);

	dynamic_cast<QuickGUI::ComboBox*>(mSheetFromFile->findWidget("CB"))->clearItems();
}

MainForm::~MainForm()
{
}

void MainForm::onExitPressed(const QuickGUI::EventArgs& args)
{
	mSkinSetEditor->exit();
}

void MainForm::testConsoleHandler(QuickGUI::Console* c, bool& clearInputBox, bool& addToDisplayArea)
{
	clearInputBox = false;
	addToDisplayArea = true;
}

void MainForm::windowClosed(Ogre::RenderWindow* rw)
{
	mSkinSetEditor->exit();
}
