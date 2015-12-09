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

#ifndef QUICKGUI_H
#define QUICKGUI_H

#include "QuickGUIAnchors.h"
#include "QuickGUIBorderEnums.h"
#include "QuickGUIBrush.h"
#include "QuickGUIBrushEnums.h"
#include "QuickGUIButton.h"
#include "QuickGUICharacter.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIColorPicker.h"
#include "QuickGUIComboBox.h"
#include "QuickGUISkinReference.h"
#include "QuickGUIComponentWidget.h"
#include "QuickGUIConsole.h"
#include "QuickGUIConsoleEnums.h"
#include "QuickGUIConsoleInputHandler.h"
#include "QuickGUIContainerWidget.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUIDefinitionProperty.h"
#include "QuickGUIDesc.h"
#include "QuickGUIDescFactory.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIEventArgs.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIEventHandlerPointer.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIForwardDeclaredDesc.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIImage.h"
#include "QuickGUIKeyCode.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIListItem.h"
#include "QuickGUIListImageItem.h"
#include "QuickGUIListPanelItem.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUIManager.h"
#include "QuickGUIMenu.h"
#include "QuickGUIMenuImageItem.h"
#include "QuickGUIMenuItem.h"
#include "QuickGUIMenuTextItem.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIModalWindow.h"
#include "QuickGUIMouseButtonID.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUIPanel.h"
#include "QuickGUIPoint.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUIProgressBarEnums.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUIPropertyGridBoolProperty.h"
#include "QuickGUIPropertyGridComboBoxProperty.h"
#include "QuickGUIPropertyGridItem.h"
#include "QuickGUIPropertyGridSection.h"
#include "QuickGUIPropertyGridTextProperty.h"
#include "QuickGUIRadioButton.h"
#include "QuickGUIRect.h"
#include "QuickGUIRoot.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIScriptToken.h"
#include "QuickGUIScriptWriter.h"
#include "QuickGUIScrollBarButtonLayouts.h"
#include "QuickGUISheetManager.h"
#include "QuickGUISerialBase.h"
#include "QuickGUISerializable.h"
#include "QuickGUISerialReader.h"
#include "QuickGUISerialWriter.h"
#include "QuickGUISheet.h"
#include "QuickGUISize.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinElement.h"
#include "QuickGUISkinType.h"
#include "QuickGUISkinTypeManager.h"
#include "QuickGUIStringConverter.h"
#include "QuickGUITab.h"
#include "QuickGUITabControl.h"
#include "QuickGUITabPage.h"
#include "QuickGUIText.h"
#include "QuickGUITextAlignment.h"
#include "QuickGUITextArea.h"
#include "QuickGUITextBox.h"
#include "QuickGUITextCursor.h"
#include "QuickGUITextInputValidator.h"
#include "QuickGUITextLine.h"
#include "QuickGUITimer.h"
#include "QuickGUITimerManager.h"
#include "QuickGUITitleBar.h"
#include "QuickGUIToolBar.h"
#include "QuickGUIToolBarEnums.h"
#include "QuickGUIToolBarItem.h"
#include "QuickGUITreeView.h"
#include "QuickGUITreeViewCheckBoxNode.h"
#include "QuickGUITreeViewNode.h"
#include "QuickGUITreeViewRadioButtonNode.h"
#include "QuickGUIUVRect.h"
#include "QuickGUIVertex.h"
#include "QuickGUIVScrollBar.h"
#include "QuickGUIWidget.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIWindow.h"

#endif
