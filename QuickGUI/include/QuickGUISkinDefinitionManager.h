#ifndef QUICKGUISKINDEFINITIONMANAGER_H
#define QUICKGUISKINDEFINITIONMANAGER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISkinTypeManager.h"

#include "OgreSingleton.h"
#include <unordered_map>

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

		SkinDefinition* getSkinDefinition(const std::string& className);

		bool hasSkinDefinition(const std::string& className);

		void registerSkinDefinition(const std::string& className, SkinDefinition* d);

	protected:
		SkinDefinitionManager();
		virtual ~SkinDefinitionManager();

#if USEHASHMAPS
	std::unordered_map<std::string,SkinDefinition*> mWidgetSkinDefs;
#else
	std::map<std::string,SkinDefinition*> mWidgetSkinDefs;
#endif
		
	};
}

#endif
