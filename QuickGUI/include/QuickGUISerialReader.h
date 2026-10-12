#ifndef QUICKGUISERIALREADER_H
#define QUICKGUISERIALREADER_H

#include "QuickGUISerialBase.h"
#include "QuickGUIText.h"

#include "OgreSingleton.h"
#include "OgreStringConverter.h"

namespace QuickGUI
{
	class _QuickGUIExport SerialReader :
		public SerialBase,
		public Ogre::Singleton<SerialReader>,
		public Ogre::GeneralAllocatedObject
	{
	public:
		friend class TextDesc;
		friend class Root;
	public:

		static SerialReader& getSingleton(void); 
		static SerialReader* getSingletonPtr(void);

		virtual bool begin(const std::string& definitionType, const std::string& definitionID);

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
		SerialReader();
		virtual ~SerialReader() {}
	private:
	};
}

#endif
