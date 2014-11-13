#include <string.h>
#include "TestinLuaExceptionHandler.h"
#include "../TestinCrashHelper.h"
#include "CCLuaEngine.h"




void TestinLuaExceptionHandler::registerLuaExceptionHandler() {
#if COCOS2D_VERSION >= 0x00030000
	lua_State* ls = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState(); 
#else
	lua_State* ls = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState(); 
#endif
	lua_register(ls, "onLuaException", onLuaException); 
}

int TestinLuaExceptionHandler::onLuaException(lua_State* ls) {
	const char* brief = lua_tostring(ls, 1); 
	const char* traceback = lua_tostring(ls, 2); 
	TestinCrashHelper::reportException(EXCEPTION_TYPE_LUA, brief, traceback);

	return 0;
}








