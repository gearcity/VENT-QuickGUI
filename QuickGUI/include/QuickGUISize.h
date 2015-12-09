#ifndef QUICKGUISIZE_H
#define QUICKGUISIZE_H

#include "QuickGUIExportDLL.h"

namespace QuickGUI
{
	class _QuickGUIExport Size
	{
	public:
		Size();
		Size(float WidthHeight);
		Size(float Width, float Height);

		inline Size( const Size& s )
            : width( s.width ), height( s.height )
        {
        }

		inline Size& operator = ( const Size& s )
        {
            width = s.width;
            height = s.height;

            return *this;
        }

		inline bool operator != ( const Size& s ) const
        {
            return ( width != s.width ||
                height != s.height );
        }

		inline bool operator == ( const Size& s ) const
        {
            return ( width == s.width &&
                height == s.height );
        }

		inline Size& operator /= ( float f )
        {
            width /= f;
            height /= f;

            return *this;
        }

		inline Size operator * ( const float& r ) const
        {
            return Size(width * r,height * r);
        }

		inline Size& operator *= ( const float& r )
        {
            width *= r;
            height *= r;

            return *this;
        }

		inline Size operator * ( const Size& s ) const
        {
			return Size(width * s.width,height * s.height);
        }

		inline Size operator / ( const Size& s ) const
        {
            return Size(width / s.width,height / s.height);
        }

		inline Size operator + ( const Size& s ) const
        {
            return Size(width + s.width,height + s.height);
        }

		inline Size operator - ( const Size& s ) const
        {
            return Size(width - s.width,height - s.height);
        }

		Size& roundUp();
		Size& roundDown();
		Size& round();

		float width;
		float height;

		static const Size ZERO;
	};
}

#endif
