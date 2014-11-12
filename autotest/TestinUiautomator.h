#ifndef __CLASS_TESTIN_UIAUTOMATOR__
#define __CLASS_TESTIN_UIAUTOMATOR__

#include <list>
#include <string>
#include "cocos_inc.h"
#include "pthread.h"

#define MAX_FPS_COUNT 60
#define FPS_INTERVAL 1000
#define TESTIN_CC_VERSION "0.1.0"

NS_CC_BEGIN

class TestinUIAutomator
{
public:

	static TestinUIAutomator* getInstance();
	void startServer();
	void registerScene(void* scene, const char* name);
	const char* sceneName(void* scene);
	const std::string dump();

private:
	TestinUIAutomator();
	~TestinUIAutomator();
	static long getCurrentTime();
	
private:
	pthread_t mPidSocket;
	static pthread_mutex_t sSocketMutex;
	static void* threadSocketRun(void *r);
	static int parseSocketReadBuff(const char* buff);

	pthread_t mPidFPS;
	static pthread_mutex_t sFPSMutex;
	static void* threadFPSRun(void *r);

	static float mFPSArray[MAX_FPS_COUNT];
	static int mFPSCount;
private:
	static int sServerSocket;
	static bool initSocketServer();
	static void startSocketServer();

	static const std::string getFPSString();
	static const std::string packageHttpResponse(const char* body);

private:
	int mSceneCount;
	void** mSceneList;
	char** mSceneName;
};
NS_CC_END


#endif	//__CLASS_TESTIN_UIAUTOMATOR__
