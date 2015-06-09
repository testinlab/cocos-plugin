#include <string.h>
#include "TestinAgentHelper.h"
#include "TestinAgentConfig.h"
#include "cocos2d.h"

#define LOG_TAG "TestinAgentHelper"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"

#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#define AGENT_CLASS "com/testin/agent/TestinAgent"
#define AGENT_CLASS_CONFIG "com/testin/agent/TestinAgentConfig"
#define AGENT_CLASS_CONFIG_BUILDER "com/testin/agent/TestinAgentConfig$Builder"
#define AGENT_METHOD_CONFIG_BUILDER "build"
#define AGENT_METHOD_CONFIG_BUILDER_PARAMETER "()Lcom/testin/agent/TestinAgentConfig;"
#define AGENT_METHOD_INIT "init"
#define AGENT_METHOD_INIT_PARAMETER "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)V"
#define AGENT_METHOD_INIT_PARAMETER_ "(Lcom/testin/agent/TestinAgentConfig;)V"
#define AGENT_METHOD_EXCEPTION "reportCustomizedException"
#define AGENT_METHOD_EXCEPTION_PARAMETER "(ILjava/lang/String;Ljava/lang/String;)V"
#define AGENT_METHOD_USERINFO "setUserInfo"
#define AGENT_METHOD_USERINFO_PARAMETER "(Ljava/lang/String;)V"
#define AGENT_METHOD_ISDEBUG "setLocalDebug"
#define AGENT_METHOD_ISDEBUG_PARAMETER "(Z)V"
#define AGENT_METHOD_LEAVEBREADCRUMB "leaveBreadcrumb"
#define AGENT_METHOD_LEAVEBREADCRUMB_PARAMETER "(Ljava/lang/String;)V"
#define AGENT_METHOD_BTRANSACTION "beginTransaction"
#define AGENT_METHOD_BTRANSACTION_PARAMETER "(Ljava/lang/String;)V"
#define AGENT_METHOD_ETRANSACTION "endTransaction"
#define AGENT_METHOD_ETRANSACTION_PARAMETER "(Ljava/lang/String;)V"
#define AGENT_METHOD_FTRANSACTION "failTransaction"
#define AGENT_METHOD_FTRANSACTION_PARAMETER "(Ljava/lang/String;Ljava/lang/String;)V"
#define AGENT_METHOD_CTRANSACTION "cancelTransaction"
#define AGENT_METHOD_CTRANSACTION_PARAMETER "(Ljava/lang/String;Ljava/lang/String;)V"
#define AGENT_NONE_PARAMETER "()V"
#define AGENT_CONTEXT_PARAMETER "(Landroid/content/Context;)V"
#define COCOS_ACTIVITY_CLASS "org/cocos2dx/lib/Cocos2dxActivity"
#define COCOS_ACTIVITY_METHOD_CONTEXT "getContext"
#define COCOS_ACTIVITY_METHOD_CONTEXT_PARAMETER "()Landroid/content/Context;"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <UIKit/UIKit.h>
#define AGENT_CLASS @"TestinAgent"
#define AGENT_CLASS_CONFIG @"TestinConfig"
#define AGENT_METHOD_INIT @"init:channel:"
#define AGENT_METHOD_INIT_ @"init:channel:config"
#define AGENT_METHOD_EXCEPTION @"reportCustomizedException:message:stackTrace:"
#define AGENT_METHOD_USERINFO "setUserInfo:"
#define AGENT_METHOD_LEAVEBREADCRUMB "leaveBreadcrumbWithString:"
#define AGENT_METHOD_BTRANSACTION "beginTransaction"
#define AGENT_METHOD_ETRANSACTION "endTransaction"
#define AGENT_METHOD_FTRANSACTION "failTransaction"
#define AGENT_METHOD_CTRANSACTION "cancelTransaction"
#endif


bool TestinAgentHelper::_initialed = false;

