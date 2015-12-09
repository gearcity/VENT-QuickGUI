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

#include "QuickGUISkinDefinition.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIWidgetFactory.h"

namespace QuickGUI
{
	SkinDefinition::SkinDefinition(const std::string& className) :
		mClassName(className),
		mDefinitionCompleted(false)
	{
	}

	void SkinDefinition::defineSkinReference(const std::string& refName, const std::string& widgetClass)
	{
		if(mDefinitionCompleted)
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition has already been finalized, cannot add more references!","SkinDefinition::defineSkinReference");

		if(mDefinedReferences.find(refName) != mDefinedReferences.end())
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition already contains reference for Component \"" + refName + "\"!","SkinDefinition::defineSkinReference");

		if(!FactoryManager::getSingletonPtr()->getWidgetFactory()->classRegistered(widgetClass))
			throw Exception(Exception::ERR_SKINNING,"Widget Class \"" + widgetClass + "\" is not recognized! Did you forget to register it with the Widget Factory?","SkinDefinition::defineSkinReference");

		mDefinedReferences[refName] = widgetClass;
	}

	void SkinDefinition::defineSkinElement(const std::string& elementName)
	{
		if(mDefinitionCompleted)
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition has already been finalized, cannot add more references!","SkinDefinition::defineSkinElement");

		if(mDefinedElements.find(elementName) != mDefinedElements.end())
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition already contains reference for SkinElement \"" + elementName + "\"!","SkinDefinition::defineSkinElement");

		mDefinedElements.insert(elementName);
	}

	void SkinDefinition::definitionComplete()
	{
		mDefinitionCompleted = true;
	}

	bool SkinDefinition::definitionCompleted()
	{
		return mDefinitionCompleted;
	}

	bool SkinDefinition::validateSkinType(SkinType* t)
	{
		for(std::set<std::string>::iterator it = mDefinedElements.begin(); it != mDefinedElements.end(); ++it)
		{
			if(!t->hasSkinElement((*it)))
				return false;
		}

		for(std::map<std::string,std::string>::iterator it = mDefinedReferences.begin(); it != mDefinedReferences.end(); ++it)
		{
			if(!t->hasSkinReference((*it).first,(*it).second))
				return false;
		}

		return true;
	}
}
