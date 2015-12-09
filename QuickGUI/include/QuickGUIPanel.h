#ifndef QUICKGUIPANEL_H
#define QUICKGUIPANEL_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIText.h"
#include "QuickGUIForwardDeclaredDesc.h"

namespace QuickGUI
{
	// forward declarations
	class Button;
	class CheckBox;
	class ColorPicker;
	class ComboBox;
	class Console;
	class HScrollBar;
	class Image;
	class Label;
	class List;
	class ProgressBar;
	class PropertyGrid;
	class RadioButton;
	class TabControl;
	class TextArea;
	class TextBox;
	class ToolBar;
	class TreeView;
	class VScrollBar;

	class ButtonDesc;
	class CheckBoxDesc;
	class ColorPickerDesc;
	class ComboBoxDesc;
	class ConsoleDesc;
	class HScrollBarDesc;
	class ImageDesc;
	class LabelDesc;
	class ListDesc;
	class ProgressBarDesc;
	class PropertyGridDesc;
	class RadioButtonDesc;
	class TextAreaDesc;
	class TextBoxDesc;
	class ToolBarDesc;
	class TreeViewDesc;
	class VScrollBarDesc;

	class _QuickGUIExport Panel :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;
		// TabPage uses a Panel internally to represent the page.
		// All child widgets are added to the component Panel, thus it
		// needs access to the protected "addChild" function
		friend class TabPage;

		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Creates a user defined custom widget.
		*/
		Widget* createCustomWidget(const Ogre::String& className, WidgetDesc* d);

