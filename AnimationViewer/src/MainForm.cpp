#include "MainForm.h"

// I got tired of typing "QuickGUI::" in front of everything..
using namespace QuickGUI;

namespace AnimationViewer
{
	MainForm::MainForm(AnimationViewerApp* app, Ogre::RenderWindow* w) :
		Window(app,w),
		mBase(NULL),
		mEntity(NULL),
		mAnimationState(NULL),
		mGridMaterial("AnimationViewer.Grid.Material"),
		mDefaultCameraRTTName("AnimationViewer.DefaultCam.RTT"),
		mSideCameraRTTName("AnimationViewer.SideCam.RTT"),
		mTopCameraRTTName("AnimationViewer.TopCam.RTT"),
		mFrontCameraRTTName("AnimationViewer.FrontCam.RTT"),
		mDistanceToMesh(0),
		mFrontCamera(NULL),
		mSideCamera(NULL),
		mTopCamera(NULL),
		mPlayButton(NULL),
		mPauseButton(NULL),
		mResumeButton(NULL),
		mStopButton(NULL),
		mLoopCheckBox(NULL),
		mTimePositionTextBox(NULL),
		mTimePositionScrollBar(NULL),
		mAnimationSpeedScrollBar(NULL),
		mPaused(false),
		mAnimationSpeed(1.0),
		mPlaying(false)
	{
		Root::getSingleton().setDefaultFontName("micross.12");

		_setupScene();
		_defineGUI();
		_defineGrids();
	}

	MainForm::~MainForm()
	{
	}