void TestinAgentHelper::initTestinAgent(const char* appKey, const char* channel) {
    if (!_initialed) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
        JNIEnv* env = NULL;
        jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        if (NULL == jvm || NULL == env) {
            LOGE("Could not complete opertion because JavaVM or JNIEnv is null!");
            return;
        }
        jvm->AttachCurrentThread(&env, 0);
        
        jclass clz = env->FindClass(COCOS_ACTIVITY_CLASS);
        jmethodID method = env->GetStaticMethodID(clz, COCOS_ACTIVITY_METHOD_CONTEXT, COCOS_ACTIVITY_METHOD_CONTEXT_PARAMETER);
        jobject obj = (jobject) env->CallStaticObjectMethod(clz, method);
        if (NULL == obj) {
            LOGD("Could not find Cocos2dxActivity object!");
            return;
        } else {
            LOGD("Found Cocos2dxActivity object!");
            //will throw ClassNotFoundException if Testin crash sdk is not included
            clz = env->FindClass(AGENT_CLASS);
            //will throw NoSuchMethodException if Testin crash sdk is not included
            method = env->GetStaticMethodID(clz, AGENT_METHOD_INIT, AGENT_METHOD_INIT_PARAMETER);
            
            jstring strParam1 = env->NewStringUTF(appKey);
            jstring strParam2 = (NULL == channel) ? NULL : env->NewStringUTF(channel);
            env->CallStaticVoidMethod(clz, method, obj, strParam1, strParam2);
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NSString* strParam1 = @(appKey);
        NSString* strParam2 = (NULL == channel) ? nil : @(channel);
        Class cls = NSClassFromString(AGENT_CLASS);
        SEL func = NSSelectorFromString(AGENT_METHOD_INIT);
        NSMethodSignature* signature = [cls methodSignatureForSelector:func];
        NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
        [invocation setTarget:cls];
        [invocation setSelector:func];
        [invocation setArgument:&strParam1 atIndex:2];
        [invocation setArgument:&strParam2 atIndex:3];
        [invocation invoke];
#endif
        _initialed = true;
    }
}


void TestinAgentHelper::reportException(int type, const char* reason, const char* traceback) {
    if (_initialed) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
        JNIEnv* env = NULL;
        jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        if (NULL == jvm || NULL == env) {
            LOGE("Could not complete opertion because JavaVM or JNIEnv is null!");
            return;
        }
        jvm->AttachCurrentThread(&env, 0);
        
        //will throw ClassNotFoundException if Testin crash sdk is not included
        jclass clz = env->FindClass(AGENT_CLASS);
        //will throw NoSuchMethodException if Testin crash sdk is not included
        jmethodID method = env->GetStaticMethodID(clz, AGENT_METHOD_EXCEPTION, AGENT_METHOD_EXCEPTION_PARAMETER);
        
        jstring strParam1 = env->NewStringUTF(reason);
        jstring strParam2 = env->NewStringUTF(traceback);
        env->CallStaticVoidMethod(clz, method, type, strParam1, strParam2);
        //env->DeleteLocalRef(strParam1);
        //env->DeleteLocalRef(strParam2);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NSNumber* num = @(type);
        NSString* strParam1 = @(reason);
        NSString* strParam2 = @(traceback);
        Class cls = NSClassFromString(AGENT_CLASS);
        SEL func = NSSelectorFromString(AGENT_METHOD_EXCEPTION);
        NSMethodSignature* signature = [cls methodSignatureForSelector:func];
        NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
        [invocation setTarget:cls];
        [invocation setSelector:func];
        [invocation setArgument:&num atIndex:2];
        [invocation setArgument:&strParam1 atIndex:3];
        [invocation setArgument:&strParam2 atIndex:4];
        [invocation invoke];
#endif
    }else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        LOGE("Please init TestinAgent first!!!");
#endif
    }
}

void TestinAgentHelper::setLocalDebug(bool isDebug) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
    JNIEnv* env = NULL;
    jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
    
    if (NULL == jvm || NULL == env) {
        LOGE("Could not complete opertion because JavaVM or JNIEnv is null!");
        return;
    }
    jvm->AttachCurrentThread(&env, 0);
    
    //will throw ClassNotFoundException if Testin crash sdk is not included
    jclass clz = env->FindClass(AGENT_CLASS);
    //will throw NoSuchMethodException if Testin crash sdk is not included
    jmethodID method = env->GetStaticMethodID(clz, AGENT_METHOD_ISDEBUG, AGENT_METHOD_ISDEBUG_PARAMETER);
    
    env->CallStaticVoidMethod(clz, method, isDebug);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
#endif
}

