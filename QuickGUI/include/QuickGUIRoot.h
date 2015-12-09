#ifndef QUICKGUIROOT_H
#define QUICKGUIROOT_H

#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIManager.h"

#include "OgreFontManager.h"
#include "OgrePrerequisites.h"
#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	// forward declarations
	class Sheet;

	class _QuickGUIExport Root :
		public Ogre::Singleton<Root>
	{
	public:
		friend class Sheet;
		// Needs access to first available GUIManager
		friend class ComboBox;
		friend class ContainerWidget;
		friend class ContextMenu;
		friend class List;
		friend class ListItem;
		friend class Menu;
		friend class MenuItem;
		friend class TabControl;
		friend class ToolBarItem;
		friend class Widget;
	public:
		Root();
		~Root();

		static Root& getSingleton(void); 
		static Root* getSingletonPtr(void);

		GUIManager* createGUIManager(GUIManagerDesc& d);

		void destroyGUIManager(GUIManager* gm);
		void destroyGUIManager(const std::string& name);

		GUIManager* getGUIManager(const std::string& name);
		/**
		* Gets the default color used by widgets on initialization.
		*/
		ColourValue getDefaultColor();
		/**
		* By default, the name of the first font found on initialization of Root is returned.
		* The default font name can be changed using "setDefaultFontName"
		*/
		Ogre::String getDefaultFontName();
		/**
		* Get the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired.
		*/
		float getDefaultHoverTime();

		/**
		* Sets the default color used by widgets on initialization.
		*/
		void setDefaultColor(const ColourValue& cv);
		/**
		* Sets the default font used by widgets on initialization.
		*/
		void setDefaultFontName(const Ogre::String& fontName);
		/**
		* Set the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired. (Commonly used for tool tips)
		*/
		void setDefaultHoverTime(float seconds);

	protected:
		ColourValue mDefaultColor;
		Ogre::FontPtr mDefaultFont;

		int mGUIManagerCounter;

		std::map<std::string,GUIManager*> mGUIManagers;

		void notifyWidgetDestroyed(Widget* w);
		void notifySheetDestroyed(Sheet* sheet);

		// The amount of time the cursor has to hover over a widget before
		// the ON_HOVER event is fired. (3 seconds by default)
		float mDefaultHoverTime;
	};
}

#endif
