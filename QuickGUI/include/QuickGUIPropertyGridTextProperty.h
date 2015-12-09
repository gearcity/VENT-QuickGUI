#ifndef QUICKGUIPROPERTYGRIDTEXTPROPERTY_H
#define QUICKGUIPROPERTYGRIDTEXTPROPERTY_H

#include "QuickGUIPropertyGridItem.h"

namespace QuickGUI
{
	// forward declarations
	class TextBox;

	class _QuickGUIExport PropertyGridTextPropertyDesc :
		public PropertyGridItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		PropertyGridTextPropertyDesc();
		virtual ~PropertyGridTextPropertyDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridTextPropertyDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridTextProperty"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridTextProperty :
		public PropertyGridItem
	{
	public:
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// TreeView has to set positions, toggle visibility to manage ListItems
		friend class TreeView;
		// Section updates Property TreeView pointer
		friend class PropertyGridSection;

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

	protected:
		PropertyGridTextProperty(const Ogre::String& name);
		virtual ~PropertyGridTextProperty();

		PropertyGridTextPropertyDesc* mDesc;

		TextBox* mTextBox;

		virtual void updateFieldSkin();

	private:
	};
}

#endif
