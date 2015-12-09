#ifndef QUICKGUIFACTORYMANAGER_H
#define QUICKGUIFACTORYMANAGER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	// forward declarations
	class WidgetFactory;
	class DescFactory;

	/*
	* The Factory manager manages factories of QuickGUI, including Widget and Desc factories.
	*/
	class _QuickGUIExport FactoryManager :
		public Ogre::Singleton<FactoryManager>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class Root;
	public:
		static FactoryManager& getSingleton(void); 
		static FactoryManager* getSingletonPtr(void);

		/**
		* Returns the Widget Factory.
		*/
		WidgetFactory* getWidgetFactory();
		/**
		* Returns the WidgetDesc Factory.
		*/
		DescFactory* getDescFactory();

	protected:
		FactoryManager();
		virtual ~FactoryManager();

		WidgetFactory* mWidgetFactory;
		DescFactory* mDescFactory;

	private:
	};
}

#endif
