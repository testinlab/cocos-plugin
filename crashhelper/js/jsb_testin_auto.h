#ifndef jsb_jsb_testin_auto_h
#define jsb_jsb_testin_auto_h

#include "cocos2d.h"

std::string testin_leaveBreadcrumb(const char * breadcrumb);
bool jsb_testin_leaveBreadcrumb(JSContext *cx, uint32_t argc, JS::Value *vp);
void register_jsb_testin_all(JSContext* cx, JSObject* obj);

#endif