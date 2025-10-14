#ifndef QUICKGUILABEL_H
#define QUICKGUILABEL_H

#include "QuickGUIWidget.h"
#include "QuickGUITextUser.h"

namespace QuickGUI
{
	class _QuickGUIExport LabelDesc :
			public WidgetDesc,
			public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		LabelDesc();
		virtual ~LabelDesc() {}
	public:

		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment label_verticalTextAlignment;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "LabelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Label"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Label :
		public Widget,
		public TextUser
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the vertical alignment of text within this widget's client area.
		*/
		VerticalTextAlignment getVerticalTextAlignment();

		/**
		* Sets the Vertical alignment of Text as displayed within the Label area.
		*/
		void setVerticalTextAlignment(VerticalTextAlignment a);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

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

	protected:
		Label(const Ogre::String& name);
		virtual ~Label();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		LabelDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
