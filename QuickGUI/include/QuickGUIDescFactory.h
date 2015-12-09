#ifndef QUICKGUIDESCFACTORY_H
#define QUICKGUIDESCFACTORY_H

#include "QuickGUIDesc.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"

#include "OgrePrerequisites.h"

#include <map>

namespace QuickGUI
{
	class _QuickGUIExport DescFactory
	{
	public:
		// Only FactoryManager can create/destroy Factories
		friend class FactoryManager;
		// DescManager uses factory to create/destroy Descs
		friend class DescManager;
		// These Widgets need to create/destroy Descs
		friend class Sheet;
		friend class Widget;
	protected:
		typedef Desc* (DescFactory::*createDescFunction)();

	public:
		template<typename ClassType>
		bool registerClass(const Ogre::String& className)
		{
			if (mFunctorMap.find(className) != mFunctorMap.end())
				return false;

			mFunctorMap[className] = &DescFactory::createDesc<ClassType>;

			return true;
		}

		bool unregisterClass(const Ogre::String& className)
		{
			return (mFunctorMap.erase(className) == 1);
		}

	protected:

		template<typename ClassType>
		ClassType* createInstance(const Ogre::String& className)
		{
			typename std::map<Ogre::String, createDescFunction>::iterator iter = mFunctorMap.find(className);

			if (iter == mFunctorMap.end())
				throw Exception(Exception::ERR_FACTORY,"\"" + className + "\" is not a registered class!","DescFactory::createInstance");

			Desc* newInstance = (this->*(*iter).second)();

			return dynamic_cast<ClassType*>(newInstance);
		}

		void destroyInstance(Desc* instance)
		{
			OGRE_DELETE_T(instance,Desc,Ogre::MEMCATEGORY_GENERAL);
		}

	protected:
		DescFactory() {}
		virtual ~DescFactory() {}

		std::map<Ogre::String, createDescFunction> mFunctorMap;
		
		template<typename ClassType>
		Desc* createDesc()
		{
			return OGRE_NEW_T(ClassType,Ogre::MEMCATEGORY_GENERAL)();
		}
	};
}

#endif
