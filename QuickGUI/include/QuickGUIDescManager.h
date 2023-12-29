#ifndef QUICKGUIDESCMANAGER_H
#define QUICKGUIDESCMANAGER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIDescFactory.h"

#include "OgreSingleton.h"
#include "OgrePrerequisites.h"

#include <map>

namespace QuickGUI
{
	// forward declarations
	class Desc;
	class ButtonDesc;
	class CheckBoxDesc;
	class ColorPickerDesc;
	class ComboBoxDesc;
	class ConsoleDesc;
	class ContextMenuDesc;
	class HScrollBarDesc;
	class ImageDesc;
	class LabelDesc;
	class ListDesc;
	class ListImageItemDesc;
	class ListPanelItemDesc;
	class ListTextItemDesc;
	class MenuDesc;
	class MenuImageItemDesc;
	class MenuTextItemDesc;
	class MenuPanelDesc;
	class ModalWindowDesc;
	class PanelDesc;
	class ProgressBarDesc;
	class PropertyGridDesc;
	class PropertyGridSectionDesc;
	class PropertyGridTextPropertyDesc;
	class PropertyGridBoolPropertyDesc;
	class PropertyGridComboBoxPropertyDesc;
	class RadioButtonDesc;
	class SheetDesc;
	class TabControlDesc;
	class TabDesc;
	class TabPageDesc;
	class TextAreaDesc;
	class TextBoxDesc;
	class TitleBarDesc;
	class ToolBarDesc;
	class TreeViewDesc;
	class TreeViewNodeDesc;
	class TreeViewCheckBoxNodeDesc;
	class TreeViewRadioButtonNodeDesc;
	class VScrollBarDesc;
	class WindowDesc;

	/*
	* This manager manages user-defined Event Handlers.
	*/
	class _QuickGUIExport DescManager :
		public Ogre::Singleton<DescManager>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class Root;
	public:
		static DescManager& getSingleton(void); 
		static DescManager* getSingletonPtr(void);

		template<class T>
		T* createDesc(const Ogre::String& className, const Ogre::String& id)
		{
			if(mUserCreatedDescs.find(id) != mUserCreatedDescs.end())
				throw Exception(Exception::ERR_FACTORY,"Cannot create Desc with id \"" + id + "\", it already exists!","DescManager::createDesc");

			Desc* d = FactoryManager::getSingleton().getDescFactory()->createInstance<Desc>(className);

			mUserCreatedDescs[id] = d;

			return dynamic_cast<T*>(d);
		}

		void destroyDesc(Desc* d);

		template<class T>
		T* getDesc(const Ogre::String id)
		{
			if(mUserCreatedDescs.find(id) == mUserCreatedDescs.end())
				throw Exception(Exception::ERR_FACTORY,"Cannot get Desc with id \"" + id + "\", it does not exist!","DescManager::getDesc");

			return dynamic_cast<T*>(mUserCreatedDescs[id]);
		}

		ButtonDesc* getDefaultButtonDesc();
		CheckBoxDesc* getDefaultCheckBoxDesc();
		ColorPickerDesc* getDefaultColorPickerDesc();
		ComboBoxDesc* getDefaultComboBoxDesc();
		ConsoleDesc* getDefaultConsoleDesc();
		ContextMenuDesc* getDefaultContextMenuDesc();
		HScrollBarDesc* getDefaultHScrollBarDesc();
		ImageDesc* getDefaultImageDesc();
		LabelDesc* getDefaultLabelDesc();
		ListDesc* getDefaultListDesc();
		ListImageItemDesc* getDefaultListImageItemDesc();
		ListPanelItemDesc* getDefaultListPanelItemDesc();
		ListTextItemDesc* getDefaultListTextItemDesc();
		MenuDesc* getDefaultMenuDesc();
		MenuImageItemDesc* getDefaultMenuImageItemDesc();
		MenuTextItemDesc* getDefaultMenuTextItemDesc();
		MenuPanelDesc* getDefaultMenuPanelDesc();
		ModalWindowDesc* getDefaultModalWindowDesc();
		PanelDesc* getDefaultPanelDesc();
		ProgressBarDesc* getDefaultProgressBarDesc();
		PropertyGridDesc* getDefaultPropertyGridDesc();
		PropertyGridSectionDesc* getDefaultPropertyGridSectionDesc();
		PropertyGridTextPropertyDesc* getDefaultPropertyGridTextPropertyDesc();
		PropertyGridBoolPropertyDesc* getDefaultPropertyGridBoolPropertyDesc();
		PropertyGridComboBoxPropertyDesc* getDefaultPropertyGridComboBoxPropertyDesc();
		RadioButtonDesc* getDefaultRadioButtonDesc();
		SheetDesc* getDefaultSheetDesc();
		TabControlDesc* getDefaultTabControlDesc();
		TabDesc* getDefaultTabDesc();
		TabPageDesc* getDefaultTabPageDesc();
		TextAreaDesc* getDefaultTextAreaDesc();
		TextBoxDesc* getDefaultTextBoxDesc();
		TitleBarDesc* getDefaultTitleBarDesc();
		ToolBarDesc* getDefaultToolBarDesc();
		TreeViewDesc* getDefaultTreeViewDesc();
		TreeViewNodeDesc* getDefaultTreeViewNodeDesc();
		TreeViewCheckBoxNodeDesc* getDefaultTreeViewCheckBoxNodeDesc();
		TreeViewRadioButtonNodeDesc* getDefaultTreeViewRadioButtonNodeDesc();
		VScrollBarDesc* getDefaultVScrollBarDesc();
		WindowDesc* getDefaultWindowDesc();

