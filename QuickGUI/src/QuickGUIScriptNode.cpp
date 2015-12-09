#include "QuickGUIScriptNode.h"

namespace QuickGUI
{
	ScriptNode::ScriptNode(ScriptNode* parent, const Ogre::String& name)
	{
		ScriptNode::name = name;
		ScriptNode::parent = parent;
		_removeSelf = true;	//For proper destruction

		//Add self to parent's child list (unless this is the root node being created)
		if (parent != NULL){
			parent->children.push_back(this);
		}
	}

	ScriptNode::~ScriptNode()
	{
		//Delete all children
		std::vector<ScriptNode*>::iterator i;
		for (i = children.begin(); i != children.end(); i++){
			ScriptNode* node = *i;
			node->_removeSelf = false;
			delete node;
		}
		children.clear();

		//Remove self from parent's child list
		if (_removeSelf && parent != NULL)
		{
			for(std::vector<ScriptNode*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it) 
			{ 
				if((*it) == this) 
				{ 
					parent->children.erase(it); 
					break; 
				} 
			}
		}
	}

	ScriptNode* ScriptNode::addChild(const Ogre::String& name)
	{
		return new ScriptNode(this, name);
	}

	void ScriptNode::addValue(const Ogre::String& value)
	{
		values.push_back(value);
	}

	void ScriptNode::clearValues()
	{
		values.clear();
	}

	ScriptNode* ScriptNode::findChild(const Ogre::String &name, bool recursive)
	{
		std::vector<ScriptNode*>::iterator i;

		//Search for node
		for (i = children.begin(); i != children.end(); i++){
			ScriptNode* node = *i;
			if (node->name == name)
				return node;
		}

		//If not found, search child nodes (if recursive == true)
		if (recursive){
			for (i = children.begin(); i != children.end(); i++){
				(*i)->findChild(name, recursive);
			}
		}

		//Not found anywhere
		return NULL;
	}

	std::vector<ScriptNode*>& ScriptNode::getChildren()
	{
		return children;
	}

	Ogre::String ScriptNode::getName()
	{
		return name;
	}

	ScriptNode* ScriptNode::getParent()
	{
		return parent;
	}

	ScriptNode* ScriptNode::getSibling()
	{
		if(parent == NULL)
			return NULL;

		bool found = false;
		std::vector<ScriptNode*> pChildren = parent->getChildren();
		for(std::vector<ScriptNode*>::iterator it = pChildren.begin(); it != pChildren.end(); ++it)
		{
			if(found)
				return (*it);

			// Compare by pointers and not name, 
			// to gaurd against separate lines that start with the same name
			if((*it) == this)
				found = true;
		}

		return NULL;
	}

	Ogre::StringVector& ScriptNode::getValues()
	{
		return values;
	}

	void ScriptNode::setName(const Ogre::String& name)
	{
		this->name = name;
	}

	void ScriptNode::setParent(ScriptNode* newParent)
	{
		//Remove self from current parent
		for(std::vector<ScriptNode*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it) 
		{ 
			if((*it) == this) 
			{ 
				parent->children.erase(it); 
				break; 
			} 
		}

		//Set new parent
		parent = newParent;

		//Add self to new parent
		parent->children.push_back(this);
	}
}