void TestinAgentHelper::initTestinAgent(){
    if (!_initialed) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
        JNIEnv* env = NULL;
        jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        if (NULL == jvm || NULL == env) {
            LOGE("Could not complete opertion because JavaVM or JNIEnv is null!");
            return;
        }
        jvm->AttachCurrentThread(&env, 0);
        
        jclass clz = env->FindClass(COCOS_ACTIVITY_CLASS);
        jmethodID method = env->GetStaticMethodID(clz, COCOS_ACTIVITY_METHOD_CONTEXT, COCOS_ACTIVITY_METHOD_CONTEXT_PARAMETER);
        jobject objCtx = (jobject) env->CallStaticObjectMethod(clz, method);
        if (NULL == objCtx) {
            LOGD("Could not find Cocos2dxActivity object!");
            return;
        } else {
            LOGD("Found Cocos2dxActivity object!");
            //will throw ClassNotFoundException if Testin crash sdk is not included
            jclass agent = env->FindClass(AGENT_CLASS);
            jclass agentConfigBuilder = env->FindClass(AGENT_CLASS_CONFIG_BUILDER);
            //will throw NoSuchMethodException if Testin crash sdk is not included
            
            jmethodID mAgentConfigBuilder = env->GetMethodID(agentConfigBuilder, "<init>", AGENT_CONTEXT_PARAMETER);
            jmethodID mConfigBuild = env->GetMethodID(agentConfigBuilder, AGENT_METHOD_CONFIG_BUILDER, AGENT_METHOD_CONFIG_BUILDER_PARAMETER);
            jmethodID mInit = env->GetStaticMethodID(agent, AGENT_METHOD_INIT, AGENT_METHOD_INIT_PARAMETER_);
            
            jobject objAgentConfigBuilder =(jobject) env->NewObject(agentConfigBuilder, mAgentConfigBuilder, objCtx);
            
            if(NULL != objAgentConfigBuilder){
                if(NULL != mConfigBuild){
                    jobject objAgentConfig = (jobject) env->CallObjectMethod(objAgentConfigBuilder, mConfigBuild);
                    if(NULL != objAgentConfig){
                        
                        jclass agentConfig = env->FindClass(AGENT_CLASS_CONFIG);
                        
                        jmethodID mSetAppKey = env->GetMethodID(agentConfig, "setAppKey", "(Ljava/lang/String;)V");
                        jstring strAppkey = env->NewStringUTF(appKey);
                        env->CallObjectMethod(objAgentConfig, mSetAppKey, strAppkey);
                        
                        jmethodID mSetAppChannel = env->GetMethodID(agentConfig, "setAppChannel", "(Ljava/lang/String;)V");
                        jstring strAppChannel = env->NewStringUTF(appChannel);
                        env->CallObjectMethod(objAgentConfig, mSetAppChannel, strAppChannel);
                        
                        jmethodID mSetUserInfo = env->GetMethodID(agentConfig, "setUserInfo", "(Ljava/lang/String;)V");
                        jstring strUserInfo = env->NewStringUTF(userInfo);
                        env->CallObjectMethod(objAgentConfig, mSetUserInfo, strUserInfo);
                        
                        jmethodID mSetDebug = env->GetMethodID(agentConfig, "setDebug", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mSetDebug, isDebug);
                        
                        jmethodID mSetAppLogPermission = env->GetMethodID(agentConfig, "setAppLogPermission", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mSetAppLogPermission, lPer);
                        
                        jmethodID mSetAppActPermission = env->GetMethodID(agentConfig, "setAppActPermission", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mSetAppActPermission, aPer);
                        
                        jmethodID mSetCollectNDKCrash = env->GetMethodID(agentConfig, "setCollectNDKCrash", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mSetCollectNDKCrash, isNCh);
                        
                        jmethodID mSetOpenAPM = env->GetMethodID(agentConfig, "setOpenAPM", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mSetOpenAPM, isAPM);
                        
                        jmethodID mSetOpenCrash = env->GetMethodID(agentConfig, "setOpenCrash", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mSetOpenCrash, isCh);
                        
                        jmethodID mSetOpenEx = env->GetMethodID(agentConfig, "setOpenEx", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mSetOpenEx, isEx);
                        
                        jmethodID mSetReportOnBack = env->GetMethodID(agentConfig, "setReportOnBack", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mSetReportOnBack, isRWifi);
                        
                        jmethodID mReportOnlyWifi = env->GetMethodID(agentConfig, "setReportOnlyWifi", "(Z)V");
                        env->CallObjectMethod(objAgentConfig, mReportOnlyWifi, isRBack);
                        
                        env->CallStaticObjectMethod(agent, mInit, objAgentConfig);
                    }else{
                        LOGD("Could not find TestinAgentConfig object!");
                        return;
                    }
                }else{
                    LOGD("Could not find the build mehtod of the TestinAgentConfigBuilder!");
                }
            }else{
                LOGD("Could not find TestinAgentConfigBuilder object!");
                return;
            }
        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NSString* strParam1 = @(appKey);
        NSString* strParam2 = @(appChannel);
        Class cAgent = NSClassFromString(AGENT_CLASS);
        SEL funcInit = NSSelectorFromString(AGENT_METHOD_INIT_);
        NSMethodSignature* signature = [cAgent methodSignatureForSelector:funcInit];
        NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
        [invocation setTarget:cAgent];
        [invocation setSelector:funcInit];
        [invocation setArgument:&strParam1 atIndex:2];
        [invocation setArgument:&strParam2 atIndex:3];
        [invocation invoke];
#endif
        _initialed = true;
    }
}

void TestinAgentHelper::setUserInfo(const char* userInfo) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    callTestinAgentMethod(AGENT_METHOD_USERINFO, AGENT_METHOD_USERINFO_PARAMETER, userInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    callTestinAgentMethod(AGENT_METHOD_USERINFO, "", userInfo);
#endif
}

