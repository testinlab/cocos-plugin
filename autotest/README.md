Cocos手游自动化支持
============

  cocos手游录制插件，通过dump游戏对象信息，实现游戏对象的控件化，以实现精准的手游自动化测试

## 支持
-----------
  cocos手游录制插件，只支持cocos游戏引擎中提供的CCNode（Node）对象以及继承CCNode（Node）类的对象。

## 如何添加代码支持
-----------
  在游戏初始化的代码中添加testin手游支持，并修改make文件

#### 添加头文件
```C++
#include "autotest/testincc.h"
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
LOCAL_WHOLE_STATIC_LIBRARIES += testin_cocos2dx_autotest_static
```
 - 这一行放在最后
```MK
$(call import-module,external/autotest)
```
  
  现在可以编译你的游戏代码了，已经为你的游戏添加Testin手游自动化测试支持！
  
## 常见问题
 - 编译时找不到头文件"autotest/testincc.h"

  在make文件中添加头文件查找路径，将存放autotest目录的位置(external目录)添加到LOCAL_C_INCLUDES变量中，例如
```MK
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../cocos2d-x.2.2.3 \
    $(LOCAL_PATH)/../../cocos2d-x.2.2.3/cocos2dx \
    $(LOCAL_PATH)/../../cocos2d-x.2.2.3/external
```

 - 找不到头文件 "extensions/GUI/CCControlExtension/CCScale9Sprite.h"

  很有可能你使用的引擎为cocos2d-x 3.3beta0版本，此版本对Scale9Sprite的支持有问题。如果确认在你的工程中没有使用Scale9Sprite，那么可以找到cocos_inc.h，修改如下行，将TESTIN_USING_CC_EXTENSION的定义值改为0；或者在相应的Android.mk文件中手工添加Scale9Sprite支持。

```C++
#define TESTIN_USING_CC_EXTENSION 0
```

 - ld错误：error: undefined reference to 'typeinfo for cocos2d::extension::Scale9Sprite'

  这是由于您的工程中没有使用extensions模块，可以使用以下任意一种方法修改

  a) 找到cocos_inc.h，将TESTIN_USING_CC_EXTENSION的定义值改为0

  b) 或者，在编译时添加cocos extension模块
```MK
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
```

```MK
$(call import-module,extensions)

