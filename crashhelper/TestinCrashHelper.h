#ifndef __TESTIN_CRASH_HELPER_H__	
#define __TESTIN_CRASH_HELPER_H__



#define EXCEPTION_TYPE_JAVA 0
#define EXCEPTION_TYPE_CPP 1
#define EXCEPTION_TYPE_OBJC 2
#define EXCEPTION_TYPE_LUA 3
#define EXCEPTION_TYPE_JS 4

class  TestinCrashHelper
{
private:
	static bool _initialed;
public:
	static void initTestinAgent(const char* appKey, const char* channel);
	static void reportException(int type, const char* reason, const char* traceback);
	static void setUserInfo(const char* userInfo);

public:
	//for Android only, you can use these to functions instead of Java functions
	static void onActivityStart();
	static void onActivityStop();

};

#endif  // __TESTIN_CRASH_HELPER_H__

