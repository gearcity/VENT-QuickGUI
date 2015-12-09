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

#ifndef QUICKGUISKINTYPE_H
#define QUICKGUISKINTYPE_H

#include "QuickGUISkinReference.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISerializable.h"
#include "QuickGUISkinElement.h"

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
		virtual ~SkinType();

		/**
		* Adds a Skin Reference to the definition.
		*/
		void addSkinReference(SkinReference* t);
		/**
		* Adds a Skin Element to the definition.
		*/
		void addSkinElement(SkinElement* e);

		/**
		* Returns the name of the class this SkinType is intended for.
		*/
		Ogre::String getClassName();
		/**
		* Gets a component Type from the definition.
		*/
		SkinReference* getSkinReference(const Ogre::String& skinRefID);
		/**
		* Returns the name of this Type.
		*/
		Ogre::String getName();
		/**
		* Gets a Skin Element from the definition.
		*/
		SkinElement* getSkinElement(const Ogre::String& elementName);

		/**
		* Returns true if this Type definition has a listing for the skinRef ID with the class given, false otherwise.
		*/
		bool hasSkinReference(const Ogre::String& skinRefID, const Ogre::String& className);
		/**
		* Returns true if this Type definition has a listing for the skinRef ID, false otherwise.
		*/
		bool hasSkinReference(const Ogre::String& skinRefID);
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

		// Element Alias/SkinElement pair
		std::map<Ogre::String,SkinElement*> mSkinElements;
		// ComponentName/SkinReference pair
		std::map<Ogre::String,SkinReference*> mSkinReferences;
	};
}

#endif
