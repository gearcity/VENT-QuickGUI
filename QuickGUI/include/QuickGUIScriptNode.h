#ifndef QUICKGUISCRIPTNODE_H
#define QUICKGUISCRIPTNODE_H

#include "QuickGUIExportDLL.h"

#include "OgrePrerequisites.h"
#include "OgreStringVector.h"

namespace QuickGUI
{
	class _QuickGUIExport ScriptNode
	{
	public:
		ScriptNode(ScriptNode* parent, const Ogre::String& name = "untitled");
		~ScriptNode();

		ScriptNode* addChild(const Ogre::String& name = "untitled");
		void addValue(const std::string& value);

		void clearValues();

		ScriptNode* findChild(const Ogre::String& name, bool recursive = false);

		std::vector<ScriptNode*>& getChildren();
		Ogre::String getName();
		ScriptNode* getParent();
		ScriptNode* getSibling();
		Ogre::StringVector& getValues();

		void setName(const Ogre::String& name);
		void setParent(ScriptNode* newParent);

	protected:
	private:
		std::string name;
		Ogre::StringVector values;
		std::vector<ScriptNode*> children;
		ScriptNode* parent;

		bool _removeSelf;
	};
}

#endif
