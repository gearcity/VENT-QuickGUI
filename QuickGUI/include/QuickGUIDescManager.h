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

			Desc* d = FactoryManager::getSingleton().getDescFactory()->template createInstance<Desc>(className);

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

		std::map<Ogre::String,Desc*> mUserCreatedDescs;

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
