*************************************
*** Building the library on Linux ***
*************************************

Open a terminal window and cd to the QuickGUI root directory.  Type:
	cmake .
Default options should be sufficient in most cases, but if you want to mess with them you can now type:
	ccmake .
After you're done configuring everything, run cmake one more time to make sure you haven't introduced any more errors.  When you're ready, you can then type:
	make -j 4
	sudo make install



****************************************
*** Building the library on Mac OS X ***
****************************************

Though the Mac OS X build system is different behind the scenes, the end-user may follows the same instructions used on Linux.



***************************************
*** Building the library on Windows ***
***************************************

It is very likely the project's include/lib paths are not setup to link against Ogre correctly, so it is important to know how to update these.  The following steps show how to update the QuickGUI project from within Visual Studio:

Setting the include paths:
1. Right click the QuickGUI Project, Properties
2. Navigate to "Configuration Properties", "C/C++", "General", "Additional Include Directories". 
3. Make sure a path to your Ogre include directory is listed. This can be anywhere on your hard drive.

Setting the lib paths:
1. Right click the QuickGUI Project, Properties
2. Navigate to "Configuration Properties", "Linker", "General", "Additional Library Directories". 
3. Make sure a path to the directory with OgreMain.lib and OgreMain_d.lib are listed.



****************************
*** Basics with QuickGUI ***
****************************

Wiki:  http://www.ogre3d.org/wiki/index.php/QuickGUI

Getting application setup:  http://www.ogre3d.org/wiki/index.php/QuickGUI_Beginner_Tutorial_1
Creating a Button:	http://www.ogre3d.org/wiki/index.php/QuickGUI_Beginner_Tutorial_2
