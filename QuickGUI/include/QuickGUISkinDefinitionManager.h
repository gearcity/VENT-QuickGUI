#ifndef QUICKGUISKINDEFINITIONMANAGER_H
#define QUICKGUISKINDEFINITIONMANAGER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISkinTypeManager.h"

#include "OgreSingleton.h"

namespace QuickGUI
{
	// forward declarations
	class SkinDefinition;

	class _QuickGUIExport SkinDefinitionManager :
		public Ogre::Singleton<SkinDefinitionManager>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class Root;
	public:

		static SkinDefinitionManager& getSingleton(void);
		static SkinDefinitionManager* getSingletonPtr(void);

		SkinDefinition* getSkinDefinition(const Ogre::String& className);

		bool hasSkinDefinition(const Ogre::String& className);

		void registerSkinDefinition(const Ogre::String& className, SkinDefinition* d);

	protected:
		SkinDefinitionManager();
		virtual ~SkinDefinitionManager();

		std::map<Ogre::String,SkinDefinition*> mWidgetSkinDefs;
	};
}

#endif
