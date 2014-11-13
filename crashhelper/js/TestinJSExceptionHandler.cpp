#include <string.h>
#include "TestinJSExceptionHandler.h"
#include "../TestinCrashHelper.h"


void TestinJSExcetionHandler::registerJSExceptionHandler(JSContext *cx) {
	JS_SetErrorReporter(cx, TestinJSExcetionHandler::reportError);
}

void TestinJSExcetionHandler::reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
	const char* format = "%s:%u:%s\n";
	const char* fileName = report->filename ? report->filename : "<no filename=\"filename\">";

	int bufLen = strlen(format) + strlen(fileName) + strlen(message) + 16;
	char* traceback = (char*)malloc(bufLen);
	memset(traceback, 0, bufLen);
	sprintf(traceback, format, fileName, (unsigned int) report->lineno, message);
	TestinCrashHelper::reportException(EXCEPTION_TYPE_JS, message, traceback);
	free(traceback);
};






