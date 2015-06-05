#include <string.h>
#include "jsb_testin_auto.h"
#include "../TestinAgentHelper.h"

#if COCOS2D_VERSION >= 0x00030500
bool jsb_testin_leaveBreadcrumb(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        const char* arg0;
        jsval_to_charptr(cx, args.get(0), &arg0);
        std_string_to_jsval(cx, testin_leaveBreadcrumb(arg0));
        args.rval().setUndefined();
    }
    
    return true;
}

bool jsb_testin_beginTransaction(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        const char* arg0;
        jsval_to_charptr(cx, args.get(0), &arg0);
        std_string_to_jsval(cx, testin_beginTransaction(arg0));
        args.rval().setUndefined();
    }
    
    return true;
}

bool jsb_testin_endTransaction(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        const char* arg0;
        jsval_to_charptr(cx, args.get(0), &arg0);
        std_string_to_jsval(cx, testin_endTransaction(arg0));
        args.rval().setUndefined();
    }
    
    return true;
}

bool jsb_testin_failTransaction(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        const char* arg0;
        const char* arg1;
        jsval_to_charptr(cx, args.get(0), &arg0);
        jsval_to_charptr(cx, args.get(1), &arg1);
        std_string_to_jsval(cx, testin_failTransaction(arg0, arg1));
        args.rval().setUndefined();
    }
    
    return true;
}

bool jsb_testin_cancelTransaction(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        const char* arg0;
        const char* arg1;
        jsval_to_charptr(cx, args.get(0), &arg0);
        jsval_to_charptr(cx, args.get(1), &arg1);
        std_string_to_jsval(cx, testin_cancleTransaction(arg0, arg1));
        args.rval().setUndefined();
    }
    
    return true;
}
#else
bool jsb_testin_leaveBreadcrumb(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        jsval *argvp = JS_ARGV(cx, vp);
        const char* arg0;
        jsval_to_charptr(cx, *argvp++, &arg0);
        jsval ret = std_string_to_jsval(cx, testin_leaveBreadcrumb(arg0));
        JS_SET_RVAL(cx, vp, ret);
    }
    
    return true;
}

bool jsb_testin_beginTransaction(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        jsval *argvp = JS_ARGV(cx, vp);
        const char* arg0;
        jsval_to_charptr(cx, *argvp++, &arg0);
        jsval ret = std_string_to_jsval(cx, testin_beginTransaction(arg0));
        JS_SET_RVAL(cx, vp, ret);
    }
    
    return true;
}
bool jsb_testin_endTransaction(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        jsval *argvp = JS_ARGV(cx, vp);
        const char* arg0;
        jsval_to_charptr(cx, *argvp++, &arg0);
        jsval ret = std_string_to_jsval(cx, testin_endTransaction(arg0));
        JS_SET_RVAL(cx, vp, ret);
    }
    
    return true;
}
bool jsb_testin_failTransaction(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        jsval *argvp = JS_ARGV(cx, vp);
        const char* arg0;
        const char* arg1;
        jsval_to_charptr(cx, *argvp++, &arg0);
        jsval_to_charptr(cx, *argvp++, &arg1);
        jsval ret = std_string_to_jsval(cx, testin_failTransaction(arg0, arg1));
        JS_SET_RVAL(cx, vp, ret);
    }
    
    return true;
}

bool jsb_testin_cancelTransaction(JSContext *cx, uint32_t argc, JS::Value *vp) {
    if(argc > 0)
    {
        jsval *argvp = JS_ARGV(cx, vp);
        const char* arg0;
        const char* arg1;
        jsval_to_charptr(cx, *argvp++, &arg0);
        jsval_to_charptr(cx, *argvp++, &arg1);
        jsval ret = std_string_to_jsval(cx, testin_cancelTransaction(arg0, arg1));
        JS_SET_RVAL(cx, vp, ret);
    }
    
    return true;
}

#endif

std::string testin_leaveBreadcrumb(const char* breadcrumb) {
    TestinAgentHelper::leaveBreadcrumb(breadcrumb);
    return "testin_leaveBreadcrumb";
}

std::string testin_beginTransaction(const char* bTransaction) {
    TestinAgentHelper::beginTransaction(bTransaction);
    return "testin_beginTransaction";
}

std::string testin_endTransaction(const char* eTransaction) {
    TestinAgentHelper::endTransaction(eTransaction);
    return "testin_endTransaction";
}

std::string testin_failTransaction(const char* fTransaction, const char * reason) {
    TestinAgentHelper::failTransaction(fTransaction, reason);
    return "testin_fialTransaction";
}

std::string testin_cancelTransaction(const char* cTransaction, const char * reason) {
    TestinAgentHelper::cancelTransaction(cTransaction, reason);
    return "testin_cancelTransaction";
}

void register_jsb_testin_all(JSContext *cx, TJSObject obj) {
    JS_DefineFunction(cx, obj, "testinLeaveBreadcrumb", jsb_testin_leaveBreadcrumb, 1, 0);
    JS_DefineFunction(cx, obj, "testinBeginTransaction", jsb_testin_beginTransaction, 1, 0);
    JS_DefineFunction(cx, obj, "testinEndTransaction", jsb_testin_endTransaction, 1, 0);
    JS_DefineFunction(cx, obj, "testinFailTransaction", jsb_testin_failTransaction, 2, 0);
    JS_DefineFunction(cx, obj, "testinCancelTransaction", jsb_testin_cancelTransaction, 2, 0);
}



