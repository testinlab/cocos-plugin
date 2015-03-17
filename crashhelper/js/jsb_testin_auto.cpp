#include <string.h>
#include "jsb_testin_auto.h"
#include "cocos2d_specifics.hpp"
#include "../TestinCrashHelper.h"

std::string testin_leaveBreadcrumb(const char* breadcrumb) {
    TestinCrashHelper::leaveBreadcrumb(breadcrumb);
    return "testin_leaveBreadcrumb";
}

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

void register_jsb_testin_all(JSContext *cx, JSObject *obj) {
    JS_DefineFunction(cx, obj, "testinLeaveBreadcrumb", jsb_testin_leaveBreadcrumb, 1, 0);
}