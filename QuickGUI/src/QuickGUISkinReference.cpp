#include "QuickGUISkinReference.h"

namespace QuickGUI
{
	SkinReference::SkinReference(const Ogre::String& alias) :
		className(""),
		skinAlias(alias),
		typeName("")
	{
	}

	void SkinReference::serialize(SerialBase* b)
	{
		b->begin("SkinReference",skinAlias);

		b->IO("ClassName",&className);
		b->IO("SkinType",&typeName);

		b->end();
	}
}
