#ifndef QUICKGUIUVRECT_H
#define QUICKGUIUVRECT_H

#include "QuickGUIExportDLL.h"

namespace QuickGUI
{
	class _QuickGUIExport UVRect
	{
	public:
		UVRect();
		UVRect(float Left, float Top, float Right, float Bottom);

		inline UVRect( const UVRect& r )
            : left( r.left ), right( r.right ), top( r.top), bottom( r.bottom )
        {
        }

		inline UVRect& operator = ( const UVRect& r )
        {
            left = r.left;
            right = r.right;
            top = r.top;
            bottom = r.bottom;

            return *this;
        }

		inline bool operator != ( const UVRect& r ) const
        {
            return ( left != r.left ||
                right != r.right ||
                top != r.top ||
                bottom != r.bottom );
        }

		bool operator == ( const UVRect& r ) const
        {
			return ( left == r.left &&
				right == r.right &&
				top == r.top &&
				bottom == r.bottom );
        }

		float left;
		float right;
		float top;
		float bottom;

		static const UVRect ZERO;
	};
}

#endif
