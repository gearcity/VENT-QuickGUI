#include "QuickGUIBrush.h"
#include "QuickGUISkinElement.h"
#include "QuickGUIVertex.h"

#include <OgreMaterialManager.h>
#include <OgreGpuProgramManager.h>
#include <OgreHighLevelGpuProgramManager.h>

#include "OgreTechnique.h"
#include "OgreViewport.h"
#include "OgreRenderTarget.h"
#include "OgreRenderTexture.h"

template<> QuickGUI::Brush* Ogre::Singleton<QuickGUI::Brush>::msSingleton = 0;

namespace QuickGUI
{
        Brush::Brush() :
                mSceneManager(NULL),
                mDefaultViewport(NULL),
                mRenderTarget(NULL),
                mQueuedItems(false),
                mBufferPtr(NULL),
                mFilterMode(BRUSHFILTER_LINEAR),
                mBrushBlendMode(BRUSHBLEND_ALPHA),
                mUsingProgrammablePipeline(false)
        {
                mRenderSystem = Ogre::Root::getSingleton().getRenderSystem();

		_createVertexBuffer();

		// Initialise texture address modes to be used. We use this every frame, so we'll set them up now to save time later.
		mTextureAddressMode.u = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.v = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.w = Ogre::TextureUnitState::TAM_CLAMP;

		// Search for "GL" somewhere in name, to see if we using OpenGL
                Ogre::String rSysName = mRenderSystem->getName();
                mUsingOpenGL = (rSysName.find("GL") != Ogre::String::npos);
                mUsingProgrammablePipeline = (rSysName.find("OpenGL 3+") != Ogre::String::npos);

		// Store Texel offset for quick use
		mHorizontalTexelOffset = mRenderSystem->getHorizontalTexelOffset();
		mVerticalTexelOffset = mRenderSystem->getVerticalTexelOffset();

		// Create default texture
		mDefaultTexture = Ogre::TextureManager::getSingleton().createManual(
			"QuickGUI.Brush.DefaultTexture",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, 64, 64, 1, 0, Ogre::PF_R8G8B8A8, Ogre::TU_DEFAULT );

		// Clear default texture
                void* buffer = mDefaultTexture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
		memset(buffer, 0, mDefaultTexture->getBuffer()->getSizeInBytes());
		mDefaultTexture->getBuffer()->unlock();

		mColourValue = ColourValue::White;
/*
		mGUIMaterial = Ogre::MaterialManager::getSingleton().load("GuiMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mGUIPass = mGUIMaterial->getTechnique(0)->getPass(0);
		mGUIPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
*/
		mGUIMaterial = Ogre::MaterialManager::getSingleton().create("QuickGUI.Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mGUIPass = mGUIMaterial->getTechnique(0)->getPass(0);
		mGUIPass->setCullingMode(Ogre::CULL_NONE);
		mGUIPass->setDepthCheckEnabled(false);
		mGUIPass->setDepthWriteEnabled(false);
		mGUIPass->setLightingEnabled(false);
		mGUIPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

                Ogre::TextureUnitState* tus = mGUIPass->createTextureUnitState();
                tus->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
                tus->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
                if(!mDefaultTexture.isNull())
#if OGRE_VERSION >= ((1 << 16) | (13 << 8))
                        tus->setTexture(mDefaultTexture);
#else
                        tus->setTextureName(mDefaultTexture->getName(), mDefaultTexture->getGroup());
#endif

                if(mUsingProgrammablePipeline)
                        _initialiseGpuPipeline();
	}

	Brush::~Brush()
	{
		_destroyVertexBuffer();
	}

	Brush* Brush::getSingletonPtr(void)
	{
		return msSingleton;
	}

	Brush& Brush::getSingleton(void)
	{
		assert( msSingleton );
		return ( *msSingleton );
	}

	void Brush::_buildLineVertices(const Point& p1, const Point& p2, Vector3* verts)
	{
		Point firstPoint(p1);
		Point secondPoint(p2);

		// Certain Render Targets in OpenGL require the coordinates to be flipped.
		if(mRenderTarget->getTarget()->requiresTextureFlipping())
		{
			firstPoint.y = mTargetHeight - firstPoint.y;
			secondPoint.y = mTargetHeight - secondPoint.y;
		}

		firstPoint.x += mHorizontalTexelOffset;
		firstPoint.y += mVerticalTexelOffset;
		secondPoint.x += mHorizontalTexelOffset;
		secondPoint.y += mVerticalTexelOffset;

		firstPoint.x /= mTargetWidth;
		firstPoint.y /= mTargetHeight;
		secondPoint.x /= mTargetWidth;
		secondPoint.y /= mTargetHeight;

		float left = (firstPoint.x * 2) - 1;
		float top = -((firstPoint.y * 2) - 1);
		verts[0] = Vector3( left, top, 0.0f );

		left = (secondPoint.x * 2) - 1;
		top = -((secondPoint.y * 2) - 1);
		verts[1] = Vector3( left, top, 0.0f );
	}

	void Brush::_buildQuadVertices(const Rect& dimensions, const UVRect& uvCoords, Vector3* verts, Vector2* uv)
	{
		Rect r = dimensions;

		// Certain Render Targets in OpenGL require the coordinates to be flipped.
		if(mRenderTarget->getTarget()->requiresTextureFlipping())
		{
			r.position.y = mTargetHeight - dimensions.position.y;
			r.size.height = (mTargetHeight - (dimensions.position.y + dimensions.size.height)) - r.position.y;
		}

		r.position.x += mHorizontalTexelOffset;
		r.position.y += mVerticalTexelOffset;

		r.position.x /= mTargetWidth;
		r.position.y /= mTargetHeight;
		r.size.width /= mTargetWidth;
		r.size.height /= mTargetHeight;

		float left = (r.position.x * 2) - 1;
		float right = left + (r.size.width * 2);
		float top = -((r.position.y * 2) - 1);
		float bottom = top - (r.size.height * 2);

		// Populate Vertex information
		verts[0] = Vector3( left, bottom, 0.0f );
		verts[1] = Vector3( right, bottom, 0.0f );
		verts[2] = Vector3( left, top, 0.0f );
		verts[3] = Vector3( right, bottom, 0.0f );
		verts[4] = Vector3( right, top, 0.0f );
		verts[5] = Vector3( left, top, 0.0f );

		// Pupulate UV information
		uv[0] = Vector2( uvCoords.left, uvCoords.bottom );
		uv[1] = Vector2( uvCoords.right, uvCoords.bottom );
		uv[2] = Vector2( uvCoords.left, uvCoords.top );
		uv[3] = Vector2( uvCoords.right, uvCoords.bottom );
		uv[4] = Vector2( uvCoords.right, uvCoords.top );
		uv[5] = Vector2( uvCoords.left, uvCoords.top );
	}

	void Brush::_createVertexBuffer()
	{
                mRenderOperation.vertexData = OGRE_NEW Ogre::VertexData();
		mRenderOperation.vertexData->vertexStart = 0;

		_declareVertexStructure();

		// Create the Vertex Buffer, using the Vertex Structure we previously declared in _declareVertexStructure.
                mVertexBuffer = Ogre::HardwareBufferManager::getSingleton( ).createVertexBuffer(
			mRenderOperation.vertexData->vertexDeclaration->getVertexSize(0), // declared Vertex used
			VERTEX_COUNT,
                        Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
			false );

		// Bind the created buffer to the renderOperation object.  Now we can manipulate the buffer, and the RenderOp keeps the changes.
		mRenderOperation.vertexData->vertexBufferBinding->setBinding( 0, mVertexBuffer );
		mRenderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOperation.useIndexes = false;
	}

	void Brush::_declareVertexStructure()
	{
                Ogre::VertexDeclaration* vd = mRenderOperation.vertexData->vertexDeclaration;

		// Add position - Ogre::Vector3 : 4 bytes per float * 3 floats = 12 bytes

		vd->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );

		// Add color - Ogre::RGBA : 8 bits per channel (1 byte) * 4 channels = 4 bytes

                vd->addElement( 0, Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ), Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE );

