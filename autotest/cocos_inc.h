#include "pre_def.h"

#define TESTIN_USING_CC_EXTENSION 1

#ifdef COCOS2D_CPP_2X
	#include "layers_scenes_transitions_nodes/CCScene.h"
	#include "support/tinyxml2/tinyxml2.h"
	#include "cocoa/CCArray.h"
	#include "cocoa/CCDictionary.h"
#elif defined COCOS2D_CPP_3X
	#include "external/tinyxml2/tinyxml2.h"
#endif

#if TESTIN_USING_CC_EXTENSION == 1
	#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#endif



