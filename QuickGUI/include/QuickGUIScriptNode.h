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
		ScriptNode(ScriptNode* parent, const std::string& name = "untitled");
		~ScriptNode();

		ScriptNode* addChild(const std::string& name = "untitled");
		void addValue(const std::string& value);

		void clearValues();

		ScriptNode* findChild(const std::string& name, bool recursive = false);

		std::vector<ScriptNode*>& getChildren();
		std::string getName();
		ScriptNode* getParent();
		ScriptNode* getSibling();
		std::vector<std::string>& getValues();

		void setName(const std::string& name);
		void setParent(ScriptNode* newParent);

	protected:
	private:
		std::string name;
		std::vector<std::string> values;
		std::vector<ScriptNode*> children;
		ScriptNode* parent;

		bool _removeSelf;
	};
}

#endif
