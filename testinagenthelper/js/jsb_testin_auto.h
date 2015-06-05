#ifndef jsb_jsb_testin_auto_h
#define jsb_jsb_testin_auto_h

#include "cocos2d.h"
#include "pre_def.h"
#include "cocos2d_specifics.hpp"

std::string testin_leaveBreadcrumb(const char * breadcrumb);
bool jsb_testin_leaveBreadcrumb(JSContext *cx, uint32_t argc, JS::Value *vp);
std::string testin_beginTransaction(const char * bTransaction);
bool jsb_testin_beginTransaction(JSContext *cx, uint32_t argc, JS::Value *vp);
std::string testin_endTransaction(const char * eTransaction);
bool jsb_testin_endTransaction(JSContext *cx, uint32_t argc, JS::Value *vp);
std::string testin_failTransaction(const char * fTransaction, const char * reason);
bool jsb_testin_failTransaction(JSContext *cx, uint32_t argc, JS::Value *vp);
std::string testin_cancelTransaction(const char * cTransaction, const char * reason);
bool jsb_testin_cancelTransaction(JSContext *cx, uint32_t argc, JS::Value *vp);
void register_jsb_testin_all(JSContext* cx, TJSObject obj);

#endif