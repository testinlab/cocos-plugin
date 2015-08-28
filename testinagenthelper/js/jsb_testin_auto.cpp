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

#endif

std::string testin_leaveBreadcrumb(const char* breadcrumb) {
    TestinAgentHelper::leaveBreadcrumb(breadcrumb);
    return "testin_leaveBreadcrumb";
}

void register_jsb_testin_all(JSContext *cx, TJSObject obj) {
    JS_DefineFunction(cx, obj, "testinLeaveBreadcrumb", jsb_testin_leaveBreadcrumb, 1, 0);
}



