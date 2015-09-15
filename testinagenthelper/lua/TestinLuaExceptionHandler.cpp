#include <string.h>
#include "TestinLuaExceptionHandler.h"
#include "../TestinAgentHelper.h"
#include "CCLuaEngine.h"


void TestinLuaExceptionHandler::registerLuaExceptionHandler() {
#if COCOS2D_VERSION >= 0x00030000
	lua_State* ls = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState(); 
#else
	lua_State* ls = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState(); 
#endif
	lua_register(ls, "onLuaException", onLuaException);
    lua_register(ls, "leaveBreadcrumb", leaveBreadcrumb);
    lua_register(ls, "setTestinUserInfo", setTestinUserInfo);
}

int TestinLuaExceptionHandler::onLuaException(lua_State* ls) {
	const char* brief = lua_tostring(ls, 1); 
	const char* traceback = lua_tostring(ls, 2); 
	TestinAgentHelper::reportException(EXCEPTION_TYPE_LUA, brief, traceback);

	return 0;
}

int TestinLuaExceptionHandler::leaveBreadcrumb(lua_State* ls){
    const char* Breadcrumb = lua_tostring(ls, 1);
    TestinAgentHelper::leaveBreadcrumb(Breadcrumb);
    
    return 0;
}

int TestinLuaExceptionHandler::setTestinUserInfo(lua_State* ls) {
    const char* userinfo = lua_tostring(ls, 1);
    TestinAgentHelper::setUserInfo(userinfo);
    
    return 0;
}



