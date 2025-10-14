#ifndef QUICKGUISERIALIZABLE_H
#define QUICKGUISERIALIZABLE_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISerialBase.h"

namespace QuickGUI
{
	class _QuickGUIExport Serializable 
	{
	public:
		virtual void serialize(SerialBase* b) = 0;
	};
}

#endif
