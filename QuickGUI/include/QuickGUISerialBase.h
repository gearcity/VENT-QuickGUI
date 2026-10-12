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

		virtual bool begin(const std::string& definitionType, const std::string& definitionID) = 0;

		virtual void end() = 0;

		ScriptDefinition* getCurrentDefinition();

		virtual bool isSerialWriter() = 0;
		virtual bool isSerialReader() = 0;
		// Serialization of properties
		virtual void IO(const std::string& propertyName, unsigned short* member) = 0;
		virtual void IO(const std::string& propertyName, BrushFilterMode* member) = 0;
		virtual void IO(const std::string& propertyName, ConsoleLayout* member) = 0;
		virtual void IO(const std::string& propertyName, int* member) = 0;
		virtual void IO(const std::string& propertyName, unsigned int* member) = 0;
		virtual void IO(const std::string& propertyName, ColourValue* member) = 0;
		virtual void IO(const std::string& propertyName, ToolBarItemLayout* member) = 0;
		virtual void IO(const std::string& propertyName, std::string* member) = 0;
		virtual void IO(const std::string& propertyName, bool* member) = 0;
		virtual void IO(const std::string& propertyName, float* member) = 0;
		virtual void IO(const std::string& propertyName, Point* member) = 0;
		virtual void IO(const std::string& propertyName, ProgressBarFillDirection* member) = 0;
		virtual void IO(const std::string& propertyName, ProgressBarLayout* member) = 0;
		virtual void IO(const std::string& propertyName, ProgressBarClippingEdge* member) = 0;
		virtual void IO(const std::string& propertyName, Rect* member) = 0;
		virtual void IO(const std::string& propertyName, VScrollBarButtonLayout* member) = 0;
		virtual void IO(const std::string& propertyName, HScrollBarButtonLayout* member) = 0;
		virtual void IO(const std::string& propertyName, Size* member) = 0;
		virtual void IO(const std::string& propertyName, HorizontalTextAlignment* member) = 0;
		virtual void IO(const std::string& propertyName, HorizontalAnchor* member) = 0;
		virtual void IO(const std::string& propertyName, VerticalAnchor* member) = 0;
		virtual void IO(const std::string& propertyName, VerticalTextAlignment* member) = 0;

	protected:
		ScriptDefinition* mCurrentDefinition;

	private:
	};
}

#endif
