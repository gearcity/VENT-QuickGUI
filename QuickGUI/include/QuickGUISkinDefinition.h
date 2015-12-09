/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

#ifndef QUICKGUISKINDEFINITION_H
#define QUICKGUISKINDEFINITION_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISkinType.h"

#include <map>
#include <set>
#include <string>

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
		SkinDefinition(const std::string& className);

		/**
		* Adds a component entry to this Skin definition.  Components
		* can only be defined while the definition has not been completed.
		*/
		void defineSkinReference(const std::string& refName, const std::string& widgetClass);
		/**
		* Adds a Skin Element entry to this Skin definition. Skin Elements
		* can only be defined while the definition has not been completed.
		*/
		void defineSkinElement(const std::string& elementName);
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
		std::string mClassName;

		// If this is true, we cannot define any more SkinElements or ComponentTypes to this definition
		bool mDefinitionCompleted;

		std::set<std::string> mDefinedElements;
		std::map<std::string,std::string> mDefinedReferences;
	};
}

#endif
