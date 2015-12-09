#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIScriptWriter.h"
#include "QuickGUISerialReader.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIModalWindow.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIVScrollBar.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIRoot.h"

namespace QuickGUI
{
	const Ogre::String Sheet::BACKGROUND = "background";

	void Sheet::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Sheet");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Sheet",d);
	}

	SheetDesc::SheetDesc() :
		WindowDesc()
	{
		resetToDefault();
	}

	void SheetDesc::resetToDefault()
	{
		WindowDesc::resetToDefault();

		WindowDesc::window_titleBar = false;

		widget_dimensions.size = Size(800,600);
		widget_resizeFromBottom = false;
		widget_resizeFromLeft = false;
		widget_resizeFromRight = false;
		widget_resizeFromTop = false;
		widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
	}

	void SheetDesc::serialize(SerialBase* b)
	{
		// The reason I skip calling WindowDesc::serialize is because I don't
		// want the Sheet's properties to include the Window's TitleBar 
		// properties. (Nor do I want users to manually add TitleBars to Sheets)
		PanelDesc::serialize(b);
	}

	Sheet::Sheet(SheetDesc* d) :
		Window(d->widget_name),
		mDesc(NULL),
		mKeyboardListener(0),
		mDeleting(false)
	{
		_initialize(d);

		mWindowInFocus = this;
	}

	Sheet::Sheet(const Ogre::String& s, bool loadFromFile) :
		Window(""),
		mWindowInFocus(0),
		mKeyboardListener(0)
	{
		mWidgetDesc = FactoryManager::getSingleton().getDescFactory()->createInstance<WidgetDesc>("SheetDesc");

		// Perform an isolated parsing of the one file
		ScriptReader* sc = ScriptReader::getSingletonPtr();
		sc->begin();

		if(loadFromFile)
		{
			sc->parseFile(s);
			if(sc->getDefinitions().empty())
				throw Exception(Exception::ERR_SERIALIZATION,"No definitions found in file \"" + s + "\"!","Sheet::Sheet");
		}
		else
		{
			sc->parseString(s);
			if(sc->getDefinitions().empty())
				throw Exception(Exception::ERR_SERIALIZATION,"No definitions found in the string provided!","Sheet::Sheet");
		}

		mWidgetDesc->widget_name = sc->getDefinitions().front()->getID();
			
		serialize(SerialReader::getSingletonPtr());

		// remove all temporary definitions found from parsing the file
		sc->end();
	}

	Sheet::~Sheet()
	{
		Root::getSingleton().notifySheetDestroyed(this);

		mDeleting = true;

		cleanupWidgets();

		WidgetFactory* f = FactoryManager::getSingleton().getWidgetFactory();
		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
			f->destroyInstance((*it));

		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
			f->destroyInstance((*it));
	}

	void Sheet::_notifyNameFreed(const Ogre::String& name)
	{
		std::set<Ogre::String>::iterator it = mUsedNames.find(name);
		if(it != mUsedNames.end())
			mUsedNames.erase(it);
	}

	void Sheet::_notifyNameUsed(const Ogre::String& name)
	{
		if(mUsedNames.find(name) == mUsedNames.end())
			mUsedNames.insert(name);
	}

	void Sheet::_initialize(WidgetDesc* d)
	{
		Window::_initialize(d);

		mDesc = dynamic_cast<SheetDesc*>(mWidgetDesc);

		SheetDesc* sd = dynamic_cast<SheetDesc*>(d);

		setSkinType(d->widget_skinTypeName);

		mWidgetDesc->sheet = this;

		// Update Desc to be named properly
		setName(mWidgetDesc->widget_name);
	}

	void Sheet::_setSheet(Sheet* sheet)
	{
		Window::_setSheet(sheet);

		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
			(*it)->_setSheet(sheet);

		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
			(*it)->_setSheet(sheet);
	}

	void Sheet::addWindow(Window* w)
	{
		mWindows.push_back(w);
		w->_setGUIManager(mWidgetDesc->guiManager);
		w->_setSheet(this);
	}

	void Sheet::addModalWindow(ModalWindow* w)
	{
		mModalWindows.push_back(w);
		w->_setGUIManager(mWidgetDesc->guiManager);
		w->_setSheet(this);
	}

	void Sheet::bringToFront(Window* w)
	{
		std::list<Window*>::iterator it = std::find(mWindows.begin(),mWindows.end(),w);
		if(it == mWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Window \"" + w->getName() + "\" does not belong to Sheet \"" + getName() + "\".", "Sheet::bringToFront");

		mWindows.erase(it);

		mWindows.push_back(w);

		focusWindow(w);
	}

	void Sheet::cleanupWidgets()
	{
		// Clear out freelist
		while(!mFreeList.empty())
		{		
			FactoryManager::getSingleton().getWidgetFactory()->destroyInstance(mFreeList.front());
			mFreeList.pop_front();
		}
	}

	ContextMenu* Sheet::createContextMenu(ContextMenuDesc* d)
	{
		ContextMenu* m = dynamic_cast<ContextMenu*>(_createWidget(d));
		addWindow(dynamic_cast<Window*>(m));
		return m;
	}

	ContextMenu* Sheet::createContextMenu()
	{
		ContextMenuDesc* d = DescManager::getSingletonPtr()->getDefaultContextMenuDesc();
		d->resetToDefault();

		return createContextMenu(d);
	}

	ContextMenu* Sheet::createContextMenu(const Point& position)
	{
		ContextMenuDesc* d = DescManager::getSingletonPtr()->getDefaultContextMenuDesc();
		d->resetToDefault();

		d->widget_dimensions.position = position;

		return createContextMenu(d);
	}

	ContextMenu* Sheet::createContextMenu(const Size& size)
	{
		ContextMenuDesc* d = DescManager::getSingletonPtr()->getDefaultContextMenuDesc();
		d->resetToDefault();

		d->widget_dimensions.size = size;

		return createContextMenu(d);
	}

	ContextMenu* Sheet::createContextMenu(const Ogre::UTFString& titleBarText, const Size& size)
	{
		ContextMenuDesc* d = DescManager::getSingletonPtr()->getDefaultContextMenuDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(titleBarText));

		d->widget_dimensions.size = size;

		ContextMenu* m = createContextMenu(d);

		d->resetToDefault();

		return m;
	}

	ContextMenu* Sheet::createContextMenu(std::vector<TextSegment> segments, const Size& size)
	{
		ContextMenuDesc* d = DescManager::getSingletonPtr()->getDefaultContextMenuDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		d->widget_dimensions.size = size;

		ContextMenu* m = createContextMenu(d);

		d->resetToDefault();

		return m;
	}

	ContextMenu* Sheet::createContextMenuNoTitleBar(const Size& size)
	{
		ContextMenuDesc* d = DescManager::getSingletonPtr()->getDefaultContextMenuDesc();
		d->resetToDefault();

		d->window_titleBar = false;
		d->widget_dimensions.size = size;

		return createContextMenu(d);
	}

	ModalWindow* Sheet::createModalWindow(ModalWindowDesc* d)
	{
		ModalWindow* w = dynamic_cast<ModalWindow*>(_createWidget(d));
		addModalWindow(w);
		return w;
	}

	ModalWindow* Sheet::createModalWindow()
	{
		ModalWindowDesc* d = DescManager::getSingletonPtr()->getDefaultModalWindowDesc();
		d->resetToDefault();

		return createModalWindow(d);
	}

	ModalWindow* Sheet::createModalWindow(const Rect& dimensions)
	{
		ModalWindowDesc* d = DescManager::getSingletonPtr()->getDefaultModalWindowDesc();
		d->resetToDefault();

		d->widget_dimensions = dimensions;

		return createModalWindow(d);
	}

	ModalWindow* Sheet::createModalWindow(const Ogre::UTFString& titleBarText, const Rect& dimensions)
	{
		ModalWindowDesc* d = DescManager::getSingletonPtr()->getDefaultModalWindowDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(titleBarText));

		d->widget_dimensions = dimensions;

		ModalWindow* w = createModalWindow(d);

		d->resetToDefault();

		return w;
	}

	ModalWindow* Sheet::createModalWindow(std::vector<TextSegment> segments, const Rect& dimensions)
	{
		ModalWindowDesc* d = DescManager::getSingletonPtr()->getDefaultModalWindowDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		d->widget_dimensions = dimensions;

		ModalWindow* w = createModalWindow(d);

		d->resetToDefault();

		return w;
	}

	ModalWindow* Sheet::createModalWindowNoTitleBar(const Rect& dimensions)
	{
		ModalWindowDesc* d = DescManager::getSingletonPtr()->getDefaultModalWindowDesc();
		d->resetToDefault();

		d->window_titleBar = false;
		d->widget_dimensions = dimensions;

		return createModalWindow(d);
	}

	Window* Sheet::createWindow(WindowDesc* d)
	{
		Window* w = dynamic_cast<Window*>(_createWidget(d));
		addWindow(w);
		return w;
	}

	Window* Sheet::createWindow()
	{
		WindowDesc* d = DescManager::getSingletonPtr()->getDefaultWindowDesc();
		d->resetToDefault();

		return createWindow(d);
	}

	Window* Sheet::createWindow(const Point& position)
	{
		WindowDesc* d = DescManager::getSingletonPtr()->getDefaultWindowDesc();
		d->resetToDefault();

		d->widget_dimensions.position = position;

		return createWindow(d);
	}

	Window* Sheet::createWindow(const Rect& dimensions)
	{
		WindowDesc* d = DescManager::getSingletonPtr()->getDefaultWindowDesc();
		d->resetToDefault();

		d->widget_dimensions = dimensions;

		return createWindow(d);
	}

	Window* Sheet::createWindow(const Ogre::UTFString& titleBarText, const Rect& dimensions)
	{
		WindowDesc* d = DescManager::getSingletonPtr()->getDefaultWindowDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(titleBarText));

		d->widget_dimensions = dimensions;

		Window* w = createWindow(d);

		d->resetToDefault();

		return w;
	}

	Window* Sheet::createWindow(std::vector<TextSegment> segments, const Rect& dimensions)
	{
		WindowDesc* d = DescManager::getSingletonPtr()->getDefaultWindowDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		d->widget_dimensions = dimensions;

		Window* w = createWindow(d);

		d->resetToDefault();

		return w;
	}

	Window* Sheet::createWindowNoTitleBar(const Rect& dimensions)
	{
		WindowDesc* d = DescManager::getSingletonPtr()->getDefaultWindowDesc();
		d->resetToDefault();

		d->window_titleBar = false;
		d->widget_dimensions = dimensions;

		return createWindow(d);
	}

	void Sheet::destroyContextMenu(const Ogre::String& name)
	{
		destroyContextMenu(getContextMenu(name));
	}

	void Sheet::destroyContextMenu(ContextMenu* m)
	{
		Window* w = dynamic_cast<Window*>(m);

		if(w == NULL)
			throw Exception(Exception::ERR_INVALID_CHILD,"ContextMenu is NULL!","Sheet::destroyContextMenu");

		std::list<Window*>::iterator it = std::find(mWindows.begin(),mWindows.end(),w);
		if(it == mWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"ContextMenu \"" + w->getName() + "\" is not a child of Sheet \"" + getName() + "\"","Sheet::destroyContextMenu");
		mWindows.erase(it);

		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if((getKeyboardListener() != NULL) && (w->findWidget(getKeyboardListener()->getName()) != NULL))
			setKeyboardListener(NULL);

		mFreeList.push_back(w);
	}

	void Sheet::destroyModalWindow(const Ogre::String& name)
	{
		destroyModalWindow(getModalWindow(name));
	}

	void Sheet::destroyModalWindow(ModalWindow* w)
	{
		if(w == NULL)
			throw Exception(Exception::ERR_INVALID_CHILD,"Window is NULL!","Sheet::destroyModalWindow");

		std::list<ModalWindow*>::iterator it = std::find(mModalWindows.begin(),mModalWindows.end(),w);
		if(it == mModalWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"ModalWindow \"" + w->getName() + "\" is not a child of Sheet \"" + getName() + "\"","Sheet::destroyModalWindow");
		mModalWindows.erase(it);

		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if((getKeyboardListener() != NULL) && (w->findWidget(getKeyboardListener()->getName()) != NULL))
			setKeyboardListener(NULL);

		mFreeList.push_back(w);
	}

	void Sheet::destroyWidget(Widget* w)
	{
		if(w == NULL)
			throw Exception(Exception::ERR_INVALID_CHILD,"Widget is NULL!","Sheet::destroyWidget");

		Ogre::String className = w->getClass();
		if(className == "ModalWindow")
			destroyModalWindow(dynamic_cast<ModalWindow*>(w));
		else if(className == "Window")
			destroyWindow(dynamic_cast<Window*>(w));
		else if(w->getParentWidget() != NULL)
			throw Exception(Exception::ERR_INVALID_STATE,"Widget it attached to another widget!","Sheet::destroyWidget");

		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if((getKeyboardListener() != NULL) && (w->findWidget(getKeyboardListener()->getName()) != NULL))
			setKeyboardListener(NULL);

		mFreeList.push_back(w);
	}

	void Sheet::destroyWindow(const Ogre::String& name)
	{
		destroyWindow(getWindow(name));
	}

	void Sheet::destroyWindow(Window* w)
	{
		if(w == NULL)
			throw Exception(Exception::ERR_INVALID_CHILD,"Window is NULL!","Sheet::destroyWindow");

		std::list<Window*>::iterator it = std::find(mWindows.begin(),mWindows.end(),w);
		if(it == mWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Window \"" + w->getName() + "\" is not a child of Sheet \"" + getName() + "\"","Sheet::destroyWindow");
		mWindows.erase(it);

		if(std::find(mFreeList.begin(),mFreeList.end(),w) != mFreeList.end())
			return;

		if((getKeyboardListener() != NULL) && (w->findWidget(getKeyboardListener()->getName()) != NULL))
			setKeyboardListener(NULL);

		mFreeList.push_back(w);
	}

	void Sheet::drag(unsigned int xOffset, unsigned int yOffset)
	{
		Widget::drag(xOffset,yOffset);
	}

	void Sheet::draw()
	{
		Window::draw();

		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
			(*it)->draw();

		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
			(*it)->draw();
	}

	Widget* Sheet::findWidget(const Ogre::String& name)
	{
		Widget* w = NULL;

		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
			if((w = (*it)->findWidget(name)) != NULL)
				return w;
		
		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
			if((w = (*it)->findWidget(name)) != NULL)
				return w;

		return ContainerWidget::findWidget(name);
	}

	Window* Sheet::findWindowAtPoint(const Point& p, bool ignoreDisabled)
	{
		ModalWindow* mw = NULL;

		for(std::list<ModalWindow*>::reverse_iterator it = mModalWindows.rbegin(); it != mModalWindows.rend(); ++it)
		{
			// Visible Modal Window found at point p
			if((*it)->getVisible())
			{
				mw = (*it);

				if((*it)->getDimensions().isPointWithinBounds(p))
				{
					// If we are ignoring disabled windows and the window is disabled, do not return it.
					if(ignoreDisabled && !((*it)->getEnabled()))
						continue;

					return (*it);
				}
			}
		}

		if(mw != NULL)
			return mw;

		for(std::list<Window*>::reverse_iterator it = mWindows.rbegin(); it != mWindows.rend(); ++it)
		{
			// Visible Window found at point p
			if((*it)->getVisible() && (*it)->getDimensions().isPointWithinBounds(p))
			{
				// If we are ignoring disabled windows and the window is disabled, do not return it.
				if(ignoreDisabled && !((*it)->getEnabled()))
					continue;

				return (*it);
			}
		}

		return this;
	}

	bool Sheet::focusWindow(Window* w)
	{
		if(w != this)
		{
			std::list<Window*>::iterator it1 = std::find(mWindows.begin(),mWindows.end(),w);
			std::list<ModalWindow*>::iterator it2 = std::find(mModalWindows.begin(),mModalWindows.end(),w);
			if((it1 == mWindows.end()) && (it2 == mModalWindows.end()))
				throw Exception(Exception::ERR_INVALID_CHILD,"Window \"" + w->getName() + "\" does not belong to Sheet \"" + getName() + "\".", "Sheet::focusWindow");
		}

		if(mWindowInFocus != w)
		{
			Window* previousWindowInFocus = mWindowInFocus;

			// Update window in focus
			mWindowInFocus = w;

			// Notify previous window it lost focus
			if(previousWindowInFocus != NULL)
			{
				WidgetEventArgs args(mWindowInFocus);
				previousWindowInFocus->fireWindowEvent(WINDOW_EVENT_FOCUS_LOST,args);
			}

			// Move window to end of list
			if(mWindowInFocus != this)
			{
				if(mWindowInFocus->getClass() == "ModalWindow")
				{
					for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
					{
						if((*it) == mWindowInFocus)
						{
							mModalWindows.erase(it);
							mModalWindows.push_back(dynamic_cast<ModalWindow*>(mWindowInFocus));
							break;
						}
					}
				}
				else
				{
					for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
					{
						if((*it) == mWindowInFocus)
						{
							mWindows.erase(it);
							mWindows.push_back(mWindowInFocus);
							break;
						}
					}
				}
			}

			// Notify current window its in focus
			if(mWindowInFocus)
			{
				WidgetEventArgs args(mWindowInFocus);
				mWindowInFocus->fireWindowEvent(WINDOW_EVENT_FOCUS_GAINED,args);
			}

			return true;
		}

		return false;
	}

	Ogre::String Sheet::getClass()
	{
		return "Sheet";
	}

	ContextMenu* Sheet::getContextMenu(const Ogre::String& name)
	{
		if(!hasContextMenu(name))
			throw Exception(Exception::ERR_INVALID_CHILD,"Sheet \"" + getName() + "\" does not have a child context menu with name \"" + name + "\".","Sheet::getContextMenu");

		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			if(((*it)->getName() == name) && ((*it)->getClass() == "ContextMenu"))
				return dynamic_cast<ContextMenu*>((*it));
		}

		return NULL;
	}

	Widget* Sheet::getKeyboardListener()
	{
		return mKeyboardListener;
	}

	ModalWindow* Sheet::getModalWindow(const Ogre::String& name)
	{
		if(!hasModalWindow(name))
			throw Exception(Exception::ERR_INVALID_CHILD,"Sheet \"" + getName() + "\" does not have a child modal window with name \"" + name + "\".","Sheet::getModalWindow");

		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
		{
			if((*it)->getName() == name)
				return (*it);
		}

		return NULL;
	}

	Window* Sheet::getWindow(const Ogre::String& name)
	{
		if(!hasWindow(name))
			throw Exception(Exception::ERR_INVALID_CHILD,"Sheet \"" + getName() + "\" does not have a child window with name \"" + name + "\".","Sheet::getWindow");

		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			if(((*it)->getName() == name) && ((*it)->getClass() == "Window"))
				return (*it);
		}

		return NULL;
	}

	Window* Sheet::getWindowInFocus()
	{
		return mWindowInFocus;
	}

	bool Sheet::hasContextMenu(const Ogre::String& name)
	{
		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			if(((*it)->getName() == name) && ((*it)->getClass() == "ContextMenu"))
				return true;
		}

		return false;
	}

	bool Sheet::hasModalWindow(const Ogre::String& name)
	{
		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
		{
			if((*it)->getName() == name)
				return true;
		}

		return false;
	}

	bool Sheet::hasWindow(const Ogre::String& name)
	{
		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			if(((*it)->getName() == name) && ((*it)->getClass() == "Window"))
				return true;
		}

		return false;
	}

	bool Sheet::isNameUsed(const Ogre::String& name)
	{
		return (mUsedNames.find(name) != mUsedNames.end());
	}

	bool Sheet::isPositionManaged()
	{
		return true;
	}

	bool Sheet::isSizeManaged()
	{
		return true;
	}

	void Sheet::notifyViewport(float* width, float* height)
	{
		if(mDesc->containerwidget_supportScrollBars)
		{
			// Sheets may be bigger than viewport they are drawn on.  If this is the case,
			// and Scrolling is supported, update the ScrollBars to use viewport dimensions
/*
			mHScrollBar->setPosition(Point(0,(*height) - mHScrollBar->getHeight()));
			mHScrollBar->setWidth((*width) - mVScrollBar->getWidth());
			mHScrollBar->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT);
			mHScrollBar->setVerticalAnchor(ANCHOR_VERTICAL_TOP);
		
			mVScrollBar->setPosition(Point((*width) - mVScrollBar->getWidth(),0));
			mVScrollBar->setHeight((*height) - mHScrollBar->getHeight());
			mVScrollBar->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT);
			mVScrollBar->setVerticalAnchor(ANCHOR_VERTICAL_TOP);

			updateVirtualDimensions();
*/
		}
	}

	void Sheet::removeModalWindow(ModalWindow* w)
	{
		for(std::list<ModalWindow*>::iterator it = mModalWindows.begin(); it != mModalWindows.end(); ++it)
		{
			if((*it) == w)
			{
				mModalWindows.erase(it);
				w->_setSheet(NULL);
				return;
			}
		}
	}

	void Sheet::removeWindow(Window* w)
	{
		for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			if((*it) == w)
			{
				mWindows.erase(it);
				w->_setSheet(NULL);
				return;
			}
		}
	}

	Ogre::String Sheet::resolveName(const Ogre::String& name)
	{
		if(mUsedNames.find(name) != mUsedNames.end())
		{
			int strLen = static_cast<int>(name.size());

			int counter = strLen - 1;
			while(isdigit(name[counter]))
				--counter;

			int n = 1;
			if(counter != (strLen - 1))
				n = atoi(name.substr(counter + 1).c_str());

			Ogre::String baseName = name.substr(0,counter + 1);

			while(mUsedNames.find(baseName + Ogre::StringConverter::toString(n)) != mUsedNames.end())
				++n;
			
			return (baseName + Ogre::StringConverter::toString(n));
		}
		else
			return name;
	}

	void Sheet::saveToDisk(const Ogre::String fileName)
	{
		ScriptWriter::getSingletonPtr()->begin(fileName);

		serialize(SerialWriter::getSingletonPtr());

		ScriptWriter::getSingletonPtr()->end();
	}

	void Sheet::sendToBack(Window* w)
	{
		std::list<Window*>::iterator it = std::find(mWindows.begin(),mWindows.end(),w);
		if(it == mWindows.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Window \"" + w->getName() + "\" does not belong to Sheet \"" + getName() + "\".", "Sheet::bringToFront");

		mWindows.erase(it);

		mWindows.push_front(w);
	}

	void Sheet::serialize(SerialBase* b)
	{
		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		if(b->isSerialReader())
			_initialize(mWidgetDesc);

		if(b->begin("Child","Widgets"))
		{
			// If we are reading, we need to create and populate widgets
			if(b->isSerialReader())
			{
				std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions();
				for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
				{
					// Create Empty Widget, supplying class name and widget name from script
					Widget* newWidget = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>((*it)->getType(),(*it)->getID());

					// Populate Desc object from Script Text, and initialize widget
					newWidget->serialize(b);

					// Add as child Widget
					addChild(newWidget);
				}
			}
			// If we are writing, we need to serialize children
			else
			{
				for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
				{
					(*it)->serialize(b);
				}
			}
			b->end();
		}

		if(b->begin("Child","Windows"))
		{
			// If we are reading, we need to create and populate widgets
			if(b->isSerialReader())
			{
				std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions();
				for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
				{
					// Create Empty Widget, supplying class name and widget name from script
					Widget* newWidget = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>((*it)->getType(),(*it)->getID());

					// Populate Desc object from Script Text, and initialize widget
					newWidget->serialize(b);

					// Windows are not children, make sure to add them appropriately
					addWindow(dynamic_cast<Window*>(newWidget));
				}
			}
			// If we are writing, we need to serialize children
			else
			{
				for(std::list<Window*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it)
				{
					(*it)->serialize(b);
				}
			}
			b->end();
		}

		b->end();
	}

	void Sheet::setKeyboardListener(Widget* w)
	{
		if(mKeyboardListener == w)
			return;

		if(mKeyboardListener != NULL)
		{
			WidgetEventArgs args(mKeyboardListener);
			mKeyboardListener->fireWidgetEvent(WIDGET_EVENT_KEYBOARD_INPUT_LOSE,args);
		}

		mKeyboardListener = w;

		if(mKeyboardListener != NULL)
		{
			WidgetEventArgs args(mKeyboardListener);
			mKeyboardListener->fireWidgetEvent(WIDGET_EVENT_KEYBOARD_INPUT_GAIN,args);
		}
	}
}
