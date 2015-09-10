#include <string.h>
#include "cocos2d.h"
#include "TestinAgentHelper.h"
#include "TestinAgentConfig.h"

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
#define AGENT_METHOD_REPORTURL "reportURL"
#define AGENT_METHOD_REPORTURL_PARAMETER "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JJJI)V"
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
#import "TestinAgent.h"
#define AGENT_CLASS @"TestinAgent"
#define AGENT_CLASS_CONFIG @"TestinConfig"
#define AGENT_METHOD_INIT @"init:channel:"
#define AGENT_METHOD_INIT_ @"init:channel:config"
#define AGENT_METHOD_EXCEPTION @"reportCustomizedException:message:stackTrace:"
#define AGENT_METHOD_USERINFO "setUserInfo:"
#define AGENT_METHOD_LEAVEBREADCRUMB "leaveBreadcrumbWithString:"
#endif


bool TestinAgentHelper::_initialed = false;

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
        if(env->ExceptionCheck())
        {
            env->ExceptionDescribe();
            env->ExceptionClear();
            LOGE("Exception initClassLoaderForMultiThread COCOS_ACTIVITY_CLASS is exception");
            return;
        }

        jmethodID method = env->GetStaticMethodID(clz, COCOS_ACTIVITY_METHOD_CONTEXT, COCOS_ACTIVITY_METHOD_CONTEXT_PARAMETER);
        jobject objCtx = (jobject) env->CallStaticObjectMethod(clz, method);
        if (NULL == objCtx) {
            LOGD("Could not find Cocos2dxActivity object!");
            return;
        } else {
            LOGD("Found Cocos2dxActivity object!");
            //will throw ClassNotFoundException if Testin crash sdk is not included
            jclass agent = env->FindClass(AGENT_CLASS);
            if(env->ExceptionCheck())
            {
                env->ExceptionDescribe();
                env->ExceptionClear();
                LOGE("Exception initClassLoaderForMultiThread AGENT_CLASS is exception");
                return;
            }

            jclass agentConfigBuilder = env->FindClass(AGENT_CLASS_CONFIG_BUILDER);
            if(env->ExceptionCheck())
            {
                env->ExceptionDescribe();
                env->ExceptionClear();
                LOGE("Exception initClassLoaderForMultiThread AGENT_CLASS_CONFIG_BUILDER is exception");
                return;
            }
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
                        if(env->ExceptionCheck())
                        {
                            env->ExceptionDescribe();
                            env->ExceptionClear();
                            LOGE("Exception initClassLoaderForMultiThread AGENT_CLASS_CONFIG is exception");
                            return;
                        }
                        
                        jmethodID mSetAppKey = env->GetMethodID(agentConfig, "setAppKey", "(Ljava/lang/String;)V");
                        if(env->ExceptionCheck())
                        {
                            env->ExceptionDescribe();
                            env->ExceptionClear();
                            LOGE("TestinAgentHelper GetMethodID setAppKey is exception");
                            return;
                        }
                        jstring strAppkey = env->NewStringUTF(appKey);
                        env->CallVoidMethod(objAgentConfig, mSetAppKey, strAppkey);
                        if(env->ExceptionCheck())
                        {
                            env->ExceptionDescribe();
                            env->ExceptionClear();
                            LOGE("TestinAgentHelper setAppKey is exception");
                            return;
                        }
                        
                        jmethodID mSetAppChannel = env->GetMethodID(agentConfig, "setAppChannel", "(Ljava/lang/String;)V");
                        jstring strAppChannel = env->NewStringUTF(appChannel);
                        env->CallVoidMethod(objAgentConfig, mSetAppChannel, strAppChannel);
                        
                        jmethodID mSetUserInfo = env->GetMethodID(agentConfig, "setUserInfo", "(Ljava/lang/String;)V");
                        jstring strUserInfo = env->NewStringUTF(userInfo);
                        env->CallVoidMethod(objAgentConfig, mSetUserInfo, strUserInfo);
                        
                        jmethodID mSetDebug = env->GetMethodID(agentConfig, "setDebug", "(Z)V");
                        env->CallVoidMethod(objAgentConfig, mSetDebug, isDebug);
                        
                        jmethodID mSetAppLogPermission = env->GetMethodID(agentConfig, "setAppLogPermission", "(Z)V");
                        env->CallVoidMethod(objAgentConfig, mSetAppLogPermission, lPer);
                        
                        jmethodID mSetAppActPermission = env->GetMethodID(agentConfig, "setAppActPermission", "(Z)V");
                        env->CallVoidMethod(objAgentConfig, mSetAppActPermission, aPer);
                        
                        jmethodID mSetCollectNDKCrash = env->GetMethodID(agentConfig, "setCollectNDKCrash", "(Z)V");
                        env->CallVoidMethod(objAgentConfig, mSetCollectNDKCrash, isNCh);
                        
                        jmethodID mSetOpenCrash = env->GetMethodID(agentConfig, "setOpenCrash", "(Z)V");
                        env->CallVoidMethod(objAgentConfig, mSetOpenCrash, isCh);
                        
                        jmethodID mSetOpenEx = env->GetMethodID(agentConfig, "setOpenEx", "(Z)V");
                        env->CallVoidMethod(objAgentConfig, mSetOpenEx, isEx);
                        
                        jmethodID mSetReportOnBack = env->GetMethodID(agentConfig, "setReportOnBack", "(Z)V");
                        env->CallVoidMethod(objAgentConfig, mSetReportOnBack, isRWifi);
                        
                        jmethodID mReportOnlyWifi = env->GetMethodID(agentConfig, "setReportOnlyWifi", "(Z)V");
                        env->CallVoidMethod(objAgentConfig, mReportOnlyWifi, isRBack);
                        
                        env->CallStaticVoidMethod(agent, mInit, objAgentConfig);
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

        //create config object
        if (NULL == appKey || NULL == appChannel) {
            NSLog(@"Could not complete opertion because reason or traceback is null!");
            return;
        }
        
        TestinConfig *testCon = [TestinConfig defaultConfig];
        testCon.collectorNSURLConnection = collectorNSURLConnection;
        testCon.collectorNSURLSession = collectorNSURLSession;
        testCon.enabledMonitorException = enabledMonitorException;
        testCon.useLocationInfo = useLocationInfo;
        testCon.useSecurityHttpConnection = useSecurityHttpConnection;
        testCon.reportOnlyWIFI = reportOnlyWIFI;
        testCon.uploadLogLevel = uploadLogLevel;
        testCon.printLogForDebug = printLogForDebug;
        
        //#ifndef NDEBUG
        testCon.printLogForDebug = YES;
        //#else
        testCon.collectorNSURLConnection = YES;
        testCon.collectorNSURLSession = YES;
        //#endif
        
        NSString *ocAppkey = [NSString stringWithUTF8String:appKey];
        NSString *ocChannelId = [NSString stringWithUTF8String:appChannel];

        [TestinAgent init:ocAppkey channel:ocChannelId config:testCon];

#endif
        _initialed = true;
    }
}

