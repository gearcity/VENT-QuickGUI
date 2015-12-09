#ifndef QUICKGUISERIALBASE_H
#define QUICKGUISERIALBASE_H

#include "QuickGUIAnchors.h"
#include "QuickGUIBrushEnums.h"
#include "QuickGUIConsoleEnums.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIProgressBarEnums.h"
#include "QuickGUIRect.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIScrollBarButtonLayouts.h"
#include "QuickGUITextAlignment.h"
#include "QuickGUIToolBarEnums.h"
#include "QuickGUIOgreEquivalents.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	// forward declaration
	class ScriptDefinition;

	/**
	* Abstract class setting the foundation for the 
	* SerialWriter and SerialReader class.
	*/
	class _QuickGUIExport SerialBase
	{
	public:
		SerialBase();

		virtual bool begin(const Ogre::String& definitionType, const Ogre::String& definitionID) = 0;

		virtual void end() = 0;

		ScriptDefinition* getCurrentDefinition();

		virtual bool isSerialWriter() = 0;
		virtual bool isSerialReader() = 0;
		// Serialization of properties
		virtual void IO(const Ogre::String& propertyName, unsigned short* member) = 0;
		virtual void IO(const Ogre::String& propertyName, BrushFilterMode* member) = 0;
		virtual void IO(const Ogre::String& propertyName, ConsoleLayout* member) = 0;
		virtual void IO(const Ogre::String& propertyName, int* member) = 0;
		virtual void IO(const Ogre::String& propertyName, unsigned int* member) = 0;
		virtual void IO(const Ogre::String& propertyName, ColourValue* member) = 0;
		virtual void IO(const Ogre::String& propertyName, ToolBarItemLayout* member) = 0;
		virtual void IO(const Ogre::String& propertyName, Ogre::String* member) = 0;
		virtual void IO(const Ogre::String& propertyName, bool* member) = 0;
		virtual void IO(const Ogre::String& propertyName, float* member) = 0;
		virtual void IO(const Ogre::String& propertyName, Point* member) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarFillDirection* member) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarLayout* member) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member) = 0;
		virtual void IO(const Ogre::String& propertyName, Rect* member) = 0;
		virtual void IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member) = 0;
		virtual void IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member) = 0;
		virtual void IO(const Ogre::String& propertyName, Size* member) = 0;
		virtual void IO(const Ogre::String& propertyName, HorizontalTextAlignment* member) = 0;
		virtual void IO(const Ogre::String& propertyName, HorizontalAnchor* member) = 0;
		virtual void IO(const Ogre::String& propertyName, VerticalAnchor* member) = 0;
		virtual void IO(const Ogre::String& propertyName, VerticalTextAlignment* member) = 0;

	protected:
		ScriptDefinition* mCurrentDefinition;

	private:
	};
}

#endif
