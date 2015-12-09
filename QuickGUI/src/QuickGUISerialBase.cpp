#include "QuickGUISerialBase.h"
#include "QuickGUIScriptDefinition.h"

namespace QuickGUI
{
	SerialBase::SerialBase() :
		mCurrentDefinition(NULL)
	{
	}

	ScriptDefinition* SerialBase::getCurrentDefinition() 
	{ 
		return mCurrentDefinition; 
	}
}
