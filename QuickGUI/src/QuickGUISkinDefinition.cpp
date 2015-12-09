#include "QuickGUISkinDefinition.h"

namespace QuickGUI
{
	SkinDefinition::SkinDefinition(const Ogre::String& className) :
		mClassName(className),
		mDefinitionCompleted(false)
	{
	}

	void SkinDefinition::defineComponent(const Ogre::String& componentName)
	{
		if(mDefinitionCompleted)
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition has already been finalized, cannot add more references!","SkinDefinition::defineComponent");

		if(mDefinedComponents.find(componentName) != mDefinedComponents.end())
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition already contains reference for Component \"" + componentName + "\"!","SkinDefinition::defineComponent");

		mDefinedComponents.insert(componentName);
	}

	void SkinDefinition::defineSkinElement(const Ogre::String& elementName)
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
		for(std::set<Ogre::String>::iterator it = mDefinedElements.begin(); it != mDefinedElements.end(); ++it)
		{
			if(!t->hasSkinElement((*it)))
				return false;
		}

		for(std::set<Ogre::String>::iterator it = mDefinedComponents.begin(); it != mDefinedComponents.end(); ++it)
		{
			if(!t->hasSkinReference((*it)))
				return false;
		}

		return true;
	}
}
