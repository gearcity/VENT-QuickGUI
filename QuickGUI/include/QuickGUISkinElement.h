#ifndef QUICKGUISKINELEMENT_H
#define QUICKGUISKINELEMENT_H

#include "QuickGUIBorderEnums.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIRect.h"
#include "QuickGUISerializable.h"
#include "QuickGUIUVRect.h"

#include "OgrePrerequisites.h"
#include "OgreImage.h"

namespace QuickGUI
{
	class _QuickGUIExport SkinElement :
		public Serializable
	{
	public:
		SkinElement(const Ogre::String& elementName);

		/**
		* Returns the UV coordinates of the Background.
		*/
		UVRect getBackgroundUVCoords();
		/**
		* Returns the thickness of a border side.
		*/
		float getBorderThickness(BorderSide s);
		/**
		* Returns the UV coordinates of the Border.
		*/
		UVRect getBorderUVCoords(BorderSide s);
		/**
		* Returns the height of the texture representing this SkinElement.
		*/
		int getHeight();
		/**
		* Returns the name of the SkinElement.
		*/
		Ogre::String getName();
		/**
		* Returns the texture of the skin element as an Ogre Image object.
		*/
		const Ogre::Image& getTextureImage();
		/**
		* Returns the Texture name representing the SkinElement.
		*/
		Ogre::String getTextureName();
		/**
		* Returns true if the background texture is tiled when drawn,
		* false if background texture is stretched when drawn.
		*/
		bool getTileBackground();
		/**
		* Returns true if the border texture is tiled when drawn,
		* false if border texture is stretched when drawn.
		*/
		bool getTileBorders();
		/**
		* Returns the width of the texture representing this SkinElement.
		*/
		int getWidth();

		/**
		* Returns true if the given coordinates are over a pixel with 0 alpha, false otherwise.
		*/
		bool transparentPixel(Point p);

		/**
		* Builds the SkinElement from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Sets the thickness of a border side.
		*/
		void setBorderThickness(BorderSide s, float thickness);
		/**
		* Sets the Texture name representing the SkinElement.
		*/
		void setTextureName(const Ogre::String& textureName);
		/**
		* Set true if the Background texture should be tiled when drawn.  Set
		* false if the Background texture should be stretched when drawn.
		*/
		void setTileBackground(bool tile);
		/**
		* Set true if the Border texture should be tiled when drawn. Set
		* false if the Border texture should be stretched when drawn.
		*/
		void setTileBorders(bool tile);

	protected:
		Ogre::String mName;

		Ogre::String mTextureName;

		bool mTileBackground;
		bool mTileBorders;

		UVRect mBackgroundUV;

		float mBorderThickness[4];
		UVRect mBorderUV[8];

		bool mDirty;
		void _updateUVsAndDimensions();
		void _updateTexture();

		Ogre::Image mTexture;
	};
}

#endif
