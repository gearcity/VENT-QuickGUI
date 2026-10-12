#ifndef QUICKGUISKINTYPE_H
#define QUICKGUISKINTYPE_H

#include "QuickGUISkinReference.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISerializable.h"
#include "QuickGUISkinElement.h"
#include <unordered_map>

#include "OgrePrerequisites.h"

#include <map>

namespace QuickGUI
{
	/**
	* This class represents the skin definition of a "type" for a widget.
	* For example, you could have many different button types: round, square, star, triangle.
	* Each type would have a different appearance, each of which would be represented by an
	* instance of this class.
	*/
	class _QuickGUIExport SkinType :
		public Serializable
	{
	public:
		SkinType(const std::string& className, const std::string& typeName);
		~SkinType();

		/**
		* Adds a component Type to the definition.
		*/
		void addSkinReference(const std::string& componentAlias, SkinReference* t);
		/**
		* Adds a Skin Element to the definition.
		*/
		void addSkinElement(const std::string& elementName, SkinElement* e);

		/**
		* Returns the name of the class this SkinType is intended for.
		*/
		std::string getClassName();
		/**
		* Gets a component Type from the definition.
		*/
		SkinReference* getSkinReference(const std::string& componentAlias);
		/**
		* Returns the name of this Type.
		*/
		std::string getName();
		/**
		* Gets a Skin Element from the definition.
		*/
		SkinElement* getSkinElement(const std::string& elementName);

		/**
		* Returns true if this Type definition has a listing for the component alias, false otherwise.
		*/
		bool hasSkinReference(const std::string& componentAlias);
		/**
		* Returns true if this Type definition has a listing for the element name, false otherwise.
		*/
		bool hasSkinElement(const std::string& elementName);

		/**
		* Builds the SkinElement from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);

	protected:
		std::string mName;
		std::string mClassName;

#if USEHASHMAPS
	// Element Alias/SkinElement pair
		std::unordered_map<std::string,SkinElement*> mSkinElements;
		// ComponentName/SkinReference pair
		std::unordered_map<std::string,SkinReference*> mSkinReferences;
#else
	// Element Alias/SkinElement pair
		std::map<std::string,SkinElement*> mSkinElements;
		// ComponentName/SkinReference pair
		std::map<std::string,SkinReference*> mSkinReferences;
#endif
		
	};
}

#endif
