#ifndef MAINFORM_H
#define MAINFORM_H

#include "Window.h"

class MainForm :
	public Window
{
public:
	MainForm(PlayPen* e, Ogre::RenderWindow* w);
	virtual ~MainForm();

	virtual void windowClosed(Ogre::RenderWindow* rw);

protected:
	void onExitPressed(const QuickGUI::EventArgs& args);

	void test1(const QuickGUI::EventArgs& args);
	void test2(const QuickGUI::EventArgs& args);

	QuickGUI::Sheet* mSheetFromFile;

	void testConsoleHandler(QuickGUI::Console* c, bool& clearInputBox, bool& addToDisplayArea);

private:
	//QuickGUI::Window* mMainMenu;
	//void _createMainMenu();
};

#endif
