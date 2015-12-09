#include "MainMenu.h"

#include "QuickGUI.h"
using namespace QuickGUI;

MainMenu::MainMenu(PlayPen* e) :
	Window(e,NULL)
{
/*
	mWindow = mGUIManager->getDefaultSheet()->createWindow("SkinSetEditor.MainMenu");
	mWindow->setSize(mGUIManager->getDefaultSheet()->getSize());
	mWindow->setHorizontalAnchor(QuickGUI::Widget::ANCHOR_HORIZONTAL_LEFT_RIGHT);
	mWindow->setVerticalAnchor(QuickGUI::Widget::ANCHOR_VERTICAL_TOP_BOTTOM);
	mWindow->allowResizing(false);
	mWindow->getTitleBar()->setText("Main Menu");
	mWindow->getTitleBar()->enableDragging(false);
	mWindow->getTitleBar()->hideCloseButton();

	QuickGUI::Button* b;

	b = mWindow->createButton("NewSkinSetButton");
	b->setText("New SkinSet...");
	b->setSize(135,25);
	b->setPosition(15,35);

	b = mWindow->createButton("OpenSkinSetButton");
	b->setText("Open SkinSet...");
	b->setSize(135,25);
	b->setPosition(15,65);

	b = mWindow->createButton("SaveSkinSetButton");
	b->setText("Save SkinSet...");
	b->setSize(135,25);
	b->setPosition(15,95);

	b = mWindow->createButton("SaveAsSkinSetButton");
	b->setText("Save SkinSet As...");
	b->setSize(135,25);
	b->setPosition(15,125);

	b = mWindow->createButton("SkinSetSettingsButton");
	b->setText("SkinSet Settings...");
	b->setSize(135,25);
	b->setPosition(15,155);

	b = mWindow->createButton("ExitButton");
	b->setText("Exit");
	b->setSize(135,25);
	b->setPosition(15,185);
	b->addEventHandler(QuickGUI::Widget::EVENT_MOUSE_BUTTON_UP,&MainMenu::onExitPressed,this);
*/
}

MainMenu::~MainMenu()
{
}

void MainMenu::onExitPressed(const QuickGUI::EventArgs& args)
{
	mSkinSetEditor->exit();
}
