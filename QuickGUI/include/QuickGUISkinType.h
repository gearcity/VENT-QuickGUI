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
		SkinType(const Ogre::String& className, const Ogre::String& typeName);
		~SkinType();

		/**
		* Adds a component Type to the definition.
		*/
		void addSkinReference(const Ogre::String& componentAlias, SkinReference* t);
		/**
		* Adds a Skin Element to the definition.
		*/
		void addSkinElement(const Ogre::String& elementName, SkinElement* e);

		/**
		* Returns the name of the class this SkinType is intended for.
		*/
		Ogre::String getClassName();
		/**
		* Gets a component Type from the definition.
		*/
		SkinReference* getSkinReference(const Ogre::String& componentAlias);
		/**
		* Returns the name of this Type.
		*/
		Ogre::String getName();
		/**
		* Gets a Skin Element from the definition.
		*/
		SkinElement* getSkinElement(const Ogre::String& elementName);

		/**
		* Returns true if this Type definition has a listing for the component alias, false otherwise.
		*/
		bool hasSkinReference(const Ogre::String& componentAlias);
		/**
		* Returns true if this Type definition has a listing for the element name, false otherwise.
		*/
		bool hasSkinElement(const Ogre::String& elementName);

		/**
		* Builds the SkinElement from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);

	protected:
		Ogre::String mName;
		Ogre::String mClassName;

#if USEHASHMAPS
	// Element Alias/SkinElement pair
		std::unordered_map<Ogre::String,SkinElement*> mSkinElements;
		// ComponentName/SkinReference pair
		std::unordered_map<Ogre::String,SkinReference*> mSkinReferences;
#else
	// Element Alias/SkinElement pair
		std::map<Ogre::String,SkinElement*> mSkinElements;
		// ComponentName/SkinReference pair
		std::map<Ogre::String,SkinReference*> mSkinReferences;
#endif
		
	};
}

#endif