	void MainForm::_createRenderToTexture(Ogre::Camera* cam, const std::string& RTTName, unsigned int width, unsigned int height, unsigned int visibilityFilter)
	{
		Ogre::TextureManager* tm = Ogre::TextureManager::getSingletonPtr();
		if(tm->resourceExists(RTTName))
			return;

		Ogre::TexturePtr texture = tm->createManual(RTTName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, width, height, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

		Ogre::RenderTexture* renderTexture = texture->getBuffer()->getRenderTarget();

		Ogre::Viewport* vp = renderTexture->addViewport(cam);
		vp->setVisibilityMask(visibilityFilter);
		renderTexture->getViewport(0)->setClearEveryFrame(true);
		renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
		renderTexture->getViewport(0)->setOverlaysEnabled(false);
	}

	void MainForm::_defineXZGrid(Ogre::ManualObject* mo, float left, float right, float back, float front)
	{
		float increment = 1;

		mo->clear();
		mo->begin(mGridMaterial,Ogre::RenderOperation::OT_LINE_LIST);

		float tempBack = back;

		while(tempBack < front)
		{
			QuickGUI::ColourValue cv = QuickGUI::ColourValue::White;
			if(static_cast<int>(tempBack) % 10 == 0)
				cv = QuickGUI::ColourValue::Blue;
			else if(static_cast<int>(tempBack) % 5 == 0)
				cv = QuickGUI::ColourValue::Green;

			mo->position(left,0,tempBack);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			mo->position(right,0,tempBack);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			tempBack += increment;
		}

		while(left < right)
		{
			QuickGUI::ColourValue cv = QuickGUI::ColourValue::White;
			if(static_cast<int>(left) % 10 == 0)
				cv = QuickGUI::ColourValue::Blue;
			else if(static_cast<int>(left) % 5 == 0)
				cv = QuickGUI::ColourValue::Green;

			mo->position(left,0,back);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			mo->position(left,0,front);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			left += increment;
		}

		mo->end();
	}

	void MainForm::_defineXYGrid(Ogre::ManualObject* mo, float left, float right, float top, float bottom)
	{
		float increment = 1;

		mo->clear();
		mo->begin(mGridMaterial,Ogre::RenderOperation::OT_LINE_LIST);

		float tempBottom = bottom;

		while(tempBottom < top)
		{
			QuickGUI::ColourValue cv = QuickGUI::ColourValue::White;
			if(static_cast<int>(tempBottom) % 10 == 0)
				cv = QuickGUI::ColourValue::Blue;
			else if(static_cast<int>(tempBottom) % 5 == 0)
				cv = QuickGUI::ColourValue::Green;

			mo->position(left,tempBottom,0);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			mo->position(right,tempBottom,0);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			tempBottom += increment;
		}

		while(left < right)
		{
			QuickGUI::ColourValue cv = QuickGUI::ColourValue::White;
			if(static_cast<int>(left) % 10 == 0)
				cv = QuickGUI::ColourValue::Blue;
			else if(static_cast<int>(left) % 5 == 0)
				cv = QuickGUI::ColourValue::Green;

			mo->position(left,top,0);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			mo->position(left,bottom,0);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			left += increment;
		}

		mo->end();
	}

	void MainForm::_defineYZGrid(Ogre::ManualObject* mo, float top, float bottom, float back, float front)
	{
		float increment = 1;

		mo->clear();
		mo->begin(mGridMaterial,Ogre::RenderOperation::OT_LINE_LIST);

		float tempBottom = bottom;

		while(tempBottom < top)
		{
			QuickGUI::ColourValue cv = QuickGUI::ColourValue::White;
			if(static_cast<int>(tempBottom) % 10 == 0)
				cv = QuickGUI::ColourValue::Blue;
			else if(static_cast<int>(tempBottom) % 5 == 0)
				cv = QuickGUI::ColourValue::Green;

			mo->position(0,tempBottom,front);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			mo->position(0,tempBottom,back);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			tempBottom += increment;
		}

		while(back < front)
		{
			QuickGUI::ColourValue cv = QuickGUI::ColourValue::White;
			if(static_cast<int>(back) % 10 == 0)
				cv = QuickGUI::ColourValue::Blue;
			else if(static_cast<int>(back) % 5 == 0)
				cv = QuickGUI::ColourValue::Green;

			mo->position(0,top,back);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			mo->position(0,bottom,back);
			mo->colour(cv.r,cv.g,cv.b,cv.a);

			back += increment;
		}

		mo->end();
	}

	void MainForm::_defineGrids()
	{
		_defineXZGrid(mXZGrid,-500,500,-500,500);
		_defineXYGrid(mXYGrid,-500,500,500,-500);
		_defineYZGrid(mYZGrid,500,-500,-500,500);
	}

	void MainForm::_defineGUI()
	{
		using namespace QuickGUI;

		Ogre::ResourceManager::ResourceMapIterator it = Ogre::FontManager::getSingleton().getResourceIterator();
		while(it.hasMoreElements())
		{
			Ogre::FontPtr fp = static_cast<Ogre::FontPtr>(it.getNext());
			float fontHeight = Text::getFontHeight(fp.getPointer());
			if((fontHeight > 15) && (fontHeight < 20))
			{
				Root::getSingleton().setDefaultFontName(fp->getName());
				break;
			}
		}

		DescManager* dm = DescManager::getSingletonPtr();

		/// CREATE SHEET

		SheetDesc* sd = dm->getDefaultSheetDesc();
		sd->resetToDefault();
		sd->widget_name = "Gaia.AnimationViewerSheet";
		sd->widget_dimensions.size = Size(800,600);
		mSheet = SheetManager::getSingleton().createSheet(sd);
		mSheet->addWidgetEventHandler(WIDGET_EVENT_SIZE_CHANGED,&MainForm::onSheetResized,this);

		/// CREATE CONTROL PANEL

		PanelDesc* pd = dm->getDefaultPanelDesc();
		pd->resetToDefault();
		pd->widget_name = "AnimationViewerControlPanel";
		pd->widget_dimensions = Rect(0,450,600,150);
		pd->containerwidget_supportScrollBars = false;
		mControlPanel = mSheet->createPanel(pd);

		/// CREATE IMAGES

		ImageDesc* id = dm->getDefaultImageDesc();
		id->resetToDefault();
		id->image_updateEveryFrame = true;
		id->widget_name = "TopView";
		id->image_imageName = mTopCameraRTTName;
		id->widget_dimensions.size = Size(300,225);
		mTopViewImage = mSheet->createImage(id);

		id->widget_name = "SideView";
		id->widget_dimensions.position = Point(300,0);
		id->image_imageName = mSideCameraRTTName;
		mSideViewImage = mSheet->createImage(id);

		id->widget_name = "FrontView";
		id->widget_dimensions.position = Point(0,225);
		id->image_imageName = mFrontCameraRTTName;
		mFrontViewImage = mSheet->createImage(id);

		id->widget_name = "PerspectiveView";
		id->widget_dimensions.position = Point(300,225);
		id->image_imageName = mDefaultCameraRTTName;
		mPerspectiveViewImage = mSheet->createImage(id);

		/// CREATE LIST

		ListDesc* listDesc = dm->getDefaultListDesc();
		listDesc->resetToDefault();
		listDesc->widget_name = "MeshSelectionList";
		listDesc->widget_dimensions.position = Point(600,0);
		listDesc->widget_dimensions.size = Size(200,600);
		listDesc->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		listDesc->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
		mMeshList = mSheet->createList(listDesc);

		Ogre::StringVector groupNamesList = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
		Ogre::vector<Ogre::String>::type::const_iterator itGroup = groupNamesList.begin();
		while(itGroup != groupNamesList.end())
		{
			Ogre::StringVectorPtr resourceNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames(*itGroup,"*.mesh" );

			Ogre::vector<Ogre::String>::type::const_iterator itResourceName = resourceNames->begin();
			while(itResourceName != resourceNames->end())
			{
				mMeshList->createTextItem(*itResourceName);

				++itResourceName;
			}

			++itGroup;
		}

		mMeshList->addListEventHandler(LIST_EVENT_SELECTION_CHANGED,&MainForm::onSelectedChanged,this);

		/// LABELS
		
		LabelDesc* labelDesc = dm->getDefaultLabelDesc();
		labelDesc->resetToDefault();

		labelDesc->widget_dimensions.position = Point(5,205);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment(ColourValue::White,"Top View"));
		mTopViewLabel = mSheet->createLabel(labelDesc);

		labelDesc->widget_dimensions.position = Point(305,205);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment(ColourValue::White,"Side View"));
		mSideViewLabel = mSheet->createLabel(labelDesc);

