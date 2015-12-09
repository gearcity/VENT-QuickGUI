#ifndef MAINFORM_H
#define MAINFORM_H

#include "Window.h"

#include "QuickGUI.h"

#include "Ogre.h"
#include "OgreFontManager.h"

#include <map>

using namespace QuickGUI;

namespace AnimationViewer
{
	class MainForm :
		public Window
	{
	public:
		MainForm(AnimationViewerApp* app, Ogre::RenderWindow* w);
		virtual ~MainForm();

		virtual void windowClosed(Ogre::RenderWindow* rw);

		virtual void update(float timeSinceLastFrame);

	protected:

		/////// SCENE ///////

		void _setupScene();

		// ENTITY

		void _viewMesh(const std::string& meshName);

		Ogre::SceneNode* mBase;
		Ogre::Entity* mEntity;
		Ogre::AnimationState* mAnimationState;

		// CAMERAS

		void _createRenderToTexture(Ogre::Camera* cam, const std::string& RTTName, unsigned int width, unsigned int height, unsigned int visibilityFilter = -1);
		void _positionCameras();

		float mDistanceToMesh;
		Ogre::Camera* mDefaultCamera;
		Ogre::Camera* mSideCamera;
		Ogre::Camera* mTopCamera;
		Ogre::Camera* mFrontCamera;

		// Node that controls the orientation of the default camera
		Ogre::SceneNode* mDefaltCamOriNode;
		// Node the distance of the default camera from the entity, inheritting orientation from mDefaultCamOriNode
		Ogre::SceneNode* mDefaultCamDistNode;

		std::string mDefaultCameraRTTName;
		std::string mSideCameraRTTName;
		std::string mTopCameraRTTName;
		std::string mFrontCameraRTTName;

		// GRIDS
		
		/**
		* Creates grids of appropriate length, dependent on mesh radius.
		*/
		void _defineGrids();
		void _defineXZGrid(Ogre::ManualObject* mo, float left, float right, float back, float front);
		void _defineXYGrid(Ogre::ManualObject* mo, float left, float right, float top, float bottom);
		void _defineYZGrid(Ogre::ManualObject* mo, float top, float bottom, float back, float front);

		std::string mGridMaterial;
		Ogre::ManualObject* mXZGrid;
		Ogre::ManualObject* mXYGrid;
		Ogre::ManualObject* mYZGrid;

		///////	UI ///////

		void _defineGUI();

		QuickGUI::Sheet* mSheet;
		QuickGUI::Panel* mControlPanel;
		QuickGUI::List* mMeshList;
		QuickGUI::List* mAnimationList;

		QuickGUI::Label* mTopViewLabel;
		QuickGUI::Label* mSideViewLabel;
		QuickGUI::Label* mFrontViewLabel;
		QuickGUI::Label* mPerspectiveViewLabel;

		QuickGUI::Image* mTopViewImage;
		QuickGUI::Image* mSideViewImage;
		QuickGUI::Image* mFrontViewImage;
		QuickGUI::Image* mPerspectiveViewImage;

		bool mPlaying;
		bool mPaused;
		float mAnimationSpeed;

		QuickGUI::Button* mPlayButton;
		QuickGUI::Button* mPauseButton;
		QuickGUI::Button* mResumeButton;
		QuickGUI::Button* mStopButton;
		QuickGUI::CheckBox* mLoopCheckBox;
		QuickGUI::TextBox* mTimePositionTextBox;
		QuickGUI::HScrollBar* mTimePositionScrollBar;
		QuickGUI::Label* mMaxTimePositionLabel;
		QuickGUI::HScrollBar* mAnimationSpeedScrollBar;

		bool timeInputValidator(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText);

		void enableAnimationControls(bool enabled);

		void onCloseButton(const QuickGUI::EventArgs& args);
		void onSheetResized(const QuickGUI::EventArgs& args);

		void onTextBoxEnter(const QuickGUI::EventArgs& args);
		void onPlay(const QuickGUI::EventArgs& args);
		void onPause(const QuickGUI::EventArgs& args);
		void onResume(const QuickGUI::EventArgs& args);
		void onStop(const QuickGUI::EventArgs& args);
		void onLoopCheckChanged(const QuickGUI::EventArgs& args);
		void onAnimationPositionScrolled(const QuickGUI::EventArgs& args);
		void onAnimationSpeedScrolled(const QuickGUI::EventArgs& args);
		void onAnimationSliderDragged(const QuickGUI::EventArgs& args);

		void onSelectedChanged(const QuickGUI::EventArgs& args);
		void onAnimationSelectionChanged(const QuickGUI::EventArgs& args);

		// WINDOW event

		void onExitPressed(const QuickGUI::EventArgs& args);

	private:
	};
}

#endif
