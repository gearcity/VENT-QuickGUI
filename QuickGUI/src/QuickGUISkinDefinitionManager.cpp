#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"

template<> QuickGUI::SkinDefinitionManager* Ogre::Singleton<QuickGUI::SkinDefinitionManager>::ms_Singleton = 0;

namespace QuickGUI
{
	SkinDefinitionManager::SkinDefinitionManager()
	{
		OGRE_NEW SkinTypeManager();
	}

	SkinDefinitionManager::~SkinDefinitionManager()
	{
		for(std::map<Ogre::String,SkinDefinition*>::iterator it = mWidgetSkinDefs.begin(); it != mWidgetSkinDefs.end(); ++it)
			OGRE_DELETE_T((*it).second,SkinDefinition,Ogre::MEMCATEGORY_GENERAL);

		OGRE_DELETE SkinTypeManager::getSingletonPtr();
	}

	SkinDefinitionManager* SkinDefinitionManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	SkinDefinitionManager& SkinDefinitionManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	SkinDefinition* SkinDefinitionManager::getSkinDefinition(const Ogre::String& className)
	{
		if(!hasSkinDefinition(className))
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition does not exist for class \"" + className + "\"!","SkinDefinitionManager::getSkinDefinition");

		return mWidgetSkinDefs[className];
	}

	bool SkinDefinitionManager::hasSkinDefinition(const Ogre::String& className)
	{
		return (mWidgetSkinDefs.find(className) != mWidgetSkinDefs.end());
	}

	void SkinDefinitionManager::registerSkinDefinition(const Ogre::String& className, SkinDefinition* d)
	{
		if(hasSkinDefinition(className))
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition already exists for class \"" + className + "\"!","SkinDefinition::registerSkinDefinition");

		mWidgetSkinDefs[className] = d;
	}
}
