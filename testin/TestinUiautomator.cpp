#include "TestinUiautomator.h"
#include "TestinTraverser.h"
#include "cocos_inc.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>

#define TESTIN_UIAUTOMATOR_PORT 9464
#define TESTIN_UIAUTOMATOR_PORT_RANGE 8

#define CMD_UNKNOWN -1
#define CMD_CONTINUE 0
#define CMD_DUMP 1
#define CMD_FPS 2

NS_CC_BEGIN

static TestinUIAutomator* instance = NULL;
int TestinUIAutomator::sServerSocket;
pthread_mutex_t TestinUIAutomator::sSocketMutex;
pthread_mutex_t TestinUIAutomator::sFPSMutex;
float TestinUIAutomator::mFPSArray[MAX_FPS_COUNT];
int TestinUIAutomator::mFPSCount;

TestinUIAutomator* TestinUIAutomator::getInstance() {
	if (instance == NULL)
	{
		instance = new TestinUIAutomator();
	}
	return instance;
}

TestinUIAutomator::TestinUIAutomator() {
	sServerSocket = 0;
	mFPSCount = 0;
	mSceneCount = 0;
	mSceneName = NULL;
	mSceneList = NULL;
}

TestinUIAutomator::~TestinUIAutomator() {
	if (mSceneList != NULL) {
		free(mSceneList);
	}

	if (mSceneName != NULL) {
		for (int i = 0; i < mSceneCount; i ++) 
			free(*(mSceneName + sizeof(char*) * i));
		free(mSceneName);
	}

}

void TestinUIAutomator::startServer() {
	if (sServerSocket == 0)
	{
		pthread_mutex_init(&sSocketMutex, NULL);
		pthread_create(&mPidSocket, NULL, threadSocketRun, 0);

		pthread_mutex_init(&sFPSMutex, NULL);
		pthread_create(&mPidFPS, NULL, threadFPSRun, 0);
	}
}

void* TestinUIAutomator::threadSocketRun(void *r)
{
    pthread_mutex_lock(&sSocketMutex);
	if (initSocketServer())
	{
		startSocketServer();
	}
    pthread_mutex_unlock(&sSocketMutex);

    return NULL;
}

const std::string TestinUIAutomator::dump() {
	return TestinTraverser::dump();
}

void TestinUIAutomator::registerScene(void* scene, const char* name) {
	if (scene == NULL || name == NULL)
		return;

	if (mSceneCount > 0) {
		for (int i = 0; i < mSceneCount; i ++) {
			void* tmpScene = *(mSceneList + i);
			if (tmpScene == scene) {
				//found
				char* name_ = *(mSceneName + i);
				free(name_);
				name_ = (char*)malloc(strlen(name) + 1);
				strcpy(name_, name);
				name_[strlen(name)] = '\0';
				*(mSceneName + i) = name_;
				return;
			}
		}
	}

	//not found in old scenes
	if (mSceneCount == 0) {
		mSceneList = (void**)malloc(sizeof(void*));
		mSceneName = (char**)malloc(sizeof(char*));
	} else {
		mSceneList = (void**)realloc(mSceneList, sizeof(void*) * (mSceneCount + 1));
		mSceneName = (char**)realloc(mSceneName, sizeof(char*) * (mSceneCount + 1));
	}
	*(mSceneList + mSceneCount) = scene;
	char* newName = (char*)malloc(strlen(name) + 1);
	strcpy(newName, name);
	newName[strlen(name)] = '\0';
	*(mSceneName + mSceneCount) = newName;
	mSceneCount += 1;
}

const char* TestinUIAutomator::sceneName(void* scene) {
	if (scene == NULL || mSceneCount < 1)
		return NULL;

	for (int i = 0; i < mSceneCount; i ++) {
		void* tmpScene = *(mSceneList + i);
		if (tmpScene == scene) {
			return *(mSceneName + i);
		}
	}

	return NULL;
}