	protected:
		DescManager();
		virtual ~DescManager();

#if USEHASHMAPS
		stdext::hash_map<Ogre::String,Desc*> mUserCreatedDescs;
#else
		std::map<Ogre::String,Desc*> mUserCreatedDescs;
#endif

		ButtonDesc* mDefaultButtonDesc;
		CheckBoxDesc* mDefaultCheckBoxDesc;
		ColorPickerDesc* mDefaultColorPickerDesc;
		ComboBoxDesc* mDefaultComboBoxDesc;
		ConsoleDesc* mDefaultConsoleDesc;
		ContextMenuDesc* mDefaultContextMenuDesc;
		HScrollBarDesc* mDefaultHScrollBarDesc;
		ImageDesc* mDefaultImageDesc;
		LabelDesc* mDefaultLabelDesc;
		ListDesc* mDefaultListDesc;
		ListImageItemDesc* mDefaultListImageItemDesc;
		ListPanelItemDesc* mDefaultListPanelItemDesc;
		ListTextItemDesc* mDefaultListTextItemDesc;
		MenuDesc* mDefaultMenuDesc;
		MenuImageItemDesc* mDefaultMenuImageItemDesc;
		MenuTextItemDesc* mDefaultMenuTextItemDesc;
		MenuPanelDesc* mDefaultMenuPanelDesc;
		ModalWindowDesc* mDefaultModalWindowDesc;
		PanelDesc* mDefaultPanelDesc;
		ProgressBarDesc* mDefaultProgressBarDesc;
		PropertyGridDesc* mDefaultPropertyGridDesc;
		PropertyGridSectionDesc* mDefaultPropertyGridSectionDesc;
		PropertyGridTextPropertyDesc* mDefaultPropertyGridTextPropertyDesc;
		PropertyGridBoolPropertyDesc* mDefaultPropertyGridBoolPropertyDesc;
		PropertyGridComboBoxPropertyDesc* mDefaultPropertyGridComboBoxPropertyDesc;
		RadioButtonDesc* mDefaultRadioButtonDesc;
		SheetDesc* mDefaultSheetDesc;
		TabControlDesc* mDefaultTabControlDesc;
		TabDesc* mDefaultTabDesc;
		TabPageDesc* mDefaultTabPageDesc;
		TextAreaDesc* mDefaultTextAreaDesc;
		TextBoxDesc* mDefaultTextBoxDesc;
		TitleBarDesc* mDefaultTitleBarDesc;
		ToolBarDesc* mDefaultToolBarDesc;
		TreeViewDesc* mDefaultTreeViewDesc;
		TreeViewNodeDesc* mDefaultTreeViewNodeDesc;
		TreeViewCheckBoxNodeDesc* mDefaultTreeViewCheckBoxNodeDesc;
		TreeViewRadioButtonNodeDesc* mDefaultTreeViewRadioButtonNodeDesc;
		VScrollBarDesc* mDefaultVScrollBarDesc;
		WindowDesc* mDefaultWindowDesc;

	private:
	};
}

#endif
