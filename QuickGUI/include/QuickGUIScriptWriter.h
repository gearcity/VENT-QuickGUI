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

#ifndef QUICKGUISCRIPTWRITER_H
#define QUICKGUISCRIPTWRITER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIScriptDefinition.h"

#include "OgreSingleton.h"

#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <iomanip>   // format manipulation
#include <string>

#include <list>

namespace QuickGUI
{
	class _QuickGUIExport ScriptWriter :
		public Ogre::Singleton<ScriptWriter>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class Root;
	public:
		static ScriptWriter& getSingleton(void);
		static ScriptWriter* getSingletonPtr(void);

		/**
		* Queues a ScriptDefinition for writing to file.
		*/
		void addDefinition(ScriptDefinition* d);

		/**
		* Creates or reads in the file specified, preparing
		* the writer to write script.
		*/
		void begin(const Ogre::String& fileName);

		/**
		* Overwrites the file with all Definitions that have been queued
		* for writing.
		*/
		void end();

		/**
		* Returns true if the ScriptWriter is in the process of writing to file,
		* false otherwise.
		*/
		bool writingToFile();

	protected:
		ScriptWriter();
		virtual ~ScriptWriter();

		/// keep track of whether we are writing to file or not.
		bool mWritingToFile;
		/// name of the file we will write to
		Ogre::String mFileName;

		/// list of Definitions queued for writing.
		std::list<ScriptDefinition*> mDefinitions;

		int mNumSpacesPerLevel;

		/**
		* Writes a definition to file. The level specifies the number of spaces to 
		* pad the lines with.
		*/
		void writeDefinition(std::fstream& stream, ScriptDefinition* d, int level);

		/**
		* Creates a string with the specified number of spaces.
		*/
		Ogre::String getSpaces(unsigned int numberOfSpaces);

	private:
	};
}

#endif
