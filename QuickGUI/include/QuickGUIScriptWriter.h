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
