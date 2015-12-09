#ifndef QUICKGUIVERTEX_H
#define QUICKGUIVERTEX_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIOgreEquivalents.h"

namespace QuickGUI
{
	const int VERTICES_PER_QUAD = 6;

	struct _QuickGUIExport Vertex
	{
		Vector3 pos;
		ColourValue color;
		Vector2 uv;
	};
}

#endif
