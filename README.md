cocos-plugin
============

  cocos手游录制插件，用于添加Testin手游自动化测试支持，支持cocos2d-x和cocos2d-js全版本游戏引擎，包括cocos2d-x 2.x、cocos2d-x 3.x、cocos2d-js 3.x。

## 下载
-----------
  使用github工具，或者在命令行clone本项目到本地电脑
```bash
git clone https://github.com/testinlab/cocos-plugin.git
```

## 拷贝代码
-----------
  将下载内容中的testin目录，复制到cocos2d-x或cocos2d-js引擎安装目录的external目录下；如果是使用了cocos2d-x或cocos2d-js 3.x版本引擎自动生成的项目，需要将testin目录拷贝至项目的相应子目录中。

#### cocos2d-x 2.x版本
  将testin目录复制到<cocos2d-x安装目录>/external/下

#### cocos2d-x 3.x版本
 - 使用项目目录中的引擎代码编译
  因为引擎的核心源码会拷贝至项目目录下，需要将testin的源码同被编译的引擎源码放到一起，所以需要将testin目录拷贝至 <项目目录>/cocos2d/external/目录下。如: /cocos2d-x-3.2/projects/myCocosProject/cocos2d/external/
 - 使用引擎安装目录中的代码编译
  将testin目录拷贝至cocos2d-x安装目录>/external/目录下

#### cocos2d-js 3.x版本
 - 使用项目目录中的引擎代码编译
  因为引擎的核心源码会拷贝至项目目录下，需要将testin的源码同被编译的引擎源码放到一起，所以需要将testin目录拷贝至 <项目目录>/frameworks/js-bindings/cocos2d-x/external/目录下。如: /cocos2d-js-3.0/projects/myCocosProject/frameworks/js-bindings/cocos2d-x/external/
 - 使用引擎安装目录中的代码编译
  将testin目录拷贝至cocos2d-x安装目录>/frameworks/js-bindings/cocos2d-x/external/目录下

## 添加代码支持
-----------
  在游戏初始化的代码中添加testin手游支持，并修改make文件

#### 添加头文件
```C++
#include "testin/testincc.h"
```

#### 增加代码行
  在初始化成功的代码位置（如AppDelegate.cpp的applicationDidFinishLaunching()函数的最后），添加如下代码行
```C++
StartTestinObserver();
```

#### 修改make文件
  修改proj.android/jni目录下的Android.mk文件（暂时只支持Android），增加如下两行
  
 - 这一行需要同其他的LOCAL_WHOLE_STATIC_LIBRARIES命令放在一起
```MK
LOCAL_WHOLE_STATIC_LIBRARIES += testin_cocos2dx_support_static
```
 - 这一行放在最后
```MK
$(call import-module,external/testin)
```
  
  现在可以编译你的游戏代码了，已经为你的游戏添加Testin手游自动化测试支持！
  

