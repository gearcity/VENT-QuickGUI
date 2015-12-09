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

#ifndef QUICKGUITEXTLINE_H
#define QUICKGUITEXTLINE_H

#include "QuickGUIBrush.h"
#include "QuickGUICharacter.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIPoint.h"
#include "QuickGUISize.h"

#include <vector>

namespace Ogre
{
	// forward declarations
	class Font;
}

namespace QuickGUI
{
	class _QuickGUIExport TextLine
	{
	public:
		TextLine();
		~TextLine();

		/**
		* Adds a character to this object.
		*/
		void addCharacter(Character* c);

		/**
		* Draws all characters in this object starting at point p.
		*/
		void draw(Point p);

		/**
		* Returns true if this TextLine has no characters, false otherwise.
		*/
		bool empty();

		/**
		* Returns the character at the index given.
		*/
		Character* getCharacter(int index);
		/**
		* Returns the index of the character closest to point p. If p is to the left
		* of the TextLine, index 0 is returned.  If p is to the right of the TextLine,
		* index (length - 1) is returned.
		*/
		int getCursorIndexAtPosition(Point p);
		/**
		* Returns the height of the TextLine, which varies depending on the Font size of
		* all characters in the text line.
		*/
		float getHeight();
		/**
		* Returns the number of characters in this TextLine.
		*/
		int getLength();
		/**
		* Returns the position of the character at the index specified.
		*/
		Point getPositionAtCharacterIndex(unsigned int index);
		/**
		* Returns the width of the TextLine, which varies depending on the Font size of
		* all characters in the text line.
		*/
		float getWidth();

		/**
		* Sets whether or not the Text will be masked. (password box)
		*/
		void setMaskText(bool mask, Ogre::UTFString::code_point maskSymbol);

	protected:
		Point mPosition;

		bool mMaskText;
		Ogre::UTFString::code_point mMaskSymbol;

		std::vector<Character*> mCharacters;

		/// Record the largest font size of any character within this TextLine
		Ogre::Font* mLargestFont;

		/**
		* Iterates through characters and aligns them according to the largest baseline of any
		* character.
		*/
		void _verticallyAlignCharacters();

	private:
	};
}

#endif
