#ifndef QUICKGUISKINDEFINITION_H
#define QUICKGUISKINDEFINITION_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISkinType.h"

#include "OgrePrerequisites.h"

#include <map>
#include <set>

namespace QuickGUI
{
	/**
	* This class outlines the interface of a Skin for a Widget.
	* Each Widget class will have a corresponding SkinDefinition,
	* and each type of this class must follow the same interface.
	* As an example we create a SkinDefinition for the "Button" widget class.
	* Lets say there are 3 button types: oval, square, triangle.  Each Type will need
	* to have the same Skin interface as the Skin Definition.
	*/
	class _QuickGUIExport SkinDefinition
	{
	public:
		SkinDefinition(const Ogre::String& className);

		/**
		* Adds a component entry to this Skin definition.  Components
		* can only be defined while the definition has not been completed.
		*/
		void defineComponent(const Ogre::String& componentName);
		/**
		* Adds a Skin Element entry to this Skin definition. Skin Elements
		* can only be defined while the definition has not been completed.
		*/
		void defineSkinElement(const Ogre::String& elementName);
		/**
		* Locks the definition of this Widget Skin interface, allowing
		* types to be defined.  All types must have the same interface.
		*/
		void definitionComplete();
		/**
		* Returns true if this skin definition is complete, false if the definition
		* is still able to be defined.
		*/
		bool definitionCompleted();

		/**
		* Returns true if the given type shares the same interface as this definition,
		* false otherwise.
		* NOTE: definition must be complete before you can validate it against types.
		*/
		bool validateSkinType(SkinType* t);

	protected:
		Ogre::String mClassName;

		// If this is true, we cannot define any more SkinElements or ComponentTypes to this definition
		bool mDefinitionCompleted;

		std::set<Ogre::String> mDefinedElements;
		std::set<Ogre::String> mDefinedComponents;
	};
}

#endif
