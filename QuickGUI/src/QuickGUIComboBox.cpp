#include "QuickGUIComboBox.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUISheet.h"
#include "QuickGUIButton.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIListItem.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUIListImageItem.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	const Ogre::String ComboBox::COMBOBOXITEM = "comboboxitem";
	const Ogre::String ComboBox::DEFAULT = "default";
	const Ogre::String ComboBox::DOWN = "down";
	const Ogre::String ComboBox::OVER = "over";
	const Ogre::String ComboBox::DROPDOWNBUTTON = "dropdownbutton";
	const Ogre::String ComboBox::DROPDOWNMENUPANEL = "dropdownmenupanel";

	void ComboBox::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ComboBox");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(DOWN);
		d->defineSkinElement(OVER);
		d->defineComponent(DROPDOWNBUTTON);
		d->defineComponent(DROPDOWNMENUPANEL);
		d->defineComponent(COMBOBOXITEM);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ComboBox",d);
	}

	ComboBoxDesc::ComboBoxDesc() :
		ContainerWidgetDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void ComboBoxDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(125,25);

		combobox_dropDownButton = true;
		combobox_itemHeight = 25;
		combobox_dropDownWidth = 125;
		combobox_dropDownMaxHeight = 0;
		combobox_verticalTextAlignment = TEXT_ALIGNMENT_VERTICAL_CENTER;

		for(int index = 0; index < COMBOBOX_EVENT_COUNT; ++index)
			combobox_userHandlers[index] = "";

		containerwidget_supportScrollBars = false;

		TextUserDesc::resetToDefault();
	}

	void ComboBoxDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("ItemHeight",&combobox_itemHeight);
		b->IO("DropDownButton",&combobox_dropDownButton);
		b->IO("DropDownWidth",&combobox_dropDownWidth);
		b->IO("DropDownMaxHeight",&combobox_dropDownMaxHeight);
		b->IO("VerticalTextAlignment",&combobox_verticalTextAlignment);

		TextUserDesc::serialize(b);

		if(b->begin("UserDefinedHandlers","ComboBoxEvents"))
		{
			if(b->isSerialReader())
			{
				for(int index = 0; index < COMBOBOX_EVENT_COUNT; ++index)
					b->IO(StringConverter::toString(static_cast<ComboBoxEvent>(index)),&(combobox_userHandlers[index]));
			}
			else
			{
				for(int index = 0; index < COMBOBOX_EVENT_COUNT; ++index)
				{
					if(combobox_userHandlers[index] != "")
						b->IO(StringConverter::toString(static_cast<ComboBoxEvent>(index)),&(combobox_userHandlers[index]));
				}
			}
			b->end();
		}
	}

	ComboBox::ComboBox(const Ogre::String& name) :
		ContainerWidget(name),
		TextUser(),
		mMenuPanel(NULL),
		mSelectedItem(NULL),
		mDropDownButton(NULL),
		mAutoNameCounter(0),
		mImageName(""),
		mTileImage(true)
	{
	}

	ComboBox::~ComboBox()
	{
		// If our MenuPanel is not attached to any Sheet, delete it
		if(mWidgetDesc->sheet == NULL)
			FactoryManager::getSingleton().getWidgetFactory()->destroyInstance(mMenuPanel);
		// Else it is attached to Sheet
		else
		{
			// If Sheet is not in its destructor, remove MenuPanel and Destroy it
			if(!mWidgetDesc->sheet->mDeleting)
				mWidgetDesc->sheet->destroyWindow(mMenuPanel->getName());
		}

		// Clean up all user defined event handlers.
		for(int index = 0; index < COMBOBOX_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mComboBoxEventHandlers[index].begin(); it != mComboBoxEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void ComboBox::_initialize(WidgetDesc* d)
	{
		ComboBoxDesc* cbd = dynamic_cast<ComboBoxDesc*>(d);
		cbd->containerwidget_supportScrollBars = false;

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<ComboBoxDesc*>(mWidgetDesc);

		for(int index = 0; index < COMBOBOX_EVENT_COUNT; ++index)
			mDesc->combobox_userHandlers[index] = cbd->combobox_userHandlers[index];

		setSkinType(cbd->widget_skinTypeName);

		setItemHeight(cbd->combobox_itemHeight);

		mDesc->combobox_dropDownButton = cbd->combobox_dropDownButton;
		mDesc->combobox_dropDownWidth = cbd->combobox_dropDownWidth;
		mDesc->combobox_dropDownMaxHeight = cbd->combobox_dropDownMaxHeight;

		// Create our Menu List Window
		MenuPanelDesc* lpd = DescManager::getSingletonPtr()->getDefaultMenuPanelDesc();
		lpd->resetToDefault();
		lpd->widget_name = mWidgetDesc->widget_name + ".MenuPanel";
		lpd->widget_dimensions = Rect(0,0,mDesc->combobox_dropDownWidth,1);
		lpd->widget_visible = false;
		lpd->menupanel_owner = this;
		lpd->menupanel_maxHeight = mDesc->combobox_dropDownMaxHeight;
		mMenuPanel = dynamic_cast<MenuPanel*>(_createWidget(lpd));

		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&ComboBox::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&ComboBox::onMouseLeave,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&ComboBox::onMouseEnter,this);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = cbd->textDesc;
		mDesc->combobox_verticalTextAlignment = cbd->combobox_verticalTextAlignment;
		TextUser::_initialize(this,mDesc);

		mCurrentFontName = Text::getFirstAvailableFont()->getName();
		mCurrentColourValue = ColourValue::White;

		if(mDesc->combobox_dropDownButton)
		{
			ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
			bd->resetToDefault();
			bd->widget_name = ".Button";
			bd->widget_dimensions.size.width = mClientDimensions.size.height;
			bd->widget_dimensions.size.height = mClientDimensions.size.height;
			bd->widget_dimensions.position.x = mClientDimensions.position.x + mClientDimensions.size.width - mClientDimensions.size.height;
			bd->widget_dimensions.position.y = mClientDimensions.position.y;

			mDropDownButton = dynamic_cast<Button*>(_createWidget(bd));
			addComponent(DROPDOWNBUTTON,mDropDownButton);
			
			mDropDownButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&ComboBox::onDropDownButtonDown,this);

			mClientDimensions.size.width -= mClientDimensions.size.height;
		}

		setSkinType(mDesc->widget_skinTypeName);
	}

	void ComboBox::_setGUIManager(GUIManager* gm)
	{
		ContainerWidget::_setGUIManager(gm);

		if(mMenuPanel != NULL)
			mMenuPanel->_setGUIManager(gm);

		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->_setGUIManager(gm);
	}

	void ComboBox::_setSheet(Sheet* sheet)
	{
		if(mWidgetDesc->sheet != NULL)
			mWidgetDesc->sheet->removeWindow(mMenuPanel);

		ContainerWidget::_setSheet(sheet);

		if(sheet != NULL)
			sheet->addWindow(mMenuPanel);

		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->_setSheet(sheet);
	}

	void ComboBox::addUserDefinedComboBoxEventHandler(ComboBoxEvent EVENT, const Ogre::String& handlerName)
	{
		mDesc->combobox_userHandlers[EVENT] = handlerName;
	}

	void ComboBox::addComboBoxEventHandler(ComboBoxEvent EVENT, EventHandlerSlot* function)
	{
		mComboBoxEventHandlers[EVENT].push_back(function);
	}

	void ComboBox::addChild(Widget* w)
	{
		ListItem* i = dynamic_cast<ListItem*>(w);
		i->_setGUIManager(mWidgetDesc->guiManager);
		i->_setSheet(mWidgetDesc->sheet);

		i->setWidth(mMenuPanel->getClientDimensions().size.width);
		i->setHeight(mDesc->combobox_itemHeight);
		i->setSkinType(mSkinType->getSkinReference(COMBOBOXITEM)->typeName);

		// We cannot add the widget as a child, the texture position and drawing will be incorrect.
		int itemIndex = i->getIndex();

		// Adding Item to End of List
		if((itemIndex < 0) || (itemIndex >= static_cast<int>(mItems.size())))
		{
			// Update Index
			i->setIndex(static_cast<int>(mItems.size()));
			
			// Update Position
			if(!mItems.empty())
			{
				Point p = mItems.back()->getPosition();
				p.y += i->getSize().height;
				i->setPosition(p);
			}

			mItems.push_back(i);
		}
		// Inserting Item into the List
		else
		{
			int count = 0;
			for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			{
				if(count == itemIndex)
				{					
					mItems.insert(it,i);
					break;				
				}				
				
				++count;			
			}			
			
			Point p;			
			count = 0;			
			for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)			
			{				
				// Update Index				
				(*it)->setIndex(count);				
				
				// Update Position				
				(*it)->setPosition(p);				
				
				p.y += (*it)->getHeight();
				
				++count;
			}
		}
		
		// Add to the windows list of MenuItems
		mMenuPanel->addWidget(i);
	}

	void ComboBox::clearItems()
	{
		mMenuPanel->removeWidgets();
		mMenuPanel->setVisible(false);

		if(mDesc->sheet != NULL)
		{
			for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
				mDesc->sheet->mFreeList.push_back((*it));
		}
		else
		{
			GUIManager* gm = Root::getSingleton().mGUIManagers.begin()->second;
			for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
				gm->mFreeList.push_back((*it));
		}
		mItems.clear();

		mSelectedItem = NULL;
		mText->clearText();

		redraw();
	}

	void ComboBox::clearSelection()
	{
		mSelectedItem = NULL;
		mText->clearText();

		redraw();
	}

	void ComboBox::clearUserDefinedComboBoxEventHandler(ComboBoxEvent EVENT)
	{
		mDesc->combobox_userHandlers[EVENT] = "";
	}

	ListItem* ComboBox::createItem(ListItemDesc* d, int index)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.Item." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->listitem_index = index;
		
		ListItem* newItem = dynamic_cast<ListItem*>(_createWidget(d));
		addChild(newItem);

		return newItem;
	}

	ListImageItem* ComboBox::createImageItem(const Ogre::String& imageName, int index)
	{
		ListImageItemDesc* d = DescManager::getSingletonPtr()->getDefaultListImageItemDesc();
		d->resetToDefault();
		d->listimageitem_imageName = imageName;

		ListImageItem* i = dynamic_cast<ListImageItem*>(createItem(d,index));

		d->resetToDefault();

		return i;
	}

	ListImageItem* ComboBox::createImageItem(int index)
	{
		return createImageItem("",index);
	}

	ListTextItem* ComboBox::createTextItem(const Ogre::UTFString& text, int index)
	{
		ListTextItemDesc* d = DescManager::getSingletonPtr()->getDefaultListTextItemDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		ListTextItem* i = dynamic_cast<ListTextItem*>(createItem(d,index));

		d->resetToDefault();

		return i;
	}

	ListTextItem* ComboBox::createTextItem(std::vector<TextSegment> segments, int index)
	{
		ListTextItemDesc* d = DescManager::getSingletonPtr()->getDefaultListTextItemDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		ListTextItem* i = dynamic_cast<ListTextItem*>(createItem(d,index));

		d->resetToDefault();

		return i;
	}

	ListTextItem* ComboBox::createTextItem(int index)
	{
		return createTextItem("ListTextItem",index);
	}

	void ComboBox::destroyItem(unsigned int index)
	{
		WidgetFactory* f = FactoryManager::getSingleton().getWidgetFactory();

		bool itemRemovedFromList = false;
		int count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
			{
				mMenuPanel->removeWidget((*it));

				if(mDesc->sheet != NULL)
					mDesc->sheet->mFreeList.push_back((*it));
				else
					Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back((*it));
				mItems.erase(it);

				itemRemovedFromList = true;

				break;
			}

			++count;
		}

		if(!itemRemovedFromList)
			return;

		float yPos = 0;
		count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			// Update index
			(*it)->setIndex(count);

			// Update Position
			Point p = (*it)->getPosition();
			p.y = yPos;
			(*it)->setPosition(p);

			// Store position of next item
			yPos += (*it)->getSize().height;

			++count;
		}

		mMenuPanel->_adjustHeight();

		redraw();
	}

	void ComboBox::destroyItem(ListItem* i)
	{
		destroyItem(i->getIndex());
	}

	Widget* ComboBox::findWidget(const Ogre::String& widget_name)
	{
		Widget* w = ContainerWidget::findWidget(widget_name);
		if(w != NULL)
			return w;

		return mMenuPanel->findWidget(widget_name);
	}

	bool ComboBox::fireComboBoxEvent(ComboBoxEvent e, EventArgs& args)
	{
		bool userDefinedEventFired = false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mComboBoxEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		if(mDesc->combobox_userHandlers[e] != "")
		{
			EventHandlerManager::getSingletonPtr()->executEventHandler(mDesc->combobox_userHandlers[e],args);
			userDefinedEventFired = true;
		}

		return userDefinedEventFired;
	}

	Ogre::String ComboBox::getClass()
	{
		return "ComboBox";
	}

	float ComboBox::getDropDownWidth()
	{
		return mDesc->combobox_dropDownWidth;
	}

	ListItem* ComboBox::getItem(unsigned int index)
	{
		if(index >= mItems.size())
			return NULL;

		unsigned int count = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
				return (*it);

			++count;
		}

		return NULL;
	}

	float ComboBox::getItemHeight()
	{
		return mDesc->combobox_itemHeight;
	}

	float ComboBox::getMaxDropDownHeight()
	{
		return mDesc->combobox_dropDownMaxHeight;
	}

	int ComboBox::getNumberOfItems()
	{
		return static_cast<int>(mItems.size());
	}

	ListItem* ComboBox::getSelectedItem()
	{
		return mSelectedItem;
	}

	Ogre::UTFString ComboBox::getText()
	{
		return mText->getText();
	}

	void ComboBox::hideDropDownList()
	{
		if(mMenuPanel != NULL)
			mMenuPanel->setVisible(false);

		if(mSkinType != NULL)
		{
			mSkinElement = mSkinType->getSkinElement(DEFAULT);

			redraw();
		}
	}

	void ComboBox::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		if(!mText->empty())
		{
			// Center Text Vertically

			float textHeight = mText->getTextHeight();
			float yPos = 0;

			switch(mDesc->combobox_verticalTextAlignment)
			{
			case TEXT_ALIGNMENT_VERTICAL_BOTTOM:
				yPos = mDesc->widget_dimensions.size.height - mSkinElement->getBorderThickness(BORDER_BOTTOM) - textHeight;
				break;
			case TEXT_ALIGNMENT_VERTICAL_CENTER:
				yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);
				break;
			case TEXT_ALIGNMENT_VERTICAL_TOP:
				yPos = mSkinElement->getBorderThickness(BORDER_TOP);
				break;
			}

			// Clip to client dimensions
			Rect clipRegion(mClientDimensions);
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			// Adjust Rect to Text drawing region
			clipRegion = mClientDimensions;
			clipRegion.position.y = yPos;
			clipRegion.translate(mTexturePosition);		

			mText->draw(clipRegion.position);
		}
		else if(mImageName != "")
		{
			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);

			brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

			brush->setTexture(mImageName);
			if(mTileImage)
				brush->drawTiledRectangle(prevClipRegion.getIntersection(clipRegion),UVRect(0,0,1,1));
			else
				brush->drawRectangle(prevClipRegion.getIntersection(clipRegion),UVRect(0,0,1,1));
		}

		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void ComboBox::onDropDownButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(!mMenuPanel->getVisible())
			{
				showDropDownList();
			}
			else
			{
				hideDropDownList();
				mSkinElement = mSkinType->getSkinElement(OVER);
			}

			redraw();
		}
	}

	void ComboBox::onMouseEnter(const EventArgs& args)
	{
		if(!mMenuPanel->getVisible())
			mSkinElement = mSkinType->getSkinElement(OVER);
		else
			mSkinElement = mSkinType->getSkinElement(DOWN);

		redraw();
	}

	void ComboBox::onMouseLeave(const EventArgs& args)
	{
		if(!mMenuPanel->getVisible())
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
		else
			mSkinElement = mSkinType->getSkinElement(DOWN);

		redraw();
	}

	void ComboBox::onMouseButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(!mMenuPanel->getVisible())
			{
				mSkinElement = mSkinType->getSkinElement(DOWN);
				showDropDownList();
			}
			else
			{
				hideDropDownList();
				mSkinElement = mSkinType->getSkinElement(OVER);
			}

			redraw();
		}
	}

	void ComboBox::selectItem(const MouseEventArgs& mea)
	{
		mText->clearText();
		mImageName = "";

		ListItem* i = dynamic_cast<ListItem*>(mea.widget);
		if(i != NULL)
		{
			mSelectedItem = i;

			if(mSelectedItem->getClass() == "ListTextItem")
				mText->setText(dynamic_cast<ListTextItem*>(mSelectedItem)->getTextSegments());

			if(mSelectedItem->getClass() == "ListImageItem")
			{
				mImageName = dynamic_cast<ListImageItem*>(mSelectedItem)->getImageName();
				mTileImage = dynamic_cast<ListImageItem*>(mSelectedItem)->getTileImage();
			}
		}

		WidgetEventArgs args(this);
		fireComboBoxEvent(COMBOBOX_EVENT_SELECTION_CHANGED,args);

		hideDropDownList();

		redraw();
	}

	void ComboBox::selectItem(unsigned int index)
	{
		mSelectedItem = getItem(index);

		mText->clearText();
		mImageName = "";

		if(mSelectedItem != NULL)
		{
			if(mSelectedItem->getClass() == "ListTextItem")
				mText->setText(dynamic_cast<ListTextItem*>(mSelectedItem)->mText->getTextSegments());

			if(mSelectedItem->getClass() == "ListImageItem")
			{
				mImageName = dynamic_cast<ListImageItem*>(mSelectedItem)->getImageName();
				mTileImage = dynamic_cast<ListImageItem*>(mSelectedItem)->getTileImage();
			}
		}

		WidgetEventArgs args(this);
		fireComboBoxEvent(COMBOBOX_EVENT_SELECTION_CHANGED,args);

		redraw();
	}

	void ComboBox::serialize(SerialBase* b)
	{
		// Create Desc object if its not already created.
		_createDescObject();

		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		if(b->isSerialReader())
			// Apply Desc properties
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
				for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
				{
					(*it)->serialize(b);
				}
			}
			b->end();
		}

		b->end();
	}

	void ComboBox::setDropDownWidth(float width)
	{
		mDesc->combobox_dropDownWidth = width;

		mMenuPanel->setWidth(mDesc->combobox_dropDownWidth);
	}

	void ComboBox::setItemHeight(float height)
	{
		mDesc->combobox_itemHeight = height;

		int y = 0;
		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			(*it)->setHeight(mDesc->combobox_itemHeight);
			y += mDesc->combobox_itemHeight;
		}

		if(mMenuPanel != NULL)
			mMenuPanel->_adjustHeight();
	}

	void ComboBox::setMaxDropDownHeight(float height)
	{
		mDesc->combobox_dropDownMaxHeight = height;

		if(mMenuPanel != NULL)
			mMenuPanel->setMaxHeight(height);
	}

	void ComboBox::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getSkinReference((*it).first)->typeName);

		if(mMenuPanel != NULL)
			mMenuPanel->setSkinType(mSkinType->getSkinReference(DROPDOWNMENUPANEL)->typeName);

		for(std::list<ListItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->setSkinType(mSkinType->getSkinReference(COMBOBOXITEM)->typeName);
	}

	void ComboBox::showDropDownList()
	{
		// Determine if list should be shown above or below ComboBox
		Point p = getScreenPosition();

		if((p.y + mDesc->widget_dimensions.size.height + mMenuPanel->getHeight()) > mDesc->sheet->getHeight())
		{
			mMenuPanel->setPosition(Point(p.x,p.y - mMenuPanel->getHeight()));
		}
		else
		{
			mMenuPanel->setPosition(Point(p.x,p.y + mDesc->widget_dimensions.size.height));
		}

		mMenuPanel->setVisible(true);

		mSkinElement = mSkinType->getSkinElement(DOWN);
	}

	void ComboBox::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinElement != NULL)
		{
			mClientDimensions.position.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = mSkinElement->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));

			if(mDropDownButton != NULL)
			{
				mDropDownButton->setWidth(mClientDimensions.size.height);
				mDropDownButton->setHeight(mClientDimensions.size.height);
				mDropDownButton->setPosition(Point(mClientDimensions.position.x + mClientDimensions.size.width - mClientDimensions.size.height,mClientDimensions.position.y));

				mClientDimensions.size.width = mDropDownButton->getPosition().x;
			}	
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		hideDropDownList();
	}

	void ComboBox::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
	}

	void ComboBox::updateTexturePosition()
	{
		ContainerWidget::updateTexturePosition();

		hideDropDownList();
	}
}
