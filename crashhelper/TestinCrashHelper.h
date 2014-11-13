#ifndef __TESIN_CRASH_HELPER_H__	
#define __TESIN_CRASH_HELPER_H__



#define EXCEPTION_TYPE_JAVA = 0;
#define EXCEPTION_TYPE_CPP = 1;
#define EXCEPTION_TYPE_OBJC = 2;
#define EXCEPTION_TYPE_LUA = 3;
#define EXCEPTION_TYPE_JS = 4;

class  TestinCrashHelper
{
public:
	static void reportException(int type, const char* reason, const char* traceback);
	static void setUserInfo(const char* userInfo);

};

#endif  // __TESIN_CRASH_HELPER_H__