		/**
		* Creates a child Button.
		*/
		Button* createButton(ButtonDesc* d);
		Button* createButton();
		Button* createButton(const Point& position);
		Button* createButton(const Rect& dimensions);
		Button* createButton(const Ogre::UTFString& text, const Rect& dimensions);
		Button* createButton(std::vector<TextSegment> segments, const Rect& dimensions);
		/**
		* Creates a child CheckBox.
		*/
		CheckBox* createCheckBox(CheckBoxDesc* d);
		CheckBox* createCheckBox();
		CheckBox* createCheckBox(const Point& position);
		CheckBox* createCheckBox(const Rect& dimensions);
		/**
		* Creates a child ColorPicker.
		*/
		ColorPicker* createColorPicker(ColorPickerDesc* d);
		ColorPicker* createColorPicker();
		ColorPicker* createColorPicker(const Point& position);
		ColorPicker* createColorPicker(const Rect& dimensions);
		/**
		* Creates a child Console.
		*/
		Console* createConsole(ConsoleDesc* d);
		Console* createConsole();
		Console* createConsole(const Point& position);
		Console* createConsole(const Rect& dimensions);
		/**
		* Creates a child ComboBox.
		*/
		ComboBox* createComboBox(ComboBoxDesc* d);
		ComboBox* createComboBox();
		ComboBox* createComboBox(const Point& position);
		ComboBox* createComboBox(const Rect& dimensions);
		/**
		* Creates a child HScrollBar.
		*/
		HScrollBar* createHScrollBar(HScrollBarDesc* d);
		HScrollBar* createHScrollBar();
		HScrollBar* createHScrollBar(const Point& position);
		HScrollBar* createHScrollBar(const Rect& dimensions);
		/**
		* Creates a child Image.
		*/
		Image* createImage(ImageDesc* d);
		Image* createImage();
		Image* createImage(const Point& position);
		Image* createImage(const Rect& dimensions);
		/**
		* Creates a child Label.
		*/
		Label* createLabel(LabelDesc* d);
		Label* createLabel();
		Label* createLabel(const Point& position);
		Label* createLabel(const Rect& dimensions);
		Label* createLabel(const Ogre::UTFString& text, const Point& position);
		Label* createLabel(std::vector<TextSegment> segments, const Point& position);
		Label* createLabel(const Ogre::UTFString& text, const Rect& dimensions);
		Label* createLabel(std::vector<TextSegment> segments, const Rect& dimensions);
		/**
		* Creates a child List.
		*/
		List* createList(ListDesc* d);
		List* createList();
		List* createList(const Point& position);
		List* createList(const Rect& dimensions);
		/**
		* Creates a child Panel.
		*/
		Panel* createPanel(PanelDesc* d);
		Panel* createPanel();
		Panel* createPanel(const Point& position);
		Panel* createPanel(const Rect& dimensions);
		/**
		* Creates a child ProgressBar.
		*/
		ProgressBar* createProgressBar(ProgressBarDesc* d);
		ProgressBar* createProgressBar();
		ProgressBar* createProgressBar(const Point& position);
		ProgressBar* createProgressBar(const Rect& dimensions);
		/**
		* Creates a child PropertyGrid.
		*/
		PropertyGrid* createPropertyGrid(PropertyGridDesc* d);
		PropertyGrid* createPropertyGrid();
		PropertyGrid* createPropertyGrid(const Point& position);
		PropertyGrid* createPropertyGrid(const Rect& dimensions);
		/**
		* Creates a child RadioButton.
		*/
		RadioButton* createRadioButton(RadioButtonDesc* d);
		RadioButton* createRadioButton();
		RadioButton* createRadioButton(const Point& position);
		RadioButton* createRadioButton(const Rect& dimensions);
		/**
		* Creates a child TabControl.
		*/
		TabControl* createTabControl(TabControlDesc* d);
		TabControl* createTabControl();
		TabControl* createTabControl(const Point& position);
		TabControl* createTabControl(const Rect& dimensions);
		/**
		* Creates a child TextArea.
		*/
		TextArea* createTextArea(TextAreaDesc* d);
		TextArea* createTextArea();
		TextArea* createTextArea(const Point& position);
		TextArea* createTextArea(const Rect& dimensions);
		/**
		* Creates a child TextBox.
		*/
		TextBox* createTextBox(TextBoxDesc* d);
		TextBox* createTextBox();
		TextBox* createTextBox(const Point& position);
		TextBox* createTextBox(const Rect& dimensions);
		/**
		* Creates a child ToolBar.
		*/
		ToolBar* createToolBar(ToolBarDesc* d);
		ToolBar* createToolBar();
		ToolBar* createToolBar(const Point& position);
		ToolBar* createToolBar(const Rect& dimensions);
		/**
		* Creates a child TreeView.
		*/
		TreeView* createTreeView(TreeViewDesc* d);
		TreeView* createTreeView();
		TreeView* createTreeView(const Point& position);
		TreeView* createTreeView(const Rect& dimensions);
		/**
		* Creates a child VScrollBar.
		*/
		VScrollBar* createVScrollBar(VScrollBarDesc* d);
		VScrollBar* createVScrollBar();
		VScrollBar* createVScrollBar(const Point& position);
		VScrollBar* createVScrollBar(const Rect& dimensions);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::drag;
		using Widget::resize;
		using Widget::setConsumeKeyboardEvents;
		using Widget::setDimensions;
		using Widget::setDragable;
		using Widget::setHeight;
		using Widget::setHorizontalAnchor;
		using Widget::setMaxSize;
		using Widget::setMinSize;
		using Widget::setPosition;
		using Widget::setPositionRelativeToParentClientDimensions;
		using Widget::setResizeFromAllSides;
		using Widget::setResizeFromBottom;
		using Widget::setResizeFromLeft;
		using Widget::setResizeFromRight;
		using Widget::setResizeFromTop;
		using Widget::setScrollable;
		using Widget::setSerialize;
		using Widget::setSize;
		using Widget::setTransparencyPicking;
		using Widget::setVerticalAnchor;
		using Widget::setVisible;
		using Widget::setWidth;

		using ContainerWidget::destroyChild;		

	protected:
		Panel(const Ogre::String& name);
		virtual ~Panel();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		PanelDesc* mPanelDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
