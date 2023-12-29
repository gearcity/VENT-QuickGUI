#ifndef QUICKGUISKINTYPEMANAGER_H
#define QUICKGUISKINTYPEMANAGER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISkinType.h"

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

		void addSkinType(const Ogre::String& className, const Ogre::String& typeName, SkinType* t);

		SkinType* getSkinType(const Ogre::String& className, const Ogre::String& typeName);

		bool hasSkinType(const Ogre::String& className, const Ogre::String& typeName);

		void loadTypes();

		void saveTypesToFile(const Ogre::String& fileName);

	protected:
		SkinTypeManager();
		virtual ~SkinTypeManager();

#if USEHASHMAPS
	stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,SkinType*> > mSkinTypes;
#else
	std::map<Ogre::String, std::map<Ogre::String,SkinType*> > mSkinTypes;
#endif
		
	private:
	};
}

#endif
