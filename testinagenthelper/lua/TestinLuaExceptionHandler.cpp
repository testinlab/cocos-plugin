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
    lua_register(ls, "beginTransaction", beginTransaction);
    lua_register(ls, "endTransaction", endTransaction);
    lua_register(ls, "failTransaction", failTransaction);
    lua_register(ls, "cancelTransaction", cancelTransaction);
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

int TestinLuaExceptionHandler::beginTransaction(lua_State* ls){
    const char* bTransaction = lua_tostring(ls, 1);
    TestinAgentHelper::beginTransaction(bTransaction);
    
    return 0;
}

int TestinLuaExceptionHandler::endTransaction(lua_State* ls){
    const char* eTransaction = lua_tostring(ls, 1);
    TestinAgentHelper::endTransaction(eTransaction);
    
    return 0;
}

int TestinLuaExceptionHandler::failTransaction(lua_State* ls){
    const char* fTransaction = lua_tostring(ls, 1);
    const char* reason = lua_tostring(ls, 2);
    TestinAgentHelper::failTransaction(fTransaction, reason);
    
    return 0;
}

int TestinLuaExceptionHandler::cancelTransaction(lua_State* ls){
    const char* cTransaction = lua_tostring(ls, 1);
    const char* reason = lua_tostring(ls, 2);
    TestinAgentHelper::cancelTransaction(cTransaction, reason);
    
    return 0;
}


