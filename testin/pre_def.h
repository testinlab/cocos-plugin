#include "cocos2d.h"

//#define COCOS2D_VERSION 0x00030100
#if COCOS2D_VERSION >= 0x00020100 && COCOS2D_VERSION < 0x00030000
	#define COCOS2D_CPP_2X

	#define TCLog CCLog
	#define TCNode CCNode
	#define TCTexture2D CCTexture2D
	#define TCScene	CCScene
	#define TCLayer	CCLayer
	#define TCDirector CCDirector
	#define TCSize CCSize
	#define TCPoint CCPoint
	#define TCRect CCRect
	#define TCSprite CCSprite
	#define TCMenuItem CCMenuItem
	#define TCAtlasNode CCAtlasNode
	#define TCLabelTTF CCLabelTTF
	#define TCLabelAtlas CCLabelAtlas
	#define TCMenuItemLabel CCMenuItemLabel
	#define TCMenuItemSprite CCMenuItemSprite
	#define TCMenuItemToggle CCMenuItemToggle
	#define TCSpriteBatchNode CCSpriteBatchNode
	#define TCScale9Sprite extension::CCScale9Sprite
	#define TCTextureCache CCTextureCache

	#define funcDirectorGetInstance sharedDirector
	#define funcNodeGetZOder getZOrder
	#define funcGetString getString
	#define funcGetBoundingBox boundingBox
	#define funcGetOffsetPosition() getOffsetPosition()
	#define funcPoint ccp

#elif COCOS2D_VERSION < 0x00040000
	#define COCOS2D_CPP_3X

	#define TCLog log
	#define TCNode Node
	#define TCTexture2D Texture2D
	#define TCScene	Scene
	#define TCLayer Layer
	#define TCDirector Director
	#define TCSize Size
	#define TCPoint Point
	#define TCRect Rect
	#define TCSprite Sprite
	#define TCMenuItem MenuItem
	#define TCAtlasNode AtlasNode
	#define TCLabelTTF LabelTTF
	#define TCLabelAtlas LabelAtlas
	#define TCMenuItemLabel MenuItemLabel
	#define TCMenuItemSprite MenuItemSprite
	#define TCMenuItemToggle MenuItemToggle
	#define TCSpriteBatchNode SpriteBatchNode
	#define TCScale9Sprite extension::Scale9Sprite
	#define TCTextureCache TextureCache
	#define TCLabel Label


	#define funcDirectorGetInstance getInstance
	#define funcNodeGetZOder getLocalZOrder
	#define funcGetString getString().c_str
	#define funcGetBoundingBox getBoundingBox
	#define funcGetOffsetPosition() getBoundingBox().origin
	#define funcPoint Point


	#if COCOS2D_VERSION >= 0x00030100 && COCOS2D_VERSION < 0x00030200
		#define COCOS2D_CPP_31
	#endif
	#if COCOS2D_VERSION >= 0x00030200
		#define COCOS2D_CPP_32
		#define COCOS2D_CPP_32_ABOVE
	#endif
	#if COCOS2D_VERSION < 0x00030100
		#define COCOS2D_CPP_30
	#endif

#endif

