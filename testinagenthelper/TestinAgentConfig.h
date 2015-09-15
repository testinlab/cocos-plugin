#ifndef __TESTIN_AGENT_CONFIG_H__
#define __TESTIN_AGENT_CONFIG_H__

#include <string.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <Foundation/Foundation.h>
#endif


const char	*appKey		= "8c4baa226551a4dfe2c2339363e74227";
const char	*appChannel	= "测试";
const char	*userInfo	= "用户";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
/*Android part*/
const bool	isDebug		= true;
const bool	lPer		= true; /* collect log or not */
const bool	aPer		= true; /* collect activity or not */
const bool	isNCh		= true; /* collect native crash or not */
const bool	isCh		= true; /* collect Java Crash or not */
const bool	isEx		= true; /* collect exception or not */
const bool	isRWifi		= true; /* only report with wifi */
const bool	isRBack		= true; /* when the app in the background, whether report data or not */

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
/*iOS part*/

// Exception monitor enabled, default true.
const bool enabledMonitorException = true;

// use user location info, default false.
const bool useLocationInfo = false;

// use https, default true.
const bool useSecurityHttpConnection = true;

// only wifi report data, default true.
const bool reportOnlyWIFI = true;

// set upload log level, default 0 nothing.
const NSInteger uploadLogLevel = 0;

// print log in simulator for debug, default false.
const bool printLogForDebug = false;
#endif


#endif  /* __TESTIN_AGENT_CONFIG_H__ */
