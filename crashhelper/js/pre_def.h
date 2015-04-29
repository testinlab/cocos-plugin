#include "cocos2d.h"

#if COCOS2D_VERSION >= 0x00030500
#define TJSObject JS::HandleObject
#else
#define TJSObject JSObject *
#endif