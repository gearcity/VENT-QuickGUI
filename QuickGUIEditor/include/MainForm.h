#ifndef MAINFORM_H
#define MAINFORM_H

#include "Window.h"

#include <map>

#include "QuickGUI.h"

using namespace QuickGUI;

namespace QuickGUIEditor
{
	struct OverloadedWidgetAttributes
	{
		bool dragable;
		bool enabled;
		bool moveBaseWidgetOnDrag;
		bool resizeLeft;
		bool resizeRight;
		bool resizeTop;
		bool resizeBottom;
	};

	class MainForm :
		public Window
	{
	public:
		MainForm(QuickGUIEditorApp* e, Ogre::RenderWindow* w);
		virtual ~MainForm();

		Point getScreenPosition(Widget* w);

		virtual bool keyPressed(const OIS::KeyEvent &arg);

		void loadSheet(const Ogre::String& sheetFileName);

		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

		/**
		* Selects a widget, populating PropertyGrid with widget data.
		*/
		void selectWidget(Widget* w);

		virtual void windowClosed(Ogre::RenderWindow* rw);

	protected:
		Sheet* mSheet;
		MouseCursor* mMouseCursor;

		bool mLMBDown;

		/// CONTEXT MENU
		
		void _createContextMenu();
		void hideCreateMenuItems();
		void showCreateMenuItems(Widget* w);

		ContextMenu* mContextMenu;

		MenuTextItem* mCreateButtonItem;
		EventHandlerSlot* mCreateButtonHandler;

		MenuTextItem* mCreateComboBoxItem;
		EventHandlerSlot* mCreateComboBoxHandler;

		MenuTextItem* mCreateConsoleItem;
		EventHandlerSlot* mCreateConsoleHandler;

		MenuTextItem* mCreateContextMenuItem;
		EventHandlerSlot* mCreateContextMenuHandler;

		MenuTextItem* mCreateHScrollBarItem;
		EventHandlerSlot* mCreateHScrollBarHandler;

		MenuTextItem* mCreateImageItem;
		EventHandlerSlot* mCreateImageHandler;

		MenuTextItem* mCreateLabelItem;
		EventHandlerSlot* mCreateLabelHandler;

		MenuTextItem* mCreateListItem;
		EventHandlerSlot* mCreateListHandler;

		MenuTextItem* mCreateListImageItemItem;
		EventHandlerSlot* mCreateListImageItemHandler;

		MenuTextItem* mCreateListTextItemItem;
		EventHandlerSlot* mCreateListTextItemHandler;

		MenuTextItem* mCreateMenuItem;
		EventHandlerSlot* mCreateMenuHandler;

		MenuTextItem* mCreateMenuImageItemItem;
		EventHandlerSlot* mCreateMenuImageItemHandler;

		MenuTextItem* mCreateMenuTextItemItem;
		EventHandlerSlot* mCreateMenuTextItemHandler;

		MenuTextItem* mCreatePanelItem;
		EventHandlerSlot* mCreatePanelHandler;

		MenuTextItem* mCreateProgressBarItem;
		EventHandlerSlot* mCreateProgressBarHandler;

		MenuTextItem* mCreateRadioButtonItem;
		EventHandlerSlot* mCreateRadioButtonHandler;

		MenuTextItem* mCreateTabControlItem;
		EventHandlerSlot* mCreateTabControlHandler;

		MenuTextItem* mCreateTextAreaItem;
		EventHandlerSlot* mCreateTextAreaHandler;

		MenuTextItem* mCreateTextBoxItem;
		EventHandlerSlot* mCreateTextBoxHandler;

		MenuTextItem* mCreateToolBarItem;
		EventHandlerSlot* mCreateToolBarHandler;

		MenuTextItem* mCreateTreeViewItem;
		EventHandlerSlot* mCreateTreeViewHandler;

		MenuTextItem* mCreateVScrollBarItem;
		EventHandlerSlot* mCreateVScrollBarHandler;

		MenuTextItem* mCreateWindowItem;
		EventHandlerSlot* mCreateWindowHandler;

		/// PROPERTY WINDOW

		void _createPropertyWindow();
		void _createPropertyGrid(TabPage* p);

		QuickGUI::Window* mPropertyWindow;

		void onPropertyPressed(const EventArgs& args);
		void onPW_SheetButtonPressed(const EventArgs& args);
		void onPW_DeleteButtonPressed(const EventArgs& args);

		/// SAVE SHEET MODAL WINDOW

		void _createSaveWindow();

		ModalWindow* mSaveWindow;
		TextBox*		mSaveWindowFileName;

		void onSaveWindowSavePressed(const EventArgs& args);
		void onSaveWindowCancelPressed(const EventArgs& args);

		/// LOAD SHEET MODAL WINDOW

		void _createLoadWindow();

		ModalWindow* mLoadWindow;
		TextBox*		mLoadWindowFileName;
		Label*			mLoadWindowErrorMessage;

		void onLoadWindowLoadPressed(const EventArgs& args);
		void onLoadWindowCancelPressed(const EventArgs& args);

		/// OVERWRITE SHEET MODAL WINDOW

		void _createOverwriteWindow();

		ModalWindow* mOverwriteWindow;

		void onOverwriteWindowYesPressed(const EventArgs& args);
		void onOverwriteWindowNoPressed(const EventArgs& args);

		/// END - OVERWRITE SHEET MODAL WINDOW

		Widget* getBaseWidget(Widget* widget);
		bool isEditorWidget(Widget* widget);
		bool areModalDialogsVisible();

		Widget* mSelectedWidget;
		bool mIsVisible;
		template<class T> bool isA(Widget* w)
		{
			return (dynamic_cast<T*>(w) != NULL);
		}

		EventHandlerSlot* mLastExecutedHandler;

		std::map<Widget*,OverloadedWidgetAttributes> mWidgetOverloads;
		void overloadWidgetAttributes(Widget* w);
		void overloadWidgetAttributes();
		void restoreWidgetAttributes();
		void writeSheetToDisk(const Ogre::String& fileName);

		bool mDragable;
		bool mEnabled;
		bool mResizeLeft;
		bool mResizeRight;
		bool mResizeTop;
		bool mResizeBottom;

		// TEXT INPUT VALIDATORS

		bool defaultInputValidator(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText);

		// GUI HANDLERS

		void onCreateButton(const EventArgs& args);
		void onCreateComboBox(const EventArgs& args);
		void onCreateConsole(const EventArgs& args);
		void onCreateContextMenu(const EventArgs& args);
		void onCreateHScrollBar(const EventArgs& args);
		void onCreateImage(const EventArgs& args);
		void onCreateLabel(const EventArgs& args);
		void onCreateList(const EventArgs& args);
		void onCreateListImageItem(const EventArgs& args);
		void onCreateListTextItem(const EventArgs& args);
		void onCreateMenu(const EventArgs& args);
		void onCreateMenuImageItem(const QuickGUI::EventArgs& args);
		void onCreateMenuTextItem(const EventArgs& args);
		void onCreatePanel(const EventArgs& args);
		void onCreateProgressBar(const EventArgs& args);
		void onCreateRadioButton(const EventArgs& args);
		void onCreateTabControl(const EventArgs& args);
		void onCreateTextBox(const EventArgs& args);
		void onCreateTextArea(const EventArgs& args);
		void onCreateToolBar(const EventArgs& args);
		void onCreateTreeView(const EventArgs& args);
		void onCreateVScrollBar(const EventArgs& args);
		void onCreateWindow(const EventArgs& args);

		void onMenuItemEnableChanged(const EventArgs& args);

		void onLoadPressed(const EventArgs& args);
		void onSavePressed(const EventArgs& args);
		void onExitPressed(const EventArgs& args);

		void onDeletePressed(const EventArgs& args);

		void onKeyDownInTextBox(const EventArgs& args);
	};
}

#endif
