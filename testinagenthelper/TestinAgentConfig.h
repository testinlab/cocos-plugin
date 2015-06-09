#ifndef __TESTIN_AGENT_CONFIG_H__
#define __TESTIN_AGENT_CONFIG_H__

#include <string.h>

const char	*appKey		= "8c4baa226551a4dfe2c2339363e74227";
const char	*appChannel	= "测试";
const char	*userInfo	= "用户";
const bool	isDebug		= true;
const bool	lPer		= true; /* collect log or not */
const bool	aPer		= true; /* collect activity or not */
const bool	isNCh		= true; /* collect native crash or not */
const bool	isAPM		= true; /* collect apm data or not */
const bool	isCh		= true; /* collect Java Crash or not */
const bool	isEx		= true; /* collect exception or not */
const bool	isRWifi		= true; /* only report with wifi */
const bool	isRBack		= true; /* when the app in the background, whether report data or not */

#endif  /* __TESTIN_AGENT_CONFIG_H__ */
