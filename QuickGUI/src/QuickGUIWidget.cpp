#include "QuickGUIWidget.h"
#include "QuickGUIWindow.h"
#include "QuickGUIRoot.h"
#include "QuickGUISheet.h"
#include "QuickGUISerialWriter.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIContainerWidget.h"
#include "QuickGUIMenuPanel.h"

namespace QuickGUI
{
	WidgetDesc::WidgetDesc() :
		Desc()
	{
		resetToDefault();
	}

	void WidgetDesc::resetToDefault()
	{
		widget_baseColor = ColourValue::White;
		widget_brushFilterMode = BRUSHFILTER_LINEAR;
		widget_consumeKeyboardEvents = false;
		widget_contextMenuName = "";
		widget_enabled = true;
		widget_dimensions = Rect::ZERO;
		widget_disabledSkinType = "";
		widget_dragable = false;
		guiManager = NULL;
		widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		widget_hoverTime = Root::getSingleton().getDefaultHoverTime();
		widget_inheritOpacity = true;
		widget_maxSize = Size::ZERO;
		widget_minSize = Size(5,5);
		widget_moveBaseWidgetOnDrag = false;
		widget_name = getWidgetClass();
		widget_positionRelativeToParentClientDimensions = true;
		widget_relativeOpacity = 1.0;
		widget_resizeFromBottom = false;
		widget_resizeFromLeft = false;
		widget_resizeFromRight = false;
		widget_resizeFromTop = false;
		widget_scrollable = true;
		widget_serialize = true;
		sheet = NULL;
		widget_transparencyPicking = true;
		widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		widget_visible = true;
		widget_skinTypeName = "default";

		for(int index = 0; index < WIDGET_EVENT_COUNT; ++index)
			widget_userHandlers[index] = "";
	}

