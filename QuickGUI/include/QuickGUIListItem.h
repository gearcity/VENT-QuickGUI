#ifndef QUICKGUILISTITEM_H
#define QUICKGUILISTITEM_H

#include "QuickGUIContainerWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport ListItemDesc :
		public ContainerWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		ListItemDesc();
		virtual ~ListItemDesc() {}
	public:

		int listitem_index;
		bool listitem_selected;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "List"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ListItem :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String OVER;
		static const Ogre::String SELECTED;
	public:
		// List has to set positions to manage ListItems
		friend class List;
		// ComboBox has to set positions to manage ListItems
		friend class ComboBox;
		// ListPanel's destructor cleans up any remaining ListItems, requiring access to destructor
		friend class ListPanel;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Queues this widget for destruction, likely to happen in the next frame, before drawing occurs.
		* NOTE: Widgets that are components of another widget cannot be destroyed using this API.
		*/
		virtual void destroy();

		/**
		* Returns true if this widget is able to receive injected input events,
		* false otherwise.
		* NOTE: Disabled widgets can still have events fired manually.
		*/
		bool getEnabled();
		/**
		* Returns the listitem_index of the ListItem in its owner List.
		*/
		int getIndex();
		/**
		* Returns true if the ListItem is listitem_selected, false otherwise.
		*/
		bool getSelected();

		/**
		* Returns true if this widget's position is managed by another widget.
		*/
		virtual bool isPositionManaged();
		/**
		* Returns true if this widget's size is managed by another widget.
		*/
		virtual bool isSizeManaged();

		/**
		* Enabled Widgets receive mouse and keyboard events via injections to the GUIManager.
		* Disabled Widgets can only receive these events if they are manually fired.
		*/
		virtual void setEnabled(bool enabled);

	protected:
		ListItem(const Ogre::String& name);
		virtual ~ListItem();

		ListItemDesc* mDesc;

		void onMouseButtonDown(const EventArgs& args);
		void onMouseEnter(const EventArgs& args);
		void onMouseLeave(const EventArgs& args);

		/**
		* Sets the listitem_index of the ListItem
		*/
		void setIndex(unsigned int index);
		/**
		* Sets whether the ListItem is listitem_selected or not.
		*/
		void setSelected(bool selected);

		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();

	private:
	};
}

#endif
