#ifndef __TESTIN_AGENT_HELPER_H__
#define __TESTIN_AGENT_HELPER_H__


#define EXCEPTION_TYPE_JAVA	0
#define EXCEPTION_TYPE_CPP	1
#define EXCEPTION_TYPE_OBJC	2
#define EXCEPTION_TYPE_LUA	3
#define EXCEPTION_TYPE_JS	4

class TestinAgentHelper
{
private:
    static bool _initialed;
    
    static void callTestinAgentMethod(const char* methodName, const char* methodParam, const char* param);
    
    static void callTestinAgentMethod(const char* methodName, const char* methodParam, const char* param1, const char* param2);
public:
    static void initTestinAgent( const char* appkey, const char* channel );
    
    static void reportException( int type, const char* reason, const char* traceback );
    
    static void setLocalDebug( bool isDebug );
    
    static void initTestinAgent();
    
    static void setUserInfo( const char* userInfo );
    
    static void leaveBreadcrumb( const char* breadcrumb );
};

#endif  /* __TESTIN_AGENT_HELPER_H__ */