		labelDesc->widget_dimensions.position = Point(5,430);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment(ColourValue::White,"Front View"));
		mFrontViewLabel = mSheet->createLabel(labelDesc);

		labelDesc->widget_dimensions.position = Point(305,430);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment(ColourValue::White,"Perspective View"));
		mPerspectiveViewLabel = mSheet->createLabel(labelDesc);

		labelDesc->widget_dimensions.position = Point(600,0);
		labelDesc->textDesc.segments.clear();
		labelDesc->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Mesh List"));
		mSheet->createLabel(labelDesc);

		/// TOOLBAR

		ToolBar* toolBar = mSheet->createToolBar(QuickGUI::Rect(0,0,600,25));
		toolBar->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		toolBar->createMenu("Menu")->createTextItem("Close")->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onCloseButton,this);

		/// CONTROL PANEL

		listDesc->widget_name = "AnimationSelectionList";
		listDesc->widget_dimensions.position = Point(0,0);
		listDesc->widget_dimensions.size = Size(200,mControlPanel->getClientDimensions().size.height);
		listDesc->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		mAnimationList = mControlPanel->createList(listDesc);
		mAnimationList->addListEventHandler(LIST_EVENT_SELECTION_CHANGED,&MainForm::onAnimationSelectionChanged,this);

		/// BUTTONS

		ButtonDesc* bd = dm->getDefaultButtonDesc();

		bd->resetToDefault();
		bd->widget_dimensions.position = Point(200,0);
		bd->widget_dimensions.size = Size(95,25);
		bd->textDesc.segments.clear();
		bd->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Play"));
		mPlayButton = mControlPanel->createButton(bd);
		mPlayButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onPlay,this);

		bd->widget_dimensions.position = Point(295,0);
		bd->textDesc.segments.clear();
		bd->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Stop"));
		mStopButton = mControlPanel->createButton(bd);
		mStopButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onStop,this);

		bd->widget_dimensions.position = Point(390,0);
		bd->textDesc.segments.clear();
		bd->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Pause"));
		mPauseButton = mControlPanel->createButton(bd);
		mPauseButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onPause,this);

		bd->widget_dimensions.position = Point(485,0);
		bd->textDesc.segments.clear();
		bd->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Resume"));
		mResumeButton = mControlPanel->createButton(bd);
		mResumeButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&MainForm::onResume,this);

		CheckBoxDesc* cbd = dm->getDefaultCheckBoxDesc();
		cbd->resetToDefault();
		cbd->widget_dimensions.position = Point(205,30);
		cbd->widget_dimensions.size = Size(25,25);
		cbd->checkbox_checked = true;
		mLoopCheckBox = mControlPanel->createCheckBox(cbd);
		mLoopCheckBox->addCheckBoxEventHandler(CHECKBOX_EVENT_CHECK_CHANGED,&MainForm::onLoopCheckChanged,this);

