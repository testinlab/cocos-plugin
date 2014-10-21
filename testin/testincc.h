/****************************************************************************

Add testin ui automation support for cocos2d-x games



Copyright (c) 2014 Testin.cn

http://www.testin.cn

****************************************************************************/


#ifndef __TESTIN_CC_SUPPORT_H__
#define __TESTIN_CC_SUPPORT_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 *  启动Testin对脚本录制的支持，需要添加到程序初始化的地方
 *   
*/
extern bool StartTestinObserver();

/**
 *  标识游戏场景，用于区分不同的游戏场景界面，以便对游戏运行过程做更好的识别
 *  (目前只支持C++)
 *  
 *  建议游戏在每个CCScene初始化成功后，调用CCNode::setTag方法，对不同的CCScene
 *  设置不同的tagId，这样可以不需调用本方法
 *  
*/
extern void RegisterTestinCCScene(void* scene, const char* name);


#endif //__TESTIN_CC_SUPPORT_H__