		// Add texture coordinates - Ogre::Vector2 : 4 bytes per float * 2 floats = 8 bytes

                vd->addElement( 0, Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ) +
                                               Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT4 ),
						   Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

		/* Our structure representing the Vertices used in the buffer (24 bytes):
			struct Vertex
			{
				Vector3 pos;
				ColoureValue color;
				Vector2 uv;
			};
		*/
	}

	void Brush::_destroyVertexBuffer()
	{
		OGRE_DELETE mRenderOperation.vertexData;
		mRenderOperation.vertexData = NULL;
		mVertexBuffer.setNull();
	}

	void Brush::beginLineQueue()
	{
		if(mQueuedItems)
			throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"beginRectQueue or beginLineQueue has already been called, cannot Start a Queue within a Queue!","Brush::beginRectQueue");

		// if we don't have a pointer into the buffer, lock it
		if(mBufferPtr == NULL)
		{
			mRenderOperation.vertexData->vertexStart = 0;
			mRenderOperation.vertexData->vertexCount = 0;
			mBufferPtr = (Vertex*)mVertexBuffer->lock( Ogre::HardwareBuffer::HBL_DISCARD );
                }

		mRenderOperation.operationType = Ogre::RenderOperation::OT_LINE_LIST;

		mQueuedItems = true;
	}

	void Brush::beginRectQueue()
	{
		if(mQueuedItems)
			throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"beginRectQueue or beginLineQueue has already been called, cannot Start a Queue within a Queue!","Brush::beginRectQueue");

		// if we don't have a pointer into the buffer, lock it
		if(mBufferPtr == NULL)
		{
			mRenderOperation.vertexData->vertexStart = 0;
			mRenderOperation.vertexData->vertexCount = 0;
			mBufferPtr = (Vertex*)mVertexBuffer->lock( Ogre::HardwareBuffer::HBL_DISCARD );
		}

		mQueuedItems = true;
	}

	void Brush::clear()
	{
		//mRenderSystem->clearFrameBuffer(Ogre::FBT_COLOUR, Ogre::ColourValue(mColourValue.r,mColourValue.g,mColourValue.b,mColourValue.a));
		mRenderSystem->clearFrameBuffer(Ogre::FBT_COLOUR,Ogre::ColourValue(0,0,0,0));
	}

	void Brush::drawLine(const Point& p1, const Point& p2)
	{
		drawLine(p1,p2,mColourValue);
	}

	void Brush::drawLine(const Point& p1, const Point& p2, const ColourValue& cv)
	{
		if(mQueuedItems)
			throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"Canot draw Line, LineQueue or RectQueue has been started!","Brush::drawLine");

		// Build Vertex Data

		Vector3 verts[2];
		_buildLineVertices(p1,p2,verts);

		// Lock buffer

		Vertex* data = (Vertex*)mVertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);

		// Write vertices to vertex buffer

		for ( size_t x = 0; x < 2; x++ )
		{
			data[x].pos = verts[x];
			data[x].color = cv;
			data[x].uv = Vector2::ZERO;
		}

		// Unlock buffer

		mVertexBuffer->unlock();

		// Perform Rendering operation

		mRenderOperation.operationType = Ogre::RenderOperation::OT_LINE_LIST;
		mRenderOperation.vertexData->vertexStart = 0;
		mRenderOperation.vertexData->vertexCount = 2;
		mRenderSystem->_render(mRenderOperation);

		// Restore render operation to default used type
		mRenderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	}

	void Brush::drawLineRectangle(const Rect& r)
	{
	}

	void Brush::drawRectangle(Rect r, UVRect ur)
	{
		if(mQueuedItems)
			throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"Canot draw Rectangle, LineQueue or RectQueue has been started!","Brush::drawRectangle");

		// If width or height is less than a pixel in dimension, don't draw anything
		if(r.size.width < 1.0 || r.size.height < 1.0)
			return;

		// Build Vertex Data

		Vector3 verts[6];
		Vector2 uv[6];
		_buildQuadVertices(r,ur,verts,uv);

		// Lock buffer

		Vertex* data = (Vertex*)mVertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);

		// Write vertices to vertex buffer

		for ( size_t x = 0; x < 6; x++ )
		{
			data[x].pos = verts[x];
			data[x].color = mColourValue;
			data[x].uv = uv[x];
		}

		// Unlock buffer

		mVertexBuffer->unlock();

		// Perform Rendering operation

		mRenderOperation.vertexData->vertexStart = 0;
		mRenderOperation.vertexData->vertexCount = 6;
		mRenderSystem->_render(mRenderOperation);
	}

	void Brush::drawSkinElement(Rect r, SkinElement* e)
	{
		// SkinElements can be empty, and in this case we draw nothing.
		if((e == NULL) || (e->getTextureName() == ""))
			return;

		Ogre::String previousTextureName = mTexture->getName();

		setTexture(e->getTextureName());

		Rect temp;
		// Draw border parts
		if(e->getTileBorders())
		{
			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_LEFT);
			temp.size.height = e->getBorderThickness(BORDER_TOP);
			drawTiledRectangle(temp,e->getBorderUVCoords(BORDER_TOP_LEFT));

			temp = r;
			temp.size.width -= (e->getBorderThickness(BORDER_LEFT) + e->getBorderThickness(BORDER_RIGHT));
			temp.size.height = e->getBorderThickness(BORDER_TOP);
			temp.position.x += e->getBorderThickness(BORDER_LEFT);
			drawTiledRectangle(temp,e->getBorderUVCoords(BORDER_TOP));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_RIGHT);
			temp.size.height = e->getBorderThickness(BORDER_TOP);
			temp.position.x += r.size.width - e->getBorderThickness(BORDER_RIGHT);
			drawTiledRectangle(temp,e->getBorderUVCoords(BORDER_TOP_RIGHT));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_RIGHT);
			temp.size.height -= (e->getBorderThickness(BORDER_TOP) + e->getBorderThickness(BORDER_BOTTOM));
			temp.position.x += r.size.width - e->getBorderThickness(BORDER_RIGHT);
			temp.position.y += e->getBorderThickness(BORDER_TOP);
			drawTiledRectangle(temp,e->getBorderUVCoords(BORDER_RIGHT));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_RIGHT);
			temp.size.height = e->getBorderThickness(BORDER_BOTTOM);
			temp.position.x += r.size.width - e->getBorderThickness(BORDER_RIGHT);
			temp.position.y += r.size.height - e->getBorderThickness(BORDER_BOTTOM);
			drawTiledRectangle(temp,e->getBorderUVCoords(BORDER_BOTTOM_RIGHT));

			temp = r;
			temp.size.width -= (e->getBorderThickness(BORDER_LEFT) + e->getBorderThickness(BORDER_RIGHT));
			temp.size.height = e->getBorderThickness(BORDER_BOTTOM);
			temp.position.x += e->getBorderThickness(BORDER_LEFT);
			temp.position.y += r.size.height - e->getBorderThickness(BORDER_BOTTOM);
			drawTiledRectangle(temp,e->getBorderUVCoords(BORDER_BOTTOM));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_LEFT);
			temp.size.height = e->getBorderThickness(BORDER_BOTTOM);
			temp.position.y += r.size.height - e->getBorderThickness(BORDER_BOTTOM);
			drawTiledRectangle(temp,e->getBorderUVCoords(BORDER_BOTTOM_LEFT));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_LEFT);
			temp.size.height -= (e->getBorderThickness(BORDER_TOP) + e->getBorderThickness(BORDER_BOTTOM));
			temp.position.y += e->getBorderThickness(BORDER_TOP);
			drawTiledRectangle(temp,e->getBorderUVCoords(BORDER_LEFT));
		}
		else
		{
			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_LEFT);
			temp.size.height = e->getBorderThickness(BORDER_TOP);
			drawRectangle(temp,e->getBorderUVCoords(BORDER_TOP_LEFT));

			temp = r;
			temp.size.width -= (e->getBorderThickness(BORDER_LEFT) + e->getBorderThickness(BORDER_RIGHT));
			temp.size.height = e->getBorderThickness(BORDER_TOP);
			temp.position.x += e->getBorderThickness(BORDER_LEFT);
			drawRectangle(temp,e->getBorderUVCoords(BORDER_TOP));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_RIGHT);
			temp.size.height = e->getBorderThickness(BORDER_TOP);
			temp.position.x += r.size.width - e->getBorderThickness(BORDER_RIGHT);
			drawRectangle(temp,e->getBorderUVCoords(BORDER_TOP_RIGHT));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_RIGHT);
			temp.size.height -= (e->getBorderThickness(BORDER_TOP) + e->getBorderThickness(BORDER_BOTTOM));
			temp.position.x += r.size.width - e->getBorderThickness(BORDER_RIGHT);
			temp.position.y += e->getBorderThickness(BORDER_TOP);
			drawRectangle(temp,e->getBorderUVCoords(BORDER_RIGHT));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_RIGHT);
			temp.size.height = e->getBorderThickness(BORDER_BOTTOM);
			temp.position.x += r.size.width - e->getBorderThickness(BORDER_RIGHT);
			temp.position.y += r.size.height - e->getBorderThickness(BORDER_BOTTOM);
			drawRectangle(temp,e->getBorderUVCoords(BORDER_BOTTOM_RIGHT));

			temp = r;
			temp.size.width -= (e->getBorderThickness(BORDER_LEFT) + e->getBorderThickness(BORDER_RIGHT));
			temp.size.height = e->getBorderThickness(BORDER_BOTTOM);
			temp.position.x += e->getBorderThickness(BORDER_LEFT);
			temp.position.y += r.size.height - e->getBorderThickness(BORDER_BOTTOM);
			drawRectangle(temp,e->getBorderUVCoords(BORDER_BOTTOM));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_LEFT);
			temp.size.height = e->getBorderThickness(BORDER_BOTTOM);
			temp.position.y += r.size.height - e->getBorderThickness(BORDER_BOTTOM);
			drawRectangle(temp,e->getBorderUVCoords(BORDER_BOTTOM_LEFT));

			temp = r;
			temp.size.width = e->getBorderThickness(BORDER_LEFT);
			temp.size.height -= (e->getBorderThickness(BORDER_TOP) + e->getBorderThickness(BORDER_BOTTOM));
			temp.position.y += e->getBorderThickness(BORDER_TOP);
			drawRectangle(temp,e->getBorderUVCoords(BORDER_LEFT));
		}

		// Draw Background Texture
		if(e->getTileBackground())
		{
			temp = r;
			temp.translate(Point(e->getBorderThickness(BORDER_LEFT),e->getBorderThickness(BORDER_TOP)));
			temp.size.width -= (e->getBorderThickness(BORDER_LEFT) + e->getBorderThickness(BORDER_RIGHT));
			temp.size.height -= (e->getBorderThickness(BORDER_TOP) + e->getBorderThickness(BORDER_BOTTOM));
			drawTiledRectangle(temp,e->getBackgroundUVCoords());
		}
		else
		{
			temp = r;
			temp.translate(Point(e->getBorderThickness(BORDER_LEFT),e->getBorderThickness(BORDER_TOP)));
			temp.size.width -= (e->getBorderThickness(BORDER_LEFT) + e->getBorderThickness(BORDER_RIGHT));
			temp.size.height -= (e->getBorderThickness(BORDER_TOP) + e->getBorderThickness(BORDER_BOTTOM));
			drawRectangle(temp,e->getBackgroundUVCoords());
		}

		setTexture(previousTextureName);
	}

	void Brush::drawTiledRectangle(Rect r, UVRect ur)
	{
		// If width or height is less than a pixel in dimension, don't draw anything
		if(r.size.width < 1.0 || r.size.height < 1.0)
			return;

		float texWidth = mTexture->getWidth();
		float texHeight = mTexture->getHeight();

		float pixelWidth = (ur.right - ur.left) * texWidth;
		float pixelHeight = (ur.bottom - ur.top) * texHeight;

		float left, right, top, bottom;
		left = r.position.x;
		right = r.position.x + r.size.width;
		top = r.position.y;
		bottom = r.position.y + r.size.height;

		float x = left;
		float y = top;
		Rect tempRect;
		UVRect tempUVRect;
		while( y < (r.position.y + r.size.height) )
		{
			tempUVRect = ur;
			tempRect.position.x = x;
			tempRect.position.y = y;

			if( (x + pixelWidth) > right )
			{
				tempRect.size.width = (right - x);
				tempUVRect.right = tempUVRect.left + ((right - x) / texWidth);
			}
			else
				tempRect.size.width = pixelWidth;

			if( (y + pixelHeight) > bottom )
			{
				tempRect.size.height = (bottom - y);
				tempUVRect.bottom = tempUVRect.top + ((bottom - y) / texHeight);
			}
			else
				tempRect.size.height = pixelHeight;

			tempRect.size.round();
			drawRectangle(tempRect,tempUVRect);

			x += pixelWidth;
			if( x >= right )
			{
				x = left;
				y += pixelHeight;
			}
		}
	}

	void Brush::endLineQueue()
	{
		if(mQueuedItems)
		{
			mBufferPtr = NULL;

			mVertexBuffer->unlock();

			// Perform Rendering operation

			mRenderSystem->_render(mRenderOperation);

			mQueuedItems = false;
		}

		// Restore render operation to default used type
		mRenderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	}

	void Brush::endRectQueue()
	{
		if(mQueuedItems)
		{
			mBufferPtr = NULL;

			mVertexBuffer->unlock();

			// Perform Rendering operation

			mRenderSystem->_render(mRenderOperation);

			mQueuedItems = false;
		}
	}

	BrushBlendMode Brush::getBlendMode()
	{
		return mBrushBlendMode;
	}

	Rect Brush::getClipRegion()
	{
		return mClipRegion;
	}

	ColourValue Brush::getColour()
	{
		return mColourValue;
	}

	BrushFilterMode Brush::getFilterMode()
	{
		return mFilterMode;
	}

	float Brush::getOpacity()
	{
		return mColourValue.a;
	}

	Ogre::Viewport* Brush::getRenderTarget()
	{
		return mRenderTarget;
	}

	Ogre::TexturePtr Brush::getTexture()
	{
		return mTexture;
	}

       void Brush::prepareToDraw()
       {
               if(mUsingProgrammablePipeline)
                       _applyPassState();
               else
                       _applyFixedFunctionDefaults();

               // Set default settings

               setTexture("");
               setColor(ColourValue::White);
               setBlendMode(BRUSHBLEND_ALPHA);
               setFilterMode(BRUSHFILTER_NEAREST);
       }

	void Brush::queueLine(Point p1, Point p2)
	{
		queueLine(p1,p2,mColourValue);
	}

	void Brush::queueLine(Point p1, Point p2, const ColourValue& cv)
	{
		if(!mQueuedItems)
			throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"beginLineQueue has not been called, cannot Queue Lines!","Brush::queueLine");

		if(mRenderOperation.operationType != Ogre::RenderOperation::OT_LINE_STRIP)
			throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"Cannot Queue Rects when RectQueue has started!","Brush::queueLine");

		// If the vertex buffer is full, draw it (empty buffer).
		if((mRenderOperation.vertexData->vertexCount + 2) > VERTEX_COUNT)
		{
			endLineQueue();
			beginLineQueue();
		}

		Vector3 verts[2];
		_buildLineVertices(p1,p2,verts);

		// Write vertices to vertex buffer

		for ( size_t x = 0; x < 2; x++ )
		{
			mBufferPtr[x].pos = verts[x];
			mBufferPtr[x].color = mColourValue;
			mBufferPtr[x].uv = Vector2::ZERO;
		}

		mBufferPtr += 2;

		mRenderOperation.vertexData->vertexCount += 2;
	}

	void Brush::queueRect(Rect r, UVRect ur)
	{
		if(!mQueuedItems)
			throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"beginRectQueue has not been called, cannot Queue Rects!","Brush::queueRect");

		if(mRenderOperation.operationType != Ogre::RenderOperation::OT_TRIANGLE_LIST)
			throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"Cannot Queue Rects when LineQueue has started!","Brush::queueRect");

		// If the vertex buffer is full, draw it (empty buffer).
		if((mRenderOperation.vertexData->vertexCount + 6) > VERTEX_COUNT)
		{
			endRectQueue();
			beginRectQueue();
		}

		Vector3 verts[6];
		Vector2 uv[6];
		_buildQuadVertices(r,ur,verts,uv);

		// Write vertices to vertex buffer

		for ( size_t x = 0; x < 6; x++ )
		{
			mBufferPtr[x].pos = verts[x];
			mBufferPtr[x].color = mColourValue;
			mBufferPtr[x].uv = uv[x];
		}

		mBufferPtr += 6;

		mRenderOperation.vertexData->vertexCount += 6;
	}

	void Brush::restore()
	{
	}

        void Brush::setBlendMode(BrushBlendMode m)
        {
                mBrushBlendMode = m;

                if(mUsingProgrammablePipeline)
                {
                        switch(mBrushBlendMode)
                        {
                        case BRUSHBLEND_NONE:
                                mGUIPass->setSceneBlending( Ogre::SBF_ONE, Ogre::SBF_ZERO );
                                break;

                        case BRUSHBLEND_MODULATE:
                                mGUIPass->setSeparateSceneBlending( Ogre::SBF_ZERO, Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE, Ogre::SBF_ONE );
                                break;

                        case BRUSHBLEND_DISCARDALPHA:
                                mGUIPass->setSeparateSceneBlending( Ogre::SBF_ONE, Ogre::SBF_ZERO, Ogre::SBF_ZERO, Ogre::SBF_ONE );
                                break;

                        case BRUSHBLEND_INVERT:
                                mGUIPass->setSeparateSceneBlending( Ogre::SBF_ONE_MINUS_DEST_COLOUR, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ONE );
                                break;

                        case BRUSHBLEND_ALPHA:
                                mGUIPass->setSeparateSceneBlending( Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ONE );
                                break;
                        }

                        _applyPassState();
                        return;
                }

                switch(mBrushBlendMode)
                {
                case BRUSHBLEND_NONE:
                        mRenderSystem->_setSceneBlending( Ogre::SBF_ONE, Ogre::SBF_ZERO );
                        break;

		case BRUSHBLEND_MODULATE:
			mRenderSystem->_setSeparateSceneBlending( Ogre::SBF_ZERO, Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE, Ogre::SBF_ONE );
			break;

		case BRUSHBLEND_DISCARDALPHA:
			mRenderSystem->_setSeparateSceneBlending( Ogre::SBF_ONE, Ogre::SBF_ZERO, Ogre::SBF_ZERO, Ogre::SBF_ONE );
			break;

		case BRUSHBLEND_INVERT:
			mRenderSystem->_setSeparateSceneBlending( Ogre::SBF_ONE_MINUS_DEST_COLOUR, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ONE );
			break;

		case BRUSHBLEND_ALPHA:
			mRenderSystem->_setSeparateSceneBlending( Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ONE );
			break;
		}
	}

	void Brush::setColor(const ColourValue& cv)
	{
		mColourValue = cv;
	}

	void Brush::setClipRegion(const Rect& r)
	{
		mRenderSystem->setScissorTest(true, r.position.x, r.position.y, (r.position.x + r.size.width), (r.position.y + r.size.height));
		mClipRegion = r;
	}

        void Brush::setFilterMode(BrushFilterMode m)
        {
                mFilterMode = m;

                if(mUsingProgrammablePipeline)
                {
                        Ogre::TextureUnitState* tus = mGUIPass->getTextureUnitState(0);

                        if(tus != NULL)
                        {
                                switch (mFilterMode)
                                {
                                case BRUSHFILTER_NONE:
                                        tus->setTextureFiltering(Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE );
                                        break;
                                case BRUSHFILTER_NEAREST:
                                        tus->setTextureFiltering( Ogre::FO_POINT, Ogre::FO_POINT, Ogre::FO_POINT );
                                        break;
                                case BRUSHFILTER_LINEAR:
                                        tus->setTextureFiltering( Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT );
                                        break;
                                }
                        }

                        _applyPassState();
                        return;
                }

               Ogre::TextureManager* tm = Ogre::TextureManager::getSingletonPtr();


                switch (mFilterMode)
                {
                case BRUSHFILTER_NONE:
                        tm->getDefaultSampler()->setFiltering(Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE );
			break;
		case BRUSHFILTER_NEAREST:
                        tm->getDefaultSampler()->setFiltering( Ogre::FO_POINT, Ogre::FO_POINT, Ogre::FO_POINT );
			break;
		case BRUSHFILTER_LINEAR:
                        tm->getDefaultSampler()->setFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT );
			break;
		}

                mRenderSystem->_setSampler( 0, *tm->getDefaultSampler());
	}

	void Brush::setOpacity(float opacity)
	{
		if(opacity > 1)
			opacity = 1;
		else if(opacity < 0)
			opacity = 0;

		mColourValue.a = opacity;
	}

        void Brush::setRenderTarget(Ogre::TexturePtr p)
        {
                if((p->getUsage() & Ogre::TU_RENDERTARGET) == 0)
                        throw Exception(Exception::ERR_INVALID_RENDER_TARGET,"Texture is not a valid Render Target. Make sure the Texture is created with TU_RENDERTARGET.", "Brush::setRenderTarget");

                setRenderTarget(p->getBuffer()->getRenderTarget()->getViewport(0));
        }

	void Brush::setRenderTarget(Ogre::Viewport* vp)
	{
		if(vp == NULL)
			mRenderTarget = mDefaultViewport;
		else
			mRenderTarget = vp;

		// Set new viewport on render system and reset clip region
		mTargetWidth = mRenderTarget->getActualWidth();
		mTargetHeight = mRenderTarget->getActualHeight();

               mRenderSystem->_setViewport(mRenderTarget);
               if(!mUsingProgrammablePipeline)
                       mRenderSystem->_setCullingMode(Ogre::CULL_NONE);

               setClipRegion(Rect(mRenderTarget->getActualLeft(), mRenderTarget->getActualTop(), mRenderTarget->getActualWidth(), mRenderTarget->getActualHeight()));

               if(mUsingProgrammablePipeline)
                       _applyPassState();
        }

        void Brush::setTexture(const Ogre::String& textureName)
        {
                if(mQueuedItems)
                        throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"Cannot Change Texture with Items Queued for Drawing! Call endRectQueue to draw all queued items before hand.","Brush::setTexture");

		if(textureName == "")
		{
			mTexture = mDefaultTexture;
		}
		else
		{
			Ogre::TextureManager* tm = Ogre::TextureManager::getSingletonPtr();

			if(!tm->resourceExists(textureName))
				Ogre::TextureManager::getSingleton().load(textureName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			mTexture = tm->getByName(textureName);
		}

                if(mUsingProgrammablePipeline)
                {
                        Ogre::TextureUnitState* tus = mGUIPass->getTextureUnitState(0);
                        if((tus != NULL) && !mTexture.isNull())
                        {
#if OGRE_VERSION >= ((1 << 16) | (13 << 8))
                                tus->setTexture(mTexture);
#else
                                tus->setTextureName(mTexture->getName(), mTexture->getGroup());
#endif
                        }

                        _applyPassState();
                }
                else
                {
                        mRenderSystem->_setTexture(0, true, mTexture);
                }
        }

        void Brush::setTexture(Ogre::TexturePtr p)
        {
                if(mQueuedItems)
                        throw Exception(Exception::ERR_RENDERINGAPI_ERROR,"Cannot Change Texture with Items Queued for Drawing! Call endRectQueue to draw all queued items before hand.","Brush::setTexture");

		if(p.isNull())
			mTexture = mDefaultTexture;
		else
			mTexture = p;

                if(mUsingProgrammablePipeline)
                {
                        Ogre::TextureUnitState* tus = mGUIPass->getTextureUnitState(0);
                        if((tus != NULL) && !mTexture.isNull())
                        {
#if OGRE_VERSION >= ((1 << 16) | (13 << 8))
                                tus->setTexture(mTexture);
#else
                                tus->setTextureName(mTexture->getName(), mTexture->getGroup());
#endif
                        }

                        _applyPassState();
                }
                else
                {
                        mRenderSystem->_setTexture(0, true, mTexture);
                }
        }

	void Brush::updateSceneManager(Ogre::SceneManager* sceneManager)
	{
		mSceneManager = sceneManager;
	}

        void Brush::updateViewport(Ogre::Viewport* viewport)
        {
                if(viewport == NULL)
                        throw Exception(Exception::ERR_INVALIDPARAMS,"Brush class must have a valid default Viewport! (Cannot be NULL)","Brush::updateViewport");

                mDefaultViewport = viewport;
                mRenderTarget = viewport;
        }

       void Brush::_applyFixedFunctionDefaults()
       {
               // Legacy render systems rely on the fixed-function transform and state setup.
               mRenderSystem->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
               mRenderSystem->_setProjectionMatrix( Ogre::Matrix4::IDENTITY );
               mRenderSystem->_setViewMatrix( Ogre::Matrix4::IDENTITY );

               mRenderSystem->setLightingEnabled(false);
               mRenderSystem->_setDepthBufferParams(false, false);
               mRenderSystem->_setDepthBias(0, 0);
               mRenderSystem->_setCullingMode(Ogre::CULL_NONE);
               mRenderSystem->_setFog(Ogre::FOG_NONE);
               mRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
               mRenderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
               mRenderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
               mRenderSystem->setShadingType(Ogre::SO_GOURAUD);
               mRenderSystem->_setPolygonMode(Ogre::PM_SOLID);

               mRenderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
               mRenderSystem->_setTextureCoordSet(0, 0);
#if OGRE_VERSION >= ((1 << 16) | (13 << 8))
               if(Ogre::TextureManager* tm = Ogre::TextureManager::getSingletonPtr())
               {
                       Ogre::SamplerPtr sampler = tm->getDefaultSampler();
                       sampler->setAddressingMode(mTextureAddressMode);
                       mRenderSystem->_setSampler(0, *sampler);
               }
#else
               mRenderSystem->_setTextureAddressingMode(0, mTextureAddressMode);
#endif
               mRenderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
               mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0, false);
               mRenderSystem->_disableTextureUnitsFrom(1);
       }

        void Brush::_initialiseGpuPipeline()
        {
                Ogre::HighLevelGpuProgramManager& programManager = Ogre::HighLevelGpuProgramManager::getSingleton();

                const Ogre::String vertexProgramName = "QuickGUI/GL3PlusVertex";
                if(!programManager.resourceExists(vertexProgramName))
                {
                        Ogre::HighLevelGpuProgramPtr vertexProgram = programManager.createProgram(
                                vertexProgramName,
                                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                "glsl",
                                Ogre::GPT_VERTEX_PROGRAM);

                        const Ogre::String vertexSource =
                                "#version 330 core\n"
                                "in vec4 vertex;\n"
                                "in vec4 colour;\n"
                                "in vec2 uv0;\n"
                                "out vec4 vColour;\n"
                                "out vec2 vTexCoord;\n"
                                "void main()\n"
                                "{\n"
                                "    gl_Position = vec4(vertex.xy, 0.0, 1.0);\n"
                                "    vColour = colour;\n"
                                "    vTexCoord = uv0;\n"
                                "}\n";

                        vertexProgram->setSource(vertexSource);
                        vertexProgram->setParameter("profiles", "glsl330");
                        vertexProgram->setParameter("entry_point", "main");
                        vertexProgram->load();
                }

                mGuiVertexProgram = programManager.getByName(vertexProgramName);

                const Ogre::String fragmentProgramName = "QuickGUI/GL3PlusFragment";
                if(!programManager.resourceExists(fragmentProgramName))
                {
                        Ogre::HighLevelGpuProgramPtr fragmentProgram = programManager.createProgram(
                                fragmentProgramName,
                                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                "glsl",
                                Ogre::GPT_FRAGMENT_PROGRAM);

                        const Ogre::String fragmentSource =
                                "#version 330 core\n"
                                "in vec4 vColour;\n"
                                "in vec2 vTexCoord;\n"
                                "uniform sampler2D guiSampler;\n"
                                "out vec4 fragColor;\n"
                                "void main()\n"
                                "{\n"
                                "    vec4 texColour = texture(guiSampler, vTexCoord);\n"
                                "    fragColor = texColour * vColour;\n"
                                "}\n";

                        fragmentProgram->setSource(fragmentSource);
                        fragmentProgram->setParameter("profiles", "glsl330");
                        fragmentProgram->setParameter("entry_point", "main");
                        fragmentProgram->load();
                }

                mGuiFragmentProgram = programManager.getByName(fragmentProgramName);

                mGUIPass->setVertexProgram(vertexProgramName);
                mGUIPass->setFragmentProgram(fragmentProgramName);

                Ogre::GpuProgramParametersSharedPtr fragmentParameters = mGUIPass->getFragmentProgramParameters();
                if(!fragmentParameters.isNull())
                        fragmentParameters->setNamedConstant("guiSampler", 0);
        }

        void Brush::_applyPassState()
        {
                if(mUsingProgrammablePipeline && (mSceneManager != NULL))
                        mSceneManager->_setPass(mGUIPass,false);
        }
}
