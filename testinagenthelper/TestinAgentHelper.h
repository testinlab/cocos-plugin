#ifndef __TESTIN_AGENT_HELPER_H__
#define __TESTIN_AGENT_HELPER_H__


#define EXCEPTION_TYPE_JAVA	0
#define EXCEPTION_TYPE_CPP	1
#define EXCEPTION_TYPE_OBJC	2
#define EXCEPTION_TYPE_LUA	3
#define EXCEPTION_TYPE_JS	4

#include <jni.h>

class TestinAgentHelper
{
private:
    static bool _initialed;
    
    static jobject initConfig(JNIEnv* env, jobject objAgentConfig);
public:
    static void initTestinAgent( const char* appKey, const char* channel );
    
    
    static void reportException( int type, const char* reason, const char* traceback );
    
    
    static void setUserInfo( const char* userInfo );
    
    
    static void setLocalDebug( bool isDebug );
    
    
    static void leaveBreadcrumb( const char* breadcrumb );
    
    
    static void initTestinAgent();
};

#endif  /* __TESTIN_AGENT_HELPER_H__ */
