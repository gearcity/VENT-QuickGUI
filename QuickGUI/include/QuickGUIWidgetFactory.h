#ifndef QUICKGUIWIDGETFACTORY_H
#define QUICKGUIWIDGETFACTORY_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIWidget.h"
#include <unordered_map>

#include "OgrePrerequisites.h"

#include <map>

namespace QuickGUIEditor
{
	// forward declarations
	class MainForm;
}

namespace QuickGUI
{
	class _QuickGUIExport WidgetFactory
	{
	public:
		// Only FactoryManager can create/destroy Factories
		friend class FactoryManager;
		// Make Widgets as Friends, as they will create/destroy Widgets
		friend class ComboBox;
		friend class ComponentWidget;
		friend class ContainerWidget;
		friend class ContextMenu;
		friend class List;
		friend class Menu;
		friend class Sheet;
		friend class TabControl;
		friend class TabPage;
		friend class Widget;
		friend class GUIManager;
		// Editor creates and manages its own Widgets
		friend class QuickGUIEditor::MainForm;
	protected:
		typedef Widget* (WidgetFactory::*createWidgetFunction)(const Ogre::String&);

	public:
		Ogre::String& getNamePrefix()
		{
			return mNamingPrefix;
		}

		template<typename ClassType>
		bool registerClass(const Ogre::String& className)
		{
			if (mFunctorMap.find(className) != mFunctorMap.end())
				return false;

			mFunctorMap[className] = &WidgetFactory::createWidget<ClassType>;

			return true;
		}

		/**
		* Sets the string prepended to every instance created by this factory
		*/
		void setNamePrefix(const Ogre::String& prefix)
		{
			mNamingPrefix = prefix;
		}

		bool unregisterClass(const Ogre::String& className)
		{
			return (mFunctorMap.erase(className) == 1);
		}

	protected:
		template<typename ClassType>
		ClassType* createInstance(const Ogre::String& className, const Ogre::String& name)
		{
#if USEHASHMAPS
			typename std::unordered_map<Ogre::String, createWidgetFunction>::iterator iter = mFunctorMap.find(className);
#else
			typename std::map<Ogre::String, createWidgetFunction>::iterator iter = mFunctorMap.find(className);
#endif
			if (iter == mFunctorMap.end())
				throw Exception(Exception::ERR_FACTORY,"\"" + className + "\" is not a registered class!","WidgetFactory::createInstance");

			Widget* newInstance = (this->*(*iter).second)(mNamingPrefix + name);

			return dynamic_cast<ClassType*>(newInstance);
		}

		void destroyInstance(Widget* instance)
		{
			OGRE_DELETE_T(instance,Widget,Ogre::MEMCATEGORY_GENERAL);
		}

	protected:
		WidgetFactory() : mNamingPrefix("") {}
		virtual ~WidgetFactory() {}

#if USEHASHMAPS
		std::unordered_map<Ogre::String, createWidgetFunction> mFunctorMap;
#else
		std::map<Ogre::String, createWidgetFunction> mFunctorMap;
#endif

		Ogre::String mNamingPrefix;
		
		template<typename ClassType>
		Widget* createWidget(const Ogre::String& param1)
		{
			return OGRE_NEW_T(ClassType,Ogre::MEMCATEGORY_GENERAL)(param1);
		}
	};
}

#endif
