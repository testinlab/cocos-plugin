手游自动化测试插件
============

  Testin支持cocos引擎手游测试的插件集，支持cocos2d-x和cocos2d-js全版本游戏引擎，包括cocos2d-x 2.x、cocos2d-x 3.x、cocos2d-js 3.x。包含如下模块：

  - 手游自动化测试插件：通过dump游戏对象信息，实现游戏对象的控件化，以实现精准的手游自动化测试
  - TestinAgent SDK cocos插件：C++代码到Native代码（Java/ObjectiveC）的桥梁，方便在C++调用TestinAgent SDK，用于传递自定义用户信息、场景、错误等；实现对JavaScript、Lua脚本的错误收集


## 下载
-----------
  使用github工具，或者在命令行clone本项目到本地电脑
```bash
git clone https://github.com/testinlab/cocos-plugin.git
```
 
## 拷贝代码
-----------
  将下载内容中的autotest目录和testinagenthelper目录，复制到cocos2d-x或cocos2d-js引擎安装目录的external目录下；如果是使用了cocos2d-x或cocos2d-js 3.x版本引擎自动生成的项目，需要将autotest目录拷贝至项目的相应子目录中。

#### cocos2d-x 2.x版本
  将autotest目录和testinagenthelper目录复制到{cocos2d-x安装目录}/external/下

#### cocos2d-x 3.x版本
 - 使用项目目录中的引擎代码编译
  因为引擎的核心源码会拷贝至项目目录下，需要将源码同被编译的引擎源码放到一起，所以需要将autotest目录和testinagenthelper目录拷贝至 {项目目录}/cocos2d/external/目录下。如: /cocos2d-x-3.2/projects/myCocosProject/cocos2d/external/
 - 使用引擎安装目录中的代码编译
  将autotest目录和testinagenthelper目录拷贝至{cocos2d-x安装目录}/external/目录下

#### cocos2d-js 3.x版本
 - 使用项目目录中的引擎代码编译
  因为引擎的核心源码会拷贝至项目目录下，需要将同被编译的引擎源码放到一起，所以需要将autotest目录和testinagenthelper目录拷贝至 {项目目录}/frameworks/js-bindings/cocos2d-x/external/目录下。如: /cocos2d-js-3.0/projects/myCocosProject/frameworks/js-bindings/cocos2d-x/external/
 - 使用引擎安装目录中的代码编译
  将autotest目录和testinagenthelper目录拷贝至{cocos2d-js安装目录}/frameworks/js-bindings/cocos2d-x/external/目录下

## 添加代码支持
-----------
 - [手游自动化支持](autotest/README.md)
 - [TestinAgent SDK cocos插件支持](testinagenthelper/README.md)
