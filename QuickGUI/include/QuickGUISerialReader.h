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

		virtual bool begin(const Ogre::String& definitionType, const Ogre::String& definitionID);

		virtual void end();

		virtual void IO(const Ogre::String& propertyName, unsigned short* member);
		virtual void IO(const Ogre::String& propertyName, BrushFilterMode* member);
		virtual void IO(const Ogre::String& propertyName, ConsoleLayout* member);
		virtual void IO(const Ogre::String& propertyName, int* member);
		virtual void IO(const Ogre::String& propertyName, unsigned int* member);
		virtual void IO(const Ogre::String& propertyName, ColourValue* member);
		virtual void IO(const Ogre::String& propertyName, ToolBarItemLayout* member);
		virtual void IO(const Ogre::String& propertyName, Ogre::String* member);
		virtual void IO(const Ogre::String& propertyName, bool* member);
		virtual void IO(const Ogre::String& propertyName, float* member);
		virtual void IO(const Ogre::String& propertyName, Point* member);
		virtual void IO(const Ogre::String& propertyName, ProgressBarFillDirection* member);
		virtual void IO(const Ogre::String& propertyName, ProgressBarLayout* member);
		virtual void IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member);
		virtual void IO(const Ogre::String& propertyName, Rect* member);
		virtual void IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member);
		virtual void IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member);
		virtual void IO(const Ogre::String& propertyName, Size* member);
		virtual void IO(const Ogre::String& propertyName, HorizontalTextAlignment* member);
		virtual void IO(const Ogre::String& propertyName, HorizontalAnchor* member);
		virtual void IO(const Ogre::String& propertyName, VerticalAnchor* member);
		virtual void IO(const Ogre::String& propertyName, VerticalTextAlignment* member);
		bool isSerialWriter();
		bool isSerialReader();

	protected:
		SerialReader();
		virtual ~SerialReader() {}
	private:
	};
}

#endif
