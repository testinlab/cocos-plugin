#ifndef __TESIN_LUA_EXCEPTION_HANDLER_H__	
#define __TESIN_LUA_EXCEPTION_HANDLER_H__

#include "cocos2d.h"

class  TestinLuaExceptionHandler
{
public:
	static void registerLuaExceptionHandler();
	static int onLuaException(lua_State* ls);
    static int leaveBreadcrumb(lua_State* ls);
    static int beginTransaction(lua_State* ls);
    static int endTransaction(lua_State* ls);
    static int failTransaction(lua_State* ls);
    static int cancelTransaction(lua_State* ls);
};

#endif  // __TESIN_LUA_EXCEPTION_HANDLER_H__

