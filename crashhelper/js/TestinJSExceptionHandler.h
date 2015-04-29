#ifndef __TESIN_JS_EXCEPTION_HANDLER_H__	
#define __TESIN_JS_EXCEPTION_HANDLER_H__

#include "cocos2d.h"
#include "jsapi.h"

class  TestinJSExcetionHandler
{
public:
	static void registerJSExceptionHandler(JSContext *cx);
	static void reportError(JSContext *cx, const char *message, JSErrorReport *report);
};

#endif  // __TESIN_JS_EXCEPTION_HANDLER_H__

