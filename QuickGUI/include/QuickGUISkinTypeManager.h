#ifndef QUICKGUISKINTYPEMANAGER_H
#define QUICKGUISKINTYPEMANAGER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISkinType.h"
#include <unordered_map>

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	class _QuickGUIExport SkinTypeManager :
		public Ogre::Singleton<SkinTypeManager>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class SkinDefinitionManager;
	public:

		static SkinTypeManager& getSingleton(void);
		static SkinTypeManager* getSingletonPtr(void);

		void addSkinType(const std::string& className, const std::string& typeName, SkinType* t);

		SkinType* getSkinType(const std::string& className, const std::string& typeName);

		bool hasSkinType(const std::string& className, const std::string& typeName);

		void loadTypes();

		void saveTypesToFile(const std::string& fileName);

	protected:
		SkinTypeManager();
		virtual ~SkinTypeManager();

#if USEHASHMAPS
	std::unordered_map<std::string, std::unordered_map<std::string,SkinType*> > mSkinTypes;
#else
	std::map<std::string, std::map<std::string,SkinType*> > mSkinTypes;
#endif
		
	private:
	};
}

#endif
