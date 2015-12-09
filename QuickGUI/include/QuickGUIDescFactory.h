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
