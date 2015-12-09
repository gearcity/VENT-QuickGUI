#ifndef QUICKGUIOGREEQUIVALENTS_H
#define QUICKGUIOGREEQUIVALENTS_H

#include "QuickGUIExportDLL.h"

namespace QuickGUI
{
	struct _QuickGUIExport Vector2
	{
		float x;
		float y;

		inline Vector2() :
			x(0),
			y(0)
        {
        }

		inline Vector2(const float fX, const float fY ) : 
			x( fX ), 
			y( fY )
        {
        }

		static const Vector2 ZERO;
	};

	struct _QuickGUIExport Vector3
	{
		float x;
		float y;
		float z;

		inline Vector3() :
			x(0),
			y(0),
			z(0)
        {
        }

		inline Vector3(const float fX, const float fY , const float fZ ) : 
			x( fX ), 
			y( fY ),
			z( fZ )
        {
        }

		static const Vector3 ZERO;
	};

	struct _QuickGUIExport ColourValue
	{
		float r;
		float g;
		float b;
		float a;

		inline ColourValue( float red = 1.0f,float green = 1.0f, float blue = 1.0f, float alpha = 1.0f ) : 
			r(red), 
			g(green), 
			b(blue), 
			a(alpha)
        { 
		}

		bool operator==(const ColourValue& rhs) const;
	    bool operator!=(const ColourValue& rhs) const;

		static const ColourValue ZERO;
        static const ColourValue Black;
        static const ColourValue White;
        static const ColourValue Red;
        static const ColourValue Green;
        static const ColourValue Blue;
	};
}

#endif