//		labelDesc->widget_name = "";
		labelDesc->widget_dimensions.position = Point(235,35);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Loop"));
		mControlPanel->createLabel(labelDesc);

		labelDesc->widget_dimensions.position = Point(450,35);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Time Position:"));
		mControlPanel->createLabel(labelDesc);

		TextBoxDesc* tbd = dm->getDefaultTextBoxDesc();
		tbd->resetToDefault();
		tbd->widget_dimensions = Rect(535,30,40,25);
		tbd->textbox_defaultColor = QuickGUI::ColourValue::White;
		tbd->textbox_defaultFontName = "micross.12";
		mTimePositionTextBox = mControlPanel->createTextBox(tbd);
		mTimePositionTextBox->setTextInputValidator(&MainForm::timeInputValidator,this);
		mTimePositionTextBox->addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&MainForm::onTextBoxEnter,this);

		labelDesc->widget_dimensions.position = Point(205,65);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"0"));
		mControlPanel->createLabel(labelDesc);

		HScrollBarDesc* hsbd = dm->getDefaultHScrollBarDesc();
		hsbd->resetToDefault();
		hsbd->widget_dimensions = Rect(220,60,310,25);
		mTimePositionScrollBar = mControlPanel->createHScrollBar(hsbd);
		mTimePositionScrollBar->addScrollBarEventHandler(SCROLLBAR_EVENT_ON_SCROLLED,&MainForm::onAnimationPositionScrolled,this);
		mTimePositionScrollBar->addScrollBarEventHandler(SCROLLBAR_EVENT_SLIDER_DRAGGED,&MainForm::onAnimationSliderDragged,this);

		labelDesc->widget_dimensions.position = Point(535,65);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"      "));
		mMaxTimePositionLabel = mControlPanel->createLabel(labelDesc);

		labelDesc->widget_dimensions.position = Point(205,90);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Animation Speed:"));
		mControlPanel->createLabel(labelDesc);

		labelDesc->widget_dimensions.position = Point(205,110);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"0x"));
		mControlPanel->createLabel(labelDesc);

		hsbd->widget_dimensions = Rect(220,105,315,25);
		hsbd->hscrollbar_sliderPercentage = 0.5;
		mAnimationSpeedScrollBar = mControlPanel->createHScrollBar(hsbd);
		mAnimationSpeedScrollBar->addScrollBarEventHandler(SCROLLBAR_EVENT_ON_SCROLLED,&MainForm::onAnimationSpeedScrolled,this);

		labelDesc->widget_dimensions.position = Point(540,110);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"2x"));
		mControlPanel->createLabel(labelDesc);

		labelDesc->widget_dimensions.position = Point(0,0);
		labelDesc->textDesc.segments.clear();
		labelDesc->textDesc.segments.push_back(TextSegment("micross.12",QuickGUI::ColourValue::White,"Animation List"));
		mControlPanel->createLabel(labelDesc);

		mGUIManager->setActiveSheet(mSheet);
	}

	void MainForm::_setupScene()
	{
		/// CREATE GRID MATERIAL
		if(!Ogre::MaterialManager::getSingleton().resourceExists(mGridMaterial))
		{
			Ogre::MaterialPtr mp = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(mGridMaterial,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,true));

			Ogre::Technique* tech = mp->getTechnique(0); 
			Ogre::Pass* pass = tech->getPass(0); 
			pass->setLightingEnabled(false); 
			pass->setAmbient(Ogre::ColourValue::White); 
			pass->setDiffuse(Ogre::ColourValue::White);
		}

		mBase = mSceneManager->getRootSceneNode()->createChildSceneNode();

		/// SETUP CAMERAS

		mDefaultCamera = mCamera;
		mDefaultCamera->setNearClipDistance(0.1);
		_createRenderToTexture(mDefaultCamera,mDefaultCameraRTTName,600,600,(1 << 31) | (1 << 30));

		mSideCamera = mSceneManager->createCamera("AnimationViewer.SideCamera");
		mSideCamera->setNearClipDistance(0.1);
		mSideCamera->yaw(Ogre::Radian(Ogre::Degree(-90)));
		_createRenderToTexture(mSideCamera,mSideCameraRTTName,600,600,(1 << 31) | (1 << 28));

		//mSideCamYawNode = mSceneManager->getRootSceneNode()->createChildSceneNode();


		mTopCamera = mSceneManager->createCamera("AnimationViewer.TopCamera");
		mTopCamera->setNearClipDistance(0.1);
		mTopCamera->pitch(Ogre::Radian(Ogre::Degree(-90)));
		_createRenderToTexture(mTopCamera,mTopCameraRTTName,600,600,(1 << 31) | (1 << 30));

		mFrontCamera = mSceneManager->createCamera("AnimationViewer.FrontCamera");
		mFrontCamera->setNearClipDistance(0.1);
		_createRenderToTexture(mFrontCamera,mFrontCameraRTTName,600,600,(1 << 31) | (1 << 29));

		mDefaltCamOriNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		mDefaultCamDistNode = mDefaltCamOriNode->createChildSceneNode();
		mDefaultCamDistNode->attachObject(mDefaultCamera);

		/// SETUP GRID OBJECTS
		mXZGrid = mSceneManager->createManualObject("AnimationViewer.XZGrid");
		mXZGrid->setVisibilityFlags(1 << 30);
		mXZGrid->setVisible(false);
		mSceneManager->getRootSceneNode()->attachObject(mXZGrid);
		
		mXYGrid = mSceneManager->createManualObject("AnimationViewer.XYGrid");
		mXYGrid->setVisibilityFlags(1 << 29);
		mXYGrid->setVisible(false);
		mSceneManager->getRootSceneNode()->attachObject(mXYGrid);
		
		mYZGrid = mSceneManager->createManualObject("AnimationViewer.YZGrid");
		mYZGrid->setVisibilityFlags(1 << 28);
		mYZGrid->setVisible(false);
		mSceneManager->getRootSceneNode()->attachObject(mYZGrid);
	}

	void MainForm::_viewMesh(const std::string& meshName)
	{
		mBase->setScale(1,1,1);
		mBase->setOrientation(Ogre::Quaternion::IDENTITY);
		mBase->setPosition(0,0,0);

		mAnimationList->clearItems();
		mAnimationState = NULL;
		mTimePositionScrollBar->setPercentage(0);

		mBase->detachAllObjects();
		if(mEntity != NULL)
			mSceneManager->destroyEntity(mEntity);
		mAnimationState = NULL;
		enableAnimationControls(false);

		if(meshName == "")
		{
			mXZGrid->setVisible(false);
			mXYGrid->setVisible(false);
			mYZGrid->setVisible(false);
		}
		else
		{
			mEntity = mSceneManager->createEntity(meshName);
			mBase->attachObject(mEntity);

			QuickGUI::ListTextItemDesc* listTextDesc = QuickGUI::DescManager::getSingletonPtr()->getDefaultListTextItemDesc();

			Ogre::AnimationStateSet* set = mEntity->getAllAnimationStates();
			if(set != NULL)
			{
				Ogre::AnimationStateIterator itr = set->getAnimationStateIterator();
				while(itr.hasMoreElements())
				{
					listTextDesc->textDesc.segments.clear();
					listTextDesc->textDesc.segments.push_back(QuickGUI::TextSegment("micross.12",QuickGUI::ColourValue::White,itr.getNext()->getAnimationName()));
					mAnimationList->createItem(listTextDesc);
				}
			}

			float nearPlane = mDefaultCamera->getNearClipDistance();
			Ogre::Radian theta = mDefaultCamera->getFOVy() * 0.5;
			// get minimum dimension from aspect 
			float aspectRatio = mDefaultCamera->getAspectRatio();
			if (aspectRatio < 1.0f) 
				theta *= aspectRatio;

			float boundingRadius = mEntity->getBoundingRadius();
			mDistanceToMesh = (boundingRadius / Ogre::Math::Sin(theta)) + nearPlane;

			// Position cameras
			mFrontCamera->setPosition(0,0,mDistanceToMesh);
			mSideCamera->setPosition(-mDistanceToMesh,0,0);
			mTopCamera->setPosition(0,mDistanceToMesh,0);
			
			mDefaltCamOriNode->setOrientation(Ogre::Quaternion::IDENTITY);
			mDefaultCamDistNode->setPosition(0,0,mDistanceToMesh);
			mDefaltCamOriNode->yaw(Ogre::Radian(Ogre::Degree(45)));
			mDefaltCamOriNode->pitch(Ogre::Radian(Ogre::Degree(-45)));

			mXZGrid->setVisible(true);
			mXYGrid->setVisible(true);
			mYZGrid->setVisible(true);
		}
	}

	void MainForm::enableAnimationControls(bool enabled)
	{
		mPlayButton->setEnabled(enabled);
		mStopButton->setEnabled(enabled);
		mPauseButton->setEnabled(enabled);
		mResumeButton->setEnabled(enabled);
		mLoopCheckBox->setEnabled(enabled);
		mTimePositionTextBox->setEnabled(enabled);
		mTimePositionScrollBar->setEnabled(enabled);
		mAnimationSpeedScrollBar->setEnabled(enabled);
	}

	void MainForm::onExitPressed(const QuickGUI::EventArgs& args)
	{
		mApp->exit();
	}

	void MainForm::onAnimationSelectionChanged(const QuickGUI::EventArgs& args)
	{
		mTimePositionScrollBar->setPercentage(0);

		if(mAnimationState != NULL)
		{
			mAnimationState->setTimePosition(0);
			mAnimationState->setEnabled(false);
			
			mAnimationState = NULL;
		}

		if(mAnimationList->getSelection().empty())
		{
			enableAnimationControls(false);
			mMaxTimePositionLabel->clearText();
		}
		else
		{
			enableAnimationControls(true);
			Ogre::String animationName = dynamic_cast<QuickGUI::ListTextItem*>(mAnimationList->getSelection().front())->getText();
			mAnimationState = mEntity->getAnimationState(animationName);
			mAnimationState->setLoop(mLoopCheckBox->getChecked());
			mAnimationState->setEnabled(true);
			mMaxTimePositionLabel->setText(Ogre::StringConverter::toString(mAnimationState->getLength()),QuickGUI::Text::getFont("micross.12"),QuickGUI::ColourValue::White);
		}
	}

	void MainForm::onSelectedChanged(const QuickGUI::EventArgs& args)
	{
		std::list<QuickGUI::ListItem*> selectedItems = mMeshList->getSelection();

		if(selectedItems.empty())
			_viewMesh("");
		else
		{
			QuickGUI::ListTextItem* lti = dynamic_cast<QuickGUI::ListTextItem*>(selectedItems.front());
			_viewMesh(lti->getText());
		}		
	}

	void MainForm::onTextBoxEnter(const QuickGUI::EventArgs& args)
	{
		const QuickGUI::KeyEventArgs kea = dynamic_cast<const QuickGUI::KeyEventArgs&>(args);

		if(kea.scancode == QuickGUI::KC_RETURN)
		{
			if(mAnimationState != NULL)
			{
				mAnimationState->setTimePosition(Ogre::StringConverter::parseReal(mTimePositionTextBox->getText()));
				mTimePositionScrollBar->setPercentage(mAnimationState->getTimePosition() / mAnimationState->getLength());
			}
		}
	}

	void MainForm::onPlay(const QuickGUI::EventArgs& args)
	{
		if(mAnimationState != NULL)
		{
			mAnimationState->setEnabled(true);
			mAnimationState->setTimePosition(0);
			mAnimationState->setLoop(mLoopCheckBox->getChecked());

			mPlaying = true;
			mPaused = false;
		}
	}

	void MainForm::onPause(const QuickGUI::EventArgs& args)
	{
		mPaused = true;
		mPlaying = false;
	}

	void MainForm::onResume(const QuickGUI::EventArgs& args)
	{
		mPaused = false;
		mPlaying = true;
	}

	void MainForm::onStop(const QuickGUI::EventArgs& args)
	{
		if(mAnimationState != NULL)
		{
			mAnimationState->setTimePosition(0);
			mAnimationState->setEnabled(false);

			mPlaying = false;
			mPaused = false;
		}
	}

	void MainForm::onLoopCheckChanged(const QuickGUI::EventArgs& args)
	{
		if(mAnimationState != NULL)
			mAnimationState->setLoop(mLoopCheckBox->getChecked());
	}

	void MainForm::onAnimationPositionScrolled(const QuickGUI::EventArgs& args)
	{
		float timePosition = 0;
		if(mAnimationState != NULL)
		{
			timePosition = mTimePositionScrollBar->getPercentage() * mAnimationState->getLength();
			
			mAnimationState->setTimePosition(timePosition);
		}			

		// Update TextBox
		mTimePositionTextBox->setText(Ogre::StringConverter::toString(timePosition),QuickGUI::Text::getFont("micross.12"),QuickGUI::ColourValue::White);
	}

	void MainForm::onAnimationSliderDragged(const QuickGUI::EventArgs& args)
	{
		mPlaying = false;
	}

	void MainForm::onAnimationSpeedScrolled(const QuickGUI::EventArgs& args)
	{
		mAnimationSpeed = mAnimationSpeedScrollBar->getPercentage() * 2.0;
	}

	bool MainForm::timeInputValidator(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText)
	{
		// Limit to 6 characters
		if(currentText.size() > static_cast<Ogre::UTFString::size_type>(5))
			return false;

		// Only one period allowed
		if(cp == 46)
		{
			if(currentText.find(static_cast<Ogre::UTFString::unicode_char>(46)) != Ogre::UTFString::npos)
				return false;
			else
				return true;
		}

		// Only numbers are valid input
		if((cp < 48) || (cp > 57))
			return false;

		return true;
	}

	void MainForm::onCloseButton(const QuickGUI::EventArgs& args)
	{
		mApp->exit();
	}

	void MainForm::onSheetResized(const QuickGUI::EventArgs& args)
	{
		float cpX = (mMeshList->getPosition().x / 2.0) - (mControlPanel->getWidth() / 2.0);
		float cpY = mSheet->getHeight() - mControlPanel->getHeight();
		mControlPanel->setPosition(QuickGUI::Point(cpX,cpY));

		float availableWidth = mMeshList->getPosition().x;
		float availableHeight = mControlPanel->getPosition().y;
		QuickGUI::Size newImageSize(availableWidth / 2.0,availableHeight / 2.0);

		mDefaultCamera->setAspectRatio(newImageSize.width / newImageSize.height);
		mSideCamera->setAspectRatio(newImageSize.width / newImageSize.height);
		mTopCamera->setAspectRatio(newImageSize.width / newImageSize.height);
		mFrontCamera->setAspectRatio(newImageSize.width / newImageSize.height);

		mTopViewImage->setSize(newImageSize);
		mSideViewImage->setSize(newImageSize);
		mFrontViewImage->setSize(newImageSize);
		mPerspectiveViewImage->setSize(newImageSize);

		mSideViewImage->setPosition(QuickGUI::Point(availableWidth / 2.0,0));
		mFrontViewImage->setPosition(QuickGUI::Point(0,availableHeight / 2.0));
		mPerspectiveViewImage->setPosition(QuickGUI::Point(availableWidth / 2.0,availableHeight / 2.0));

		QuickGUI::Point offset(5,newImageSize.height - 25);
		mTopViewLabel->setPosition(mTopViewImage->getPosition() + offset);
		mSideViewLabel->setPosition(mSideViewImage->getPosition() + offset);
		mFrontViewLabel->setPosition(mFrontViewImage->getPosition() + offset);
		mPerspectiveViewLabel->setPosition(mPerspectiveViewImage->getPosition() + offset);
	}

	void MainForm::update(float timeSinceLastFrame)
	{
		Window::update(timeSinceLastFrame);

		if((mAnimationState != NULL) && mPlaying && !mPaused)
		{
			mAnimationState->addTime((timeSinceLastFrame / 1000.0) * mAnimationSpeed);

			mTimePositionScrollBar->setPercentage(mAnimationState->getTimePosition() / mAnimationState->getLength());
		}
	}

	void MainForm::windowClosed(Ogre::RenderWindow* rw)
	{
		Window::windowClosed(rw);

		mApp->exit();
	}
}
