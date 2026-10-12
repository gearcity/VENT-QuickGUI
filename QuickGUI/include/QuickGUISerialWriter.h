#ifndef QUICKGUISERIALWRITER_H
#define QUICKGUISERIALWRITER_H

#include "QuickGUISerialBase.h"
#include "QuickGUIStringConverter.h"
#include "QuickGUIText.h"

#include "OgreLogManager.h"
#include "OgreSingleton.h"
#include "OgreStringConverter.h"

namespace QuickGUI
{
	class _QuickGUIExport SerialWriter :
		public SerialBase,
		public Ogre::Singleton<SerialWriter>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class TextDesc;
		friend class Root;
	public:

		static SerialWriter& getSingleton(void); 
		static SerialWriter* getSingletonPtr(void);

		/**
		* Creates a ScriptDefinition.
		*/
		virtual bool begin(const std::string& definitionType, const std::string& definitionID);

		/**
		* Adds a ScriptDefinition to the ScriptWriter.
		*/
		virtual void end();

		virtual void IO(const std::string& propertyName, unsigned short* member);
		virtual void IO(const std::string& propertyName, BrushFilterMode* member);
		virtual void IO(const std::string& propertyName, ConsoleLayout* member);
		virtual void IO(const std::string& propertyName, int* member);
		virtual void IO(const std::string& propertyName, unsigned int* member);
		virtual void IO(const std::string& propertyName, ColourValue* member);
		virtual void IO(const std::string& propertyName, ToolBarItemLayout* member);
		virtual void IO(const std::string& propertyName, std::string* member);
		virtual void IO(const std::string& propertyName, bool* member);
		virtual void IO(const std::string& propertyName, float* member);
		virtual void IO(const std::string& propertyName, Point* member);
		virtual void IO(const std::string& propertyName, ProgressBarFillDirection* member);
		virtual void IO(const std::string& propertyName, ProgressBarLayout* member);
		virtual void IO(const std::string& propertyName, ProgressBarClippingEdge* member);
		virtual void IO(const std::string& propertyName, Rect* member);
		virtual void IO(const std::string& propertyName, VScrollBarButtonLayout* member);
		virtual void IO(const std::string& propertyName, HScrollBarButtonLayout* member);
		virtual void IO(const std::string& propertyName, Size* member);
		virtual void IO(const std::string& propertyName, HorizontalTextAlignment* member);
		virtual void IO(const std::string& propertyName, HorizontalAnchor* member);
		virtual void IO(const std::string& propertyName, VerticalAnchor* member);
		virtual void IO(const std::string& propertyName, VerticalTextAlignment* member);
		bool isSerialWriter();
		bool isSerialReader();

	protected:
		SerialWriter();
		virtual ~SerialWriter() {}
	private:
	};
}

#endif