void TestinAgentHelper::leaveBreadcrumb(const char* Breadcrumb) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    callTestinAgentMethod(AGENT_METHOD_LEAVEBREADCRUMB, AGENT_METHOD_LEAVEBREADCRUMB_PARAMETER, Breadcrumb);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    callTestinAgentMethod(AGENT_METHOD_LEAVEBREADCRUMB, "", Breadcrumb);
#endif
}

void TestinAgentHelper::beginTransaction(const char* bTransaction) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    callTestinAgentMethod(AGENT_METHOD_BTRANSACTION, AGENT_METHOD_BTRANSACTION_PARAMETER, bTransaction);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    callTestinAgentMethod(AGENT_METHOD_BTRANSACTION, "", bTransaction);
#endif
}

void TestinAgentHelper::endTransaction(const char* eTransaction) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    callTestinAgentMethod(AGENT_METHOD_ETRANSACTION, AGENT_METHOD_ETRANSACTION_PARAMETER, eTransaction);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    callTestinAgentMethod(AGENT_METHOD_ETRANSACTION, "", eTransaction);
#endif
}

void TestinAgentHelper::failTransaction(const char* fTransaction, const char* reason) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    callTestinAgentMethod(AGENT_METHOD_FTRANSACTION, AGENT_METHOD_FTRANSACTION_PARAMETER, fTransaction, reason);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    callTestinAgentMethod(AGENT_METHOD_FTRANSACTION, "", fTransaction, reason);
#endif
}

void TestinAgentHelper::cancelTransaction(const char* cTransaction, const char* reason) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    callTestinAgentMethod(AGENT_METHOD_CTRANSACTION, AGENT_METHOD_CTRANSACTION_PARAMETER, cTransaction, reason);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    callTestinAgentMethod(AGENT_METHOD_CTRANSACTION, "", cTransaction, reason);
#endif
}

void TestinAgentHelper::callTestinAgentMethod(const char* methodName, const char* methodParam, const char* param){
    if (_initialed) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
        JNIEnv* env = NULL;
        jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        if (NULL == jvm || NULL == env) {
            LOGE("Could not complete opertion because JavaVM or JNIEnv is null!");
            return;
        }
        jvm->AttachCurrentThread(&env, 0);
        
        //will throw ClassNotFoundException if Testin crash sdk is not included
        jclass clz = env->FindClass(AGENT_CLASS);
        //will throw NoSuchMethodException if Testin crash sdk is not included
        jmethodID method = env->GetStaticMethodID(clz, methodName, methodParam);
        jstring strParam = env->NewStringUTF(param);
        env->CallStaticVoidMethod(clz, method, strParam);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NSString* strParam = @(param);
        NSString* strMethodName = @(methodName);
        Class cls = NSClassFromString(AGENT_CLASS);
        SEL func = NSSelectorFromString(strMethodName);
        NSMethodSignature* signature = [cls methodSignatureForSelector:func];
        NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
        [invocation setTarget:cls];
        [invocation setSelector:func];
        [invocation setArgument:&strParam atIndex:2];
        [invocation invoke];
#endif
    }else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        LOGE("Please init TestinAgent first!!!");
#endif
    }
}

void TestinAgentHelper::callTestinAgentMethod(const char* methodName, const char* methodParam, const char* param1, const char* param2){
    if (_initialed) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
        JNIEnv* env = NULL;
        jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
        
        if (NULL == jvm || NULL == env) {
            LOGE("Could not complete opertion because JavaVM or JNIEnv is null!");
            return;
        }
        jvm->AttachCurrentThread(&env, 0);
        
        //will throw ClassNotFoundException if Testin crash sdk is not included
        jclass clz = env->FindClass(AGENT_CLASS);
        //will throw NoSuchMethodException if Testin crash sdk is not included
        jmethodID method = env->GetStaticMethodID(clz, methodName, methodParam);
        jstring strParam1 = env->NewStringUTF(param1);
        jstring strParam2 = env->NewStringUTF(param2);
        env->CallStaticVoidMethod(clz, method, strParam1, strParam2);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NSString* strParam1 = @(param1);
        NSString* strParam2 = @(param2);
        NSString* strMethodName = @(methodName);
        Class cls = NSClassFromString(AGENT_CLASS);
        SEL func = NSSelectorFromString(strMethodName);
        NSMethodSignature* signature = [cls methodSignatureForSelector:func];
        NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
        [invocation setTarget:cls];
        [invocation setSelector:func];
        [invocation setArgument:&strParam1 atIndex:2];
        [invocation setArgument:&strParam2 atIndex:3];
        [invocation invoke];
#endif
    }else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        LOGE("Please init TestinAgent first!!!");
#endif
    }
}