void TestinAgentHelper::initTestinAgent(const char* appkey, const char* channel) {
    appKey = appkey;
    appChannel = channel;
    initTestinAgent();
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
        if (NULL == reason || NULL == traceback) {
            NSLog(@"Could not complete opertion because reason or traceback is null!");
            return;
        }
        NSNumber *ocType = [NSNumber numberWithInt:type];
        NSString *ocReason = [NSString stringWithUTF8String:reason];
        NSString *ocStraceback = [NSString stringWithUTF8String:traceback];

        [TestinAgent reportCustomizedException:ocType message:ocReason stackTrace:ocStraceback];
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

void TestinAgentHelper::setUserInfo(const char* userInfo) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    callTestinAgentMethod(AGENT_METHOD_USERINFO, AGENT_METHOD_USERINFO_PARAMETER, userInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (NULL == userInfo) {
        NSLog(@"userinfo is null!");
        return;
    }
     NSString *ocUser = [NSString stringWithUTF8String:userInfo];
    [TestinAgent setUserInfo:ocUser];
#endif
}

void TestinAgentHelper::leaveBreadcrumb(const char* Breadcrumb) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    callTestinAgentMethod(AGENT_METHOD_LEAVEBREADCRUMB, AGENT_METHOD_LEAVEBREADCRUMB_PARAMETER, Breadcrumb);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (NULL == Breadcrumb) {
        NSLog(@"Breadcrumb is null!");
        return;
    }
    NSString *ocBreadcrumb = [NSString stringWithUTF8String:Breadcrumb];
    [TestinAgent leaveBreadcrumbWithString:ocBreadcrumb];
//    NSString *a = @"avd";
//    [a substringFromIndex:15];
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