bool TestinUIAutomator::initSocketServer() {
	TCLog("init socket");
	if((sServerSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		TCLog("failed creating socket");
		return false;
	}

	TCLog("socket listening on port %d", TESTIN_UIAUTOMATOR_PORT);
	struct sockaddr_in serverAddr;
	int port = TESTIN_UIAUTOMATOR_PORT;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	while (bind(sServerSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		TCLog("failed to bind socket on port %d", port);
		port += 1;
		if (port >= TESTIN_UIAUTOMATOR_PORT + TESTIN_UIAUTOMATOR_PORT_RANGE)
			port = TESTIN_UIAUTOMATOR_PORT;
		serverAddr.sin_port = htons(port);
		TCLog("try to bind socket on port %d", port);

		usleep(1000 * 100);
	}
	if(listen(sServerSocket, 5) < 0) {
		TCLog("failed to listen");
		return false;
	}

	TCLog("socket init success on port: %d", port);
	return true;
}

void TestinUIAutomator::startSocketServer() {
	int client;
	int len;
	struct sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);
	char cmd[16];
	char buf[8];

	while(1) {
		TCLog("Listening on port: %d", htons(clientAddr.sin_port));
		client = accept(sServerSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addrLen);
		if(client < 0) {
			TCLog("accept");
			continue;
		}
		TCLog("recv client data...n");
		TCLog("IP is %s", inet_ntoa(clientAddr.sin_addr));
		TCLog("Port is %d", htons(clientAddr.sin_port));
		bzero(cmd, 16);

		int flags = fcntl(client,F_GETFL,0);
		if (flags == -1)
			fcntl(client, F_SETFL, O_NONBLOCK);
		else
			fcntl(client, F_SETFL, flags | O_NONBLOCK);

		char buf[16] = {0};
		int numRead, sel;
		fd_set readFlags, writeFlags;
		struct timeval waitd = {0, 1};
		char *strRead = NULL;
		bool breakSocketReceive = false;

		long t = getCurrentTime();

		while(!breakSocketReceive) {
			FD_ZERO(&readFlags);
			FD_ZERO(&writeFlags);
			FD_SET(client, &readFlags);
			FD_SET(client, &writeFlags);
			FD_SET(STDIN_FILENO, &readFlags);
			FD_SET(STDIN_FILENO, &writeFlags);

			if ((getCurrentTime() - t) > 1000)
			{
				TCLog("recv timeout");
				break;
			}

			sel = select(client+1, &readFlags, &writeFlags, (fd_set*)0, &waitd);
			switch (sel) {
				case -1:
					{
						TCLog("closing socket for select error..");
						breakSocketReceive = true;
					}
					break;
				case 0:
					continue;
				default:
					//socket ready for reading
					if(FD_ISSET(client, &readFlags)) {

						//clear set
						FD_CLR(client, &readFlags);
						memset(&buf, 0, 16);

						numRead = recv(client, buf, 16, 0);
						if(numRead <= 0) {
							TCLog("closing socket for read error");
							breakSocketReceive = true;
							break;
						}
						
						if (strRead != NULL)
						{
							char *tmp = (char *)malloc(strlen(strRead) + numRead + 1);
							memcpy(tmp, strRead, strlen(strRead));
							memcpy(tmp + strlen(strRead), buf, numRead);
							tmp[strlen(strRead) + numRead] = '\0';
							free(strRead);
							strRead = tmp;
						} else {
							strRead = (char *)malloc(numRead + 1);
							memcpy(strRead, buf, numRead);
							strRead[numRead] = '\0';
						}

						switch (parseSocketReadBuff(strRead))
						{
							case CMD_UNKNOWN:
								{
									const char* str = "HTTP/1.1 404 Not Found\r\nConnection: Close\r\n\r\nPage Not Found!";
									send(client, str, strlen(str), 0);
									breakSocketReceive = true;
								}
								break;
							case CMD_DUMP:
								{
									std::string dumpStr = instance->dump();
									std::string httpResponse = packageHttpResponse(dumpStr.c_str());
									const char* str = httpResponse.c_str();
									send(client, str, strlen(str), 0);
									breakSocketReceive = true;
								}
								break;
							case CMD_FPS:
								{
									std::string fpsStr = getFPSString();
									std::string httpResponse = packageHttpResponse(fpsStr.c_str());
									const char* httpStr = httpResponse.c_str();
									send(client, httpStr, strlen(httpStr), 0);
									breakSocketReceive = true;
								}
								break;
							default:
								continue;
						}


					}   //end if ready for read
			}
		}

		if (strRead != NULL)
			free(strRead);
		usleep(1);
		close(client);

	}
}



const std::string TestinUIAutomator::getFPSString() {
	const char *format = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><fpsarray pid=\"%d\">%s</fpsarray>";

	pthread_mutex_lock(&sFPSMutex);
	char* buff = (char*)malloc(18 * MAX_FPS_COUNT + 16);
	memset(buff, 0, 18 * MAX_FPS_COUNT + 16);

	for (int i = 0; i < mFPSCount; i++) {
		char tmp[18] = {0};
		sprintf(tmp, "<fps>%0.1f</fps>", mFPSArray[i]);
		strcat(buff, tmp);
	}
	pthread_mutex_unlock(&sFPSMutex);

	int len = strlen(format) + strlen(buff) + 8;
	char* str = (char*)malloc(len);
	memset(str, 0, len);
	sprintf(str, format, (int)getpid(), buff);
	free(buff);
	std::string ret = str;
	free(str);
	return ret;
}

const std::string TestinUIAutomator::packageHttpResponse(const char* body) {
	const char *format = "HTTP/1.1 200 OK\r\nContent-Length: %d\r\nConnection: Close\r\nContent-Type: text/xml\r\n\r\n%s";
	char* str = (char*)malloc(strlen(format) + strlen(body) + 12);
	memset(str, 0, strlen(format) + strlen(body) + 12);
	sprintf(str, format, strlen(body), body);
	std::string ret = str;
	free(str);
	return ret;
}


int TestinUIAutomator::parseSocketReadBuff(const char* buff) {
	if (buff != NULL && strlen(buff) >= 4)
	{
		//only receive get request
		if (strstr(buff, "GET ") != buff) {
			return CMD_UNKNOWN;
		}
		

		char* eof = strstr(buff, "\r\n\r\n");
		if (eof == NULL)
			return CMD_CONTINUE;

		char* end = strstr(buff, " HTTP/1.");
		if (end == NULL)
			return CMD_UNKNOWN;

		if (end > strstr(buff, "\r\n"))
			return CMD_UNKNOWN;

		char* tmp = (char *)malloc(end - buff - 3);
		memcpy(tmp, buff + 4, end - buff - 4);
		tmp[end - buff - 4] = '\0';

		int ret = CMD_UNKNOWN;
		if (strcmp(tmp, "/dump") == 0)
			ret = CMD_DUMP;
		else if (strcmp(tmp, "/fps") == 0)
			ret = CMD_FPS;

		free(tmp);
		return ret;
	}

	return CMD_CONTINUE;
}

void* TestinUIAutomator::threadFPSRun(void *r)
{
	TCDirector* director = TCDirector::funcDirectorGetInstance();
	int frames = director->getTotalFrames();
	long time = getCurrentTime();
	while (true)
	{
		usleep(1000 * FPS_INTERVAL);

		long t = getCurrentTime() - time;
		int f = director->getTotalFrames() - frames;
		frames = director->getTotalFrames();
		time = getCurrentTime();
		if (t <= 0 || f <= 0)
			continue;
		float fps = 1000.0f * f / t;

		pthread_mutex_lock(&sFPSMutex);
		if (mFPSCount < MAX_FPS_COUNT) {

			mFPSArray[mFPSCount] = fps;
			mFPSCount ++;
		} else {
			for (int i = 0; i < MAX_FPS_COUNT - 1; i++) {
				mFPSArray[i] = mFPSArray[i + 1];
			}
			mFPSArray[MAX_FPS_COUNT - 1] = fps;
		}
		pthread_mutex_unlock(&sFPSMutex);
	}

    return NULL;
}

long TestinUIAutomator::getCurrentTime()     
{      
#ifdef COCOS2D_CPP_2X
    struct cc_timeval tv;      
    CCTime::gettimeofdayCocos2d(&tv, NULL);   
#elif defined(COCOS2D_CPP_3X)
    struct timeval tv;      
    gettimeofday(&tv, NULL); 
#endif  
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;      
}  

NS_CC_END