	void WidgetDesc::serialize(SerialBase* b)
	{
		b->IO("BaseColor",&widget_baseColor);
		b->IO("BrushFilterMode",&widget_brushFilterMode);
		b->IO("ConsumeKeyboardEvents",&widget_consumeKeyboardEvents);
		b->IO("ContextMenuName",&widget_contextMenuName);
		b->IO("Enabled",&widget_enabled);
		b->IO("Dimensions",&widget_dimensions);
		b->IO("DisabledSkinType",&widget_disabledSkinType);
		b->IO("Dragable",&widget_dragable);
		b->IO("HorizontalAnchor",&widget_horizontalAnchor);
		b->IO("HoverTime",&widget_hoverTime);
		b->IO("InheritOpacity",&widget_inheritOpacity);
		b->IO("MaxSize",&widget_maxSize);
		b->IO("MinSize",&widget_minSize);
		b->IO("MoveBaseWidgetOnDrag",&widget_moveBaseWidgetOnDrag);
		b->IO("PositionRelativeToParentClientDimensions",&widget_positionRelativeToParentClientDimensions);
		b->IO("RelativeOpacity",&widget_relativeOpacity);
		b->IO("ResizeFromBottom",&widget_resizeFromBottom);
		b->IO("ResizeFromLeft",&widget_resizeFromLeft);
		b->IO("ResizeFromRight",&widget_resizeFromRight);
		b->IO("ResizeFromTop",&widget_resizeFromTop);
		b->IO("Scrollable",&widget_scrollable);
		b->IO("TransparencyPicking",&widget_transparencyPicking);
		b->IO("VerticalAnchor",&widget_verticalAnchor);
		b->IO("Visible",&widget_visible);
		b->IO("SkinType",&widget_skinTypeName);

		if(b->begin("UserDefinedHandlers","WidgetEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < WIDGET_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<WidgetEvent>(index)),&(widget_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < WIDGET_EVENT_COUNT; ++index)
				{
					if(widget_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<WidgetEvent>(index)),&(widget_userHandlers[index]));
				}
			}
			b->end();
		}
	}

	Widget::Widget(const Ogre::String& widget_name) :
		Serializable(),
		mBeingDragged(false),
		mComponentOfAWidget(false),
		mGrabbed(false),
		mInitialized(false),
		mNameRegisteredWithSheet(false),
		mParentWidget(NULL),
		mSkinType(NULL),
		mSkinElement(NULL),
		mWidgetDesc(NULL),
		mWindow(NULL)
	{
		mName = widget_name;

		mClientDimensions.size = Size::ZERO;
	}

	Widget::~Widget()
	{
		Root::getSingleton().notifyWidgetDestroyed(this);

		FactoryManager::getSingleton().getDescFactory()->destroyInstance(mWidgetDesc);

		// Clean up all user defined event handlers.
		for(int index = 0; index < WIDGET_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mWidgetEventHandlers[index].begin(); it != mWidgetEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void Widget::_createDescObject()
	{
		if(mWidgetDesc != NULL)
			return;

		mWidgetDesc = FactoryManager::getSingleton().getDescFactory()->createInstance<WidgetDesc>(getClass() + "Desc");
		// Assign widget_name, stored from widget creation
		mWidgetDesc->widget_name = mName;

		if(mWidgetDesc->getWidgetClass() != getClass())
			throw Exception(Exception::ERR_INVALID_DESC,"Desc of class \"" + mWidgetDesc->getClass() + "\" is not meant for Widget of class \"" + getClass() + "\"!","Widget::_initialize");
	}

	Widget* Widget::_createWidget(WidgetDesc* d)
	{
		Widget* w = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>(d->getWidgetClass(),d->widget_name);
		
		w->_initialize(d);

		return w;
	}

	void Widget::_initialize(WidgetDesc* d)
	{
		_createDescObject();

		if(mInitialized)
			throw Exception(Exception::ERR_INTERNAL_ERROR,"Widget is already initialized!","Widget::_initialize");

		mInitialized = true;

		for(int index = 0; index < WIDGET_EVENT_COUNT; ++index)
			mWidgetDesc->widget_userHandlers[index] = d->widget_userHandlers[index];

		setBaseColor(d->widget_baseColor);
		setConsumeKeyboardEvents(d->widget_consumeKeyboardEvents);
		setContextMenuName(d->widget_contextMenuName);
		setEnabled(d->widget_enabled);	
		if(d->getClass() != "TitleBar")
		{
			if(d->widget_dimensions.position.x < 0 || d->widget_dimensions.position.y < 0)
			{
				int xPos = d->widget_dimensions.position.x;
				int yPos = d->widget_dimensions.position.y;

				if( xPos > 0 )
					xPos *= Root::getSingletonPtr()->getGUIScale();

				if( yPos > 0 )
					yPos *= Root::getSingletonPtr()->getGUIScale();

				setDimensions(Rect(xPos, yPos,
					d->widget_dimensions.size.width*Root::getSingletonPtr()->getGUIScale(),
					d->widget_dimensions.size.height*Root::getSingletonPtr()->getGUIScale()));
			}
			else
			{
				setDimensions(Rect(d->widget_dimensions.position.x*Root::getSingletonPtr()->getGUIScale(),
					d->widget_dimensions.position.y*Root::getSingletonPtr()->getGUIScale(),
					d->widget_dimensions.size.width*Root::getSingletonPtr()->getGUIScale(),
					d->widget_dimensions.size.height*Root::getSingletonPtr()->getGUIScale()));
			}
		}
		else
		{
			setDimensions(d->widget_dimensions);
		}

		setDragable(d->widget_dragable);
		setHorizontalAnchor(d->widget_horizontalAnchor);
		setHoverTime(d->widget_hoverTime);
		setInheritOpacity(d->widget_inheritOpacity);
		setMaxSize(QuickGUI::Size(d->widget_maxSize.width *Root::getSingletonPtr()->getGUIScale(),
			d->widget_maxSize.height *Root::getSingletonPtr()->getGUIScale()));
		setMinSize(QuickGUI::Size(d->widget_minSize.width *Root::getSingletonPtr()->getGUIScale(),
			d->widget_minSize.height *Root::getSingletonPtr()->getGUIScale()));
		setMoveBaseWidgetOnDrag(d->widget_moveBaseWidgetOnDrag);
		setPositionRelativeToParentClientDimensions(d->widget_positionRelativeToParentClientDimensions);
		setRelativeOpacity(d->widget_relativeOpacity);
		setResizeFromBottom(d->widget_resizeFromBottom);
		setResizeFromLeft(d->widget_resizeFromLeft);
		setResizeFromRight(d->widget_resizeFromRight);
		setResizeFromTop(d->widget_resizeFromTop);
		setScrollable(d->widget_scrollable);
		setSerialize(d->widget_serialize);
		setTransparencyPicking(d->widget_transparencyPicking);
		setVerticalAnchor(d->widget_verticalAnchor);
		setVisible(d->widget_visible);
	}

	void Widget::_setGUIManager(GUIManager* gm)
	{
		mWidgetDesc->guiManager = gm;
	}

	void Widget::_setSheet(Sheet* sheet)
	{
		// If previously sheet pointer is not null, notify the sheet our name is available
		if((mWidgetDesc->sheet != NULL) && mNameRegisteredWithSheet)
		{
			mWidgetDesc->sheet->_notifyNameFreed(mWidgetDesc->widget_name);
			mNameRegisteredWithSheet = false;
		}

		// Update sheet pointer
		mWidgetDesc->sheet = sheet;

		// If new sheet pointer is not null, notify the sheet our name is used
		setName(mWidgetDesc->widget_name);
	}

	void Widget::_setSkinType(const Ogre::String& type)
	{
		if(!SkinTypeManager::getSingleton().hasSkinType(getClass(),type))
			throw Exception(Exception::ERR_SKINNING,"SkinType \"" + type + "\" does not exist!","Widget::_setSkinType");

		mSkinType = SkinTypeManager::getSingleton().getSkinType(getClass(),type);
		updateSkinElement();

		updateClientDimensions();

		redraw();

		WidgetEventArgs wea(this);
		fireWidgetEvent(WIDGET_EVENT_SKIN_CHANGED,wea);
	}

	void Widget::addUserDefinedWidgetEventHandler(WidgetEvent EVENT, const Ogre::String& handlerName)
	{
		mWidgetDesc->widget_userHandlers[EVENT] = handlerName;
	}

	void Widget::addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function)
	{
		mWidgetEventHandlers[EVENT].push_back(function);
	}

	void Widget::clearUserDefinedWidgetEventHandler(WidgetEvent EVENT)
	{
		mWidgetDesc->widget_userHandlers[EVENT] = "";
	}

	void Widget::destroy()
	{
		if(isComponentOfAWidget())
			return;

		if(mParentWidget != NULL)
			dynamic_cast<ContainerWidget*>(mParentWidget)->destroyChild(this);
		else
		{
			if(mWidgetDesc->sheet != NULL)
				mWidgetDesc->sheet->mFreeList.push_back(this);
			else
				Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back(this);
		}
	}

	void Widget::drag(int xOffset, int yOffset)
	{
		mBeingDragged = true;

		if(mWidgetDesc->widget_moveBaseWidgetOnDrag)
		{
			Widget* baseWidget = getBaseWidget();
			if(baseWidget != NULL)
			{
				Point p = baseWidget->getPosition();
				p.translate(Point(xOffset,yOffset));
				baseWidget->setPosition(p);

				WidgetEventArgs wargs(this);
				baseWidget->fireWidgetEvent(WIDGET_EVENT_DRAGGED,wargs);
			}
		}
		else
		{
			Point p = getPosition();
			p.translate(Point(xOffset,yOffset));
			setPosition(p);

			WidgetEventArgs wargs(this);
			fireWidgetEvent(WIDGET_EVENT_DRAGGED,wargs);
		}
	}

	void Widget::draw()
	{
		// check visibility
		if( !mWidgetDesc->widget_visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// check clip region
		Rect clipRegion = brush->getClipRegion();
		if ( clipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size)) == Rect::ZERO )
			return;

		// Set color before drawing operations
		brush->setColor(mWidgetDesc->widget_baseColor);

		// Set opacity before drawing operations
		brush->setOpacity(getAbsoluteOpacity());

		// onDraw
		onDraw();
	}

	Widget* Widget::findFirstWidgetOfClass(const Ogre::String& className)
	{
		if(getClass() == className)
			return this;
		return NULL;
	}

	Widget* Widget::findWidget(const Ogre::String& widget_name)
	{
		if(getName() == widget_name)
			return this;
		return NULL;
	}

	Widget* Widget::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		// If we are not widget_visible, return NULL
		if(!mWidgetDesc->widget_visible)
			return NULL;

		// If we ignore disabled and this widget is !widget_enabled, return NULL
		if(ignoreDisabled && !mWidgetDesc->widget_enabled)
			return NULL;

		// See if point is within widget bounds
		if(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).isPointWithinBounds(p))
		{
			// Take transparency picking into account
			if(mWidgetDesc->widget_transparencyPicking && (mSkinElement != NULL))
			{
				// If the background is transparent, return NULL
				if((mSkinElement == NULL) || (mSkinElement->getTextureName() == ""))
					return NULL;

				// Get relative position
				Point relPos = p - mTexturePosition;
				// Get percentage of position relative to widget widget_dimensions
				relPos.x = relPos.x / mWidgetDesc->widget_dimensions.size.width;
				relPos.y = relPos.y / mWidgetDesc->widget_dimensions.size.height;
				// Get pixel position of texture
				Point pixelPos;
				pixelPos.x = relPos.x * (mSkinElement->getWidth() - 1);
				pixelPos.y = relPos.y * (mSkinElement->getHeight() - 1);
				
				if(mSkinElement->transparentPixel(pixelPos))
					return NULL;
			}
			return this;
		}
		
		return NULL;
	}

	bool Widget::fireWidgetEvent(WidgetEvent EVENT, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mWidgetEventHandlers[EVENT]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mWidgetDesc->widget_userHandlers[EVENT] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mWidgetDesc->widget_userHandlers[EVENT],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	float Widget::getAbsoluteOpacity()
	{
		if(!mWidgetDesc->widget_inheritOpacity || (mParentWidget == NULL))
			return mWidgetDesc->widget_relativeOpacity;

		return mParentWidget->getAbsoluteOpacity() * mWidgetDesc->widget_relativeOpacity;
	}

	Widget* Widget::getBaseWidget()
	{
		Widget* w = this;
		while((w != NULL) && w->isComponentOfAWidget())
			w = w->getParentWidget();

		return w;
	}

	ColourValue Widget::getBaseColor()
	{
		return mWidgetDesc->widget_baseColor;
	}

	bool Widget::getBeingDragged()
	{
		return mBeingDragged;
	}

	BorderSide Widget::getBorderSide(Point p)
	{
		if(!overBorderSide(p))
			return BORDER_NONE;

		// Convert point to relative (to widget) coordinates
		p -= mTexturePosition;

		if(p.x < mSkinElement->getBorderThickness(BORDER_LEFT))
		{
			if(p.y < mSkinElement->getBorderThickness(BORDER_TOP))
				return BORDER_TOP_LEFT;
			if(p.y > (mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM)))
				return BORDER_BOTTOM_LEFT;
			return BORDER_LEFT;
		}

		if(p.x > (mWidgetDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT)))
		{
			if(p.y < mSkinElement->getBorderThickness(BORDER_TOP))
				return BORDER_TOP_RIGHT;
			if(p.y > (mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM)))
				return BORDER_BOTTOM_RIGHT;
			return BORDER_RIGHT;
		}

		if(p.y < mSkinElement->getBorderThickness(BORDER_TOP))
			return BORDER_TOP;

		return BORDER_BOTTOM;
	}

	BrushFilterMode Widget::getBrushFilterMode()
	{
		return mWidgetDesc->widget_brushFilterMode;
	}

	Rect Widget::getClientDimensions()
	{
		return mClientDimensions;
	}

	bool Widget::getConsumeKeyboardEvents()
	{
		return mWidgetDesc->widget_consumeKeyboardEvents;
	}

	Ogre::String Widget::getContextMenuName()
	{
		return mWidgetDesc->widget_contextMenuName;
	}

	Rect Widget::getDimensions()
	{
		return mWidgetDesc->widget_dimensions;
	}

	Ogre::String Widget::getDisabledSkinType()
	{
		return mWidgetDesc->widget_disabledSkinType;
	}

	bool Widget::getDragable()
	{
		return mWidgetDesc->widget_dragable;
	}

	bool Widget::getEnabled()
	{
		return mWidgetDesc->widget_enabled;
	}

	bool Widget::getGrabbed()
	{
		return mGrabbed;
	}

	GUIManager* Widget::getGUIManager()
	{
		return mWidgetDesc->guiManager;
	}

	float Widget::getHeight()
	{
		return mWidgetDesc->widget_dimensions.size.height;
	}

	HorizontalAnchor Widget::getHorizontalAnchor()
	{
		return mWidgetDesc->widget_horizontalAnchor;
	}

	float Widget::getHoverTime()
	{
		return mWidgetDesc->widget_hoverTime;
	}

	bool Widget::getInheritOpacity()
	{
		return mWidgetDesc->widget_inheritOpacity;
	}

	Size Widget::getMaxSize()
	{
		return mWidgetDesc->widget_maxSize;
	}

	Size Widget::getMinSize()
	{
		return mWidgetDesc->widget_minSize;
	}

	bool Widget::getMoveBaseWidgetOnDrag()
	{
		return mWidgetDesc->widget_moveBaseWidgetOnDrag;
	}

	Ogre::String Widget::getName()
	{
		return mWidgetDesc->widget_name;
	}

	Widget* Widget::getParentWidget()
	{
		return mParentWidget;
	}

	Point Widget::getPosition()
	{
		return mWidgetDesc->widget_dimensions.position;
	}

	bool Widget::getPositionRelativeToParentClientDimensions()
	{
		return mWidgetDesc->widget_positionRelativeToParentClientDimensions;
	}

	float Widget::getRelativeOpacity()
	{
		return mWidgetDesc->widget_relativeOpacity;
	}

	bool Widget::getResizeFromBottom()
	{
		return mWidgetDesc->widget_resizeFromBottom;
	}

	bool Widget::getResizeFromLeft()
	{
		return mWidgetDesc->widget_resizeFromLeft;
	}

	bool Widget::getResizeFromRight()
	{
		return mWidgetDesc->widget_resizeFromRight;
	}

	bool Widget::getResizeFromTop()
	{
		return mWidgetDesc->widget_resizeFromTop;
	}

	Point Widget::getScreenPosition()
	{
		if(mParentWidget == NULL)
			return mWidgetDesc->widget_dimensions.position + mScrollOffset;

		// Component widget's positions are not typically relative to their parent's client dimensions.
		// Other widgets can share this functionality also
		if(!mWidgetDesc->widget_positionRelativeToParentClientDimensions)
			return mParentWidget->getScreenPosition() + (mWidgetDesc->widget_dimensions.position - mScrollOffset);
		else
			return mParentWidget->getScreenPosition() + mParentWidget->getClientDimensions().position + (mWidgetDesc->widget_dimensions.position - mScrollOffset);
	}

	Point Widget::getScroll()
	{
		return mScrollOffset;
	}

	bool Widget::getScrollable()
	{
		return mWidgetDesc->widget_scrollable;
	}

	Widget* Widget::getScrollableContainerWidget()
	{
		if(isContainerWidget() && dynamic_cast<ContainerWidget*>(this)->supportsScrolling())
			return this;

		Widget* w = mParentWidget;
		while(w != NULL)
		{
			if(w->isContainerWidget() && dynamic_cast<ContainerWidget*>(w)->supportsScrolling())
				return w;

			w = w->getParentWidget();
		}

		// NULL
		return w;
	}

	bool Widget::getSerialize()
	{
		return mWidgetDesc->widget_serialize;
	}

	Sheet* Widget::getSheet()
	{
		return mWidgetDesc->sheet;
	}

	Size Widget::getSize()
	{
		return mWidgetDesc->widget_dimensions.size;
	}

	Point Widget::getTexturePosition()
	{
		return mTexturePosition;
	}

	bool Widget::getTransparencyPicking()
	{
		return mWidgetDesc->widget_transparencyPicking;
	}

	VerticalAnchor Widget::getVerticalAnchor()
	{
		return mWidgetDesc->widget_verticalAnchor;
	}

	bool Widget::getVisible()
	{
		return mWidgetDesc->widget_visible;
	}

	Ogre::String Widget::getSkinTypeName()
	{
		return mWidgetDesc->widget_skinTypeName;
	}

	float Widget::getWidth()
	{
		return mWidgetDesc->widget_dimensions.size.width;
	}

	Window* Widget::getWindow()
	{
		return mWindow;
	}

	bool Widget::hasEventHandler(WidgetEvent EVENT, void* obj)
	{
		for(std::vector<EventHandlerSlot*>::iterator it = mWidgetEventHandlers[EVENT].begin(); it != mWidgetEventHandlers[EVENT].end(); ++it)
		{
			if((*it)->getClass() == obj)
				return true;
		}

		return false;
	}

	bool Widget::hasAnyEventHandlersForThisType(WidgetEvent EVENT)
	{
		if(mWidgetEventHandlers[EVENT].size() > 0)
		{
			return true;
		}
	
		return false;
	}

	bool Widget::isChildOf(Widget* w)
	{
		// this widget cannot be a parent of a NULL pointer
		if(w == NULL)
			return false;

		Widget* parent = getParentWidget();

		// If I have no parent, I cannot be a parent of the Widget given
		if(parent == NULL)
			return false;

		// If my parent is the widget given, return true
		if(parent == w)
			return true;

		// Recursively call isChildOf on my parent
		return (parent->isChildOf(w));
	}

	bool Widget::isComponentWidget()
	{
		return false;
	}

	bool Widget::isComponentOfAWidget()
	{
		return mComponentOfAWidget;
	}

	bool Widget::isComponentOf(Widget* w)
	{
		Widget* widget = this;
		while((widget != NULL) && widget->isComponentOfAWidget())
		{
			if(widget == w)
				return true;

			widget = widget->getParentWidget();
		}

		if(widget == w)
			return true;
			
		return false;
	}

	bool Widget::isContainerWidget()
	{
		return false;
	}

	bool Widget::isMenuItem()
	{
		return false;
	}

	bool Widget::isPositionManaged()
	{
		return false;
	}

	bool Widget::isSizeManaged()
	{
		return false;
	}

	bool Widget::isToolBarItem()
	{
		return false;
	}

	bool Widget::overBorderSide(Point p)
	{
		// Convert point to relative (to widget) coordinates
		p -= mTexturePosition;

		if((p.x < 0) || (p.x > mWidgetDesc->widget_dimensions.size.width))
			return false;

		if((p.y < 0) || (p.y > mWidgetDesc->widget_dimensions.size.height))
			return false;

		if(mSkinType == NULL)
			return false;

		if((p.x < mSkinElement->getBorderThickness(BORDER_LEFT)) || (p.x > (mWidgetDesc->widget_dimensions.size.width - mSkinElement->getBorderThickness(BORDER_RIGHT))))
			return true;

		if((p.y < mSkinElement->getBorderThickness(BORDER_TOP)) || (p.y > (mWidgetDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM))))
			return true;

		return false;
	}

	void Widget::redraw()
	{
		if(mWindow != NULL)
			mWindow->redraw();
	}

	void Widget::removeEventHandler(WidgetEvent EVENT, void* obj)
	{
		for(std::vector<EventHandlerSlot*>::iterator it = mWidgetEventHandlers[EVENT].begin(); it != mWidgetEventHandlers[EVENT].end(); ++it)
		{
			if((*it)->getClass() == obj)
			{
				EventHandlerSlot* ehs = (*it);
				mWidgetEventHandlers[EVENT].erase(it);
				OGRE_DELETE_T(ehs,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
				return;
			}
		}
	}

	void Widget::resize(BorderSide s, Point position)
	{
		bool changePosition = false;

		Point currentPosition = mWidgetDesc->widget_dimensions.position;

		Point newPosition = mWidgetDesc->widget_dimensions.position;
		
		// Record size before resize
		Size previousSize = mWidgetDesc->widget_dimensions.size;

		// make the given position relative to the Window this widget is located on. 
		Window* parentWindow = getWindow();
		if(parentWindow != NULL)
			position = position - parentWindow->getPosition();

		Point changeInPosition(position - currentPosition);

		switch(s)
		{
		case BORDER_BOTTOM:
			setHeight(changeInPosition.y);
			break;
		case BORDER_TOP_LEFT:
			setSize(Size(previousSize.width - changeInPosition.x,previousSize.height - changeInPosition.y));
			setPosition(currentPosition + Point(previousSize.width - getSize().width,previousSize.height - getSize().height));
			break;
		case BORDER_BOTTOM_LEFT: 
			setSize(Size(previousSize.width - changeInPosition.x,changeInPosition.y));
			setPosition(currentPosition + Point(previousSize.width - getSize().width,0));
			break;
		case BORDER_TOP_RIGHT:
			setSize(Size(changeInPosition.x,previousSize.height - changeInPosition.y));
			setPosition(currentPosition + Point(0,previousSize.height - getSize().height));
			break;
		case BORDER_BOTTOM_RIGHT:
			setSize(Size(changeInPosition.x,changeInPosition.y));
			break;
		case BORDER_LEFT: 
			setWidth(previousSize.width - changeInPosition.x);
			setPosition(currentPosition + Point(previousSize.width - getSize().width,0));
			break;
		case BORDER_RIGHT:
			setWidth(changeInPosition.x);
			break;
		case BORDER_TOP:
			setHeight(previousSize.height - changeInPosition.y);
			setPosition(currentPosition + Point(0,previousSize.height - getSize().height));
			break;
		}
	}

	void Widget::serialize(SerialBase* b)
	{
		// Create Desc object if its not already created.
		_createDescObject();

		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		if(b->isSerialReader())
		{
			// Apply Desc properties
			_initialize(mWidgetDesc);
		}

		b->end();
	}

	void Widget::setBaseColor(ColourValue cv)
	{
		mWidgetDesc->widget_baseColor = cv;

		redraw();
	}

	void Widget::setBrushFilterMode(BrushFilterMode m)
	{
		mWidgetDesc->widget_brushFilterMode = m;

		redraw();
	}

	void Widget::setConsumeKeyboardEvents(bool consume)
	{
		mWidgetDesc->widget_consumeKeyboardEvents = consume;
	}

	void Widget::setContextMenuName(const Ogre::String& contextMenuName)
	{
		mWidgetDesc->widget_contextMenuName = contextMenuName;
	}

	void Widget::setDimensions(const Rect& r)
	{
		setSize(r.size);
		setPosition(r.position);
	}

	void Widget::setDisabledSkinType(const Ogre::String& SkinTypeName)
	{
		if((SkinTypeName != "") && (!SkinTypeManager::getSingleton().hasSkinType(getClass(),SkinTypeName)))
			throw Exception(Exception::ERR_ITEM_NOT_FOUND,"Skin Type \"" + SkinTypeName + "\" does not exist for class \"" + getClass() + "\"!","Widget::setDisabledSkinType");

		mWidgetDesc->widget_disabledSkinType = SkinTypeName;

		if(!mWidgetDesc->widget_enabled)
			redraw();
	}

	void Widget::setDragable(bool dragable)
	{
		mWidgetDesc->widget_dragable = dragable;
	}

	void Widget::setEnabled(bool enabled)
	{
		if(mWidgetDesc->widget_enabled == enabled)
			return;

		mWidgetDesc->widget_enabled = enabled;

		if(mWidgetDesc->widget_disabledSkinType != "")
			_setSkinType(mWidgetDesc->widget_disabledSkinType);
		else
			_setSkinType(mWidgetDesc->widget_skinTypeName);

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_ENABLED_CHANGED,args);
	}

	void Widget::setGrabbed(bool grabbed)
	{
		mGrabbed = grabbed;
	}

	void Widget::setHeight(float pixelHeight)
	{
		// Gaurd against negative sizes or size of 0
		if(pixelHeight < 1.0)
			pixelHeight = 1.0;

		pixelHeight = Ogre::Math::Ceil(pixelHeight);

		// Enforce max height
		if((mWidgetDesc->widget_maxSize.height > 0) && (pixelHeight > mWidgetDesc->widget_maxSize.height))
			pixelHeight = mWidgetDesc->widget_maxSize.height;
		// Enforce min height
		else if(pixelHeight < mWidgetDesc->widget_minSize.height)
			pixelHeight = mWidgetDesc->widget_minSize.height;

		mWidgetDesc->widget_dimensions.size.height = pixelHeight;

		updateClientDimensions();

		redraw();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);
	}

	void Widget::setHorizontalAnchor(HorizontalAnchor a)
	{
		mWidgetDesc->widget_horizontalAnchor = a;
	}

	void Widget::setHoverTime(float seconds)
	{
		mWidgetDesc->widget_hoverTime = seconds;
	}

	void Widget::setInheritOpacity(bool inherit)
	{
		mWidgetDesc->widget_inheritOpacity = inherit;

		redraw();
	}

	void Widget::setMaxSize(const Size& s)
	{
		mWidgetDesc->widget_maxSize = s;

		setSize(mWidgetDesc->widget_dimensions.size);
	}

	void Widget::setMinSize(const Size& s)
	{
		mWidgetDesc->widget_minSize = s;

		setSize(mWidgetDesc->widget_dimensions.size);
	}

	Ogre::String Widget::setName(const Ogre::String& name)
	{
		// If we are attached to a sheet, make sure our name is unique
		if((mWidgetDesc->sheet != NULL) && !isComponentOfAWidget())
		{
			if(mNameRegisteredWithSheet)
				mWidgetDesc->sheet->_notifyNameFreed(mWidgetDesc->widget_name);

			mWidgetDesc->widget_name = mWidgetDesc->sheet->resolveName(name);
			mWidgetDesc->sheet->_notifyNameUsed(name);
			mNameRegisteredWithSheet = true;
		}
		// Otherwise, change the name to the one we passed in
		else
			mWidgetDesc->widget_name = name;

		// Return the new name of the Widget
		return mWidgetDesc->widget_name;
	}

	void Widget::setParent(Widget* parent)
	{
		// Set new parent and window

		mParentWidget = parent;

		if ( mParentWidget )
		{
			mWindow = mParentWidget->mWindow;
			mWidgetDesc->guiManager = mParentWidget->getGUIManager();
			mWidgetDesc->sheet = mParentWidget->getSheet();
		}

		// Update screen rectangle

		updateTexturePosition();
	}

	void Widget::setMoveBaseWidgetOnDrag(bool moveBaseWidget)
	{
		mWidgetDesc->widget_moveBaseWidgetOnDrag = moveBaseWidget;

		if(mWidgetDesc->widget_moveBaseWidgetOnDrag && isComponentOfAWidget())
			setDragable(true);
	}

	void Widget::setPosition(const Point& position)
	{
		mWidgetDesc->widget_dimensions.position = position;
		mWidgetDesc->widget_dimensions.position.round();

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_POSITION_CHANGED,args);
	}

	void Widget::setPositionRelativeToParentClientDimensions(bool relativeToClientOrigin)
	{
		mWidgetDesc->widget_positionRelativeToParentClientDimensions = relativeToClientOrigin;

		updateTexturePosition();
	}

	void Widget::setRelativeOpacity(float opacity)
	{
		if(opacity > 1)
			opacity = 1;
		else if(opacity < 0)
			opacity = 0;

		mWidgetDesc->widget_relativeOpacity = opacity;

		redraw();
	}

	void Widget::setResizeFromAllSides(bool resizable)
	{
		setResizeFromBottom(resizable);
		setResizeFromLeft(resizable);
		setResizeFromRight(resizable);
		setResizeFromTop(resizable);
	}

	void Widget::setResizeFromBottom(bool resizable)
	{
		mWidgetDesc->widget_resizeFromBottom = resizable;
	}

	void Widget::setResizeFromLeft(bool resizable)
	{
		mWidgetDesc->widget_resizeFromLeft = resizable;
	}

	void Widget::setResizeFromRight(bool resizable)
	{
		mWidgetDesc->widget_resizeFromRight = resizable;
	}

	void Widget::setResizeFromTop(bool resizable)
	{
		mWidgetDesc->widget_resizeFromTop = resizable;
	}

	void Widget::setScroll(unsigned int scrollX, unsigned int scrollY)
	{
		mScrollOffset.x = scrollX;
		mScrollOffset.y = scrollY;

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SCROLL_CHANGED,args);
	}

	void Widget::setScrollX(unsigned int scrollX)
	{
		if(scrollX == static_cast<int>(mScrollOffset.x))
			return;

		mScrollOffset.x = scrollX;

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SCROLL_CHANGED,args);
	}

	void Widget::setScrollY(unsigned int scrollY)
	{
		if(scrollY == static_cast<int>(mScrollOffset.y))
			return;

		mScrollOffset.y = scrollY;

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SCROLL_CHANGED,args);
	}

	void Widget::setScrollable(bool scrollable)
	{
		mWidgetDesc->widget_scrollable = scrollable;
	}

	void Widget::setSerialize(bool serialize)
	{
		mWidgetDesc->widget_serialize = serialize;
	}

	void Widget::setSize(Size size)
	{
		// Gaurd against negative sizes or size of 0
		if(size.width < 1.0)
			size.width = 1.0;
		if(size.height < 1.0)
			size.height = 1.0;

		size.roundUp();

		// Enforce max width
		if((mWidgetDesc->widget_maxSize.width > 0) && (size.width > mWidgetDesc->widget_maxSize.width))
			size.width = mWidgetDesc->widget_maxSize.width;
		// Enforce min width
		else if((mWidgetDesc->widget_minSize.width > 0) && (size.width < mWidgetDesc->widget_minSize.width))
			size.width = mWidgetDesc->widget_minSize.width;

		// Enforce min height
		if((mWidgetDesc->widget_maxSize.height > 0) && (size.height > mWidgetDesc->widget_maxSize.height))
			size.height = mWidgetDesc->widget_maxSize.height;
		// Enforce min width
		else if((mWidgetDesc->widget_minSize.height > 0) && (size.height < mWidgetDesc->widget_minSize.height))
			size.height = mWidgetDesc->widget_minSize.height;

		// Update size
		mWidgetDesc->widget_dimensions.size = size;

		updateClientDimensions();

		redraw();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);
	}

	void Widget::setSkinType(const Ogre::String type)
	{
		mWidgetDesc->widget_skinTypeName = type;

		_setSkinType(type);
	}

	void Widget::setTransparencyPicking(bool transparencyPicking)
	{
		mWidgetDesc->widget_transparencyPicking = transparencyPicking;
	}

	void Widget::setVerticalAnchor(VerticalAnchor a)
	{
		mWidgetDesc->widget_verticalAnchor = a;
	}

	void Widget::setVisible(bool visible)
	{
		if(mWidgetDesc->widget_visible == visible)
			return;

		mWidgetDesc->widget_visible = visible;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_VISIBLE_CHANGED,args);

		redraw();
	}

	void Widget::setWidth(float pixelWidth)
	{
		// Gaurd against negative sizes or size of 0
		if(pixelWidth < 1.0)
			pixelWidth = 1.0;

		pixelWidth = Ogre::Math::Ceil(pixelWidth);

		// Enforce max width
		if((mWidgetDesc->widget_maxSize.width > 0) && (pixelWidth > mWidgetDesc->widget_maxSize.width))
			pixelWidth = mWidgetDesc->widget_maxSize.width;
		// Enforce min width
		else if(pixelWidth < mWidgetDesc->widget_minSize.width)
			pixelWidth = mWidgetDesc->widget_minSize.width;

		mWidgetDesc->widget_dimensions.size.width = pixelWidth;

		updateClientDimensions();

		redraw();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);
	}

	void Widget::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinElement != NULL)
		{
			mClientDimensions.position.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = mSkinElement->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);
	}

	void Widget::updateSkinElement()
	{
		mSkinElement = NULL;

		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement("background");
	}

	void Widget::updateTexturePosition()
	{
		// Set the Texture position to ZERO by default.

		mTexturePosition = Point::ZERO;

		// If there is a parent widget, translate by the parent's TexturePosition

		if (mParentWidget)
		{
			mTexturePosition.translate(mParentWidget->getTexturePosition());
			
			// Components in general aren't restricted to Parent's Client widget_dimensions.
			// Other widgets can share this functionality as well.
			if(mWidgetDesc->widget_positionRelativeToParentClientDimensions)
				mTexturePosition.translate(mParentWidget->getClientDimensions().position);
		}			

		// Translate by the Widget's position 
		mTexturePosition.translate(mWidgetDesc->widget_dimensions.position);

		if (mWidgetDesc->widget_scrollable)
			mTexturePosition.translate((mScrollOffset) * -1.0);

		// Force redraw

		redraw();
	}
}
