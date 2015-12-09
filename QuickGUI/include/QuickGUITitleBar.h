#ifndef QUICKGUITITLEBAR_H
#define QUICKGUITITLEBAR_H

#include "QuickGUIComponentWidget.h"
#include "QuickGUITextUser.h"

namespace QuickGUI
{
	// forward declaration
	class Button;

	class _QuickGUIExport TitleBarDesc :
		public ComponentWidgetDesc,
		public TextUserDesc
	{
	public:
		friend class DescFactory;
	protected:
		TitleBarDesc();
		virtual ~TitleBarDesc() {}
	public:

		bool titlebar_closeButton;
		float titlebar_closeButtonPadding;
		Ogre::String titlebar_closeButtonSkinType;

		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment titlebar_verticalTextAlignment;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TitleBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TitleBar"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();
	};

	class TitleBar :
		public ComponentWidget,
		public TextUser
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String CLOSE_BUTTON;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;
		friend class Window;
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

	protected:
		TitleBar(const Ogre::String& name);
		virtual ~TitleBar();

		Button* mCloseButton;

		TitleBarDesc* mDesc;

		int mHeightBuffer;

		/**
		* Hides the window.  Only relevant if this TitleBar has a close button.
		*/
		void onCloseButtonUp(const EventArgs& args);
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
