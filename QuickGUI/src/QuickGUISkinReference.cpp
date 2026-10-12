#include "QuickGUISkinReference.h"

namespace QuickGUI
{
	SkinReference::SkinReference(const std::string& alias) :
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
