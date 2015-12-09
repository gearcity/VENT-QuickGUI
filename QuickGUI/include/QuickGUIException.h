/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

#ifndef QUICKGUIEXCEPTION_H
#define QUICKGUIEXCEPTION_H

#include "QuickGUIExportDLL.h"

#include "OgreException.h"
#include "OgrePrerequisites.h"

namespace QuickGUI
{
	class _QuickGUIExport Exception :
		public Ogre::Exception
	{
	public:
		enum ExceptionCodes {
			ERR_FACTORY_NOT_FOUND = (Ogre::Exception::ERR_NOT_IMPLEMENTED + 1),
			ERR_DUPLICATE_FACTORY,
			ERR_INVALID_RENDER_TARGET,
			ERR_INVALID_CHILD,
			ERR_NO_ACTIVE_SHEET,
			ERR_WIDGET_ALREADY_ADDED,
			ERR_INVALID_DESC,
			ERR_UNSUPPORTED_WIDGET,
			ERR_SERIALIZATION,
			ERR_TEXT,
			ERR_SCRIPT_PARSING,
			ERR_SCRIPT_WRITING,
			ERR_SKINNING,
			ERR_FACTORY
/*          ERR_INVALID_STATE,
            ERR_INVALIDPARAMS,
            ERR_RENDERINGAPI_ERROR,
            ERR_DUPLICATE_ITEM,
            ERR_ITEM_NOT_FOUND,
            ERR_FILE_NOT_FOUND,
            ERR_INTERNAL_ERROR,
            ERR_RT_ASSERTION_FAILED, 
			ERR_NOT_IMPLEMENTED
*/        };

		Exception( int number, const Ogre::String& description, const Ogre::String& source );

	protected:
	private:
	};
}

#endif
