# pythonvm

## 项目介绍
自己动手写Python虚拟机

## 软件架构
软件架构说明


## 安装教程

### Linux
在根目录下执行
```
./build.sh
```
即可。

### Apple
在根目录下执行
```
./build.sh
```
即可。

### Windows系统
支持两种方式编译，一种是使用MinGW，一种是使用msvc。

使用 MinGW 进行编译，可以运行如下命令：
```
mkdir build
cd build
cmake -G "MinGW Makefiles" ../
mingw32-make
```

使用 msvc 则建议直接使用 cmake gui 工具和 vscode 打开。目前代表尚未支持。

## 使用说明

注意构建系统库和测试用例只能使用 Python3.8，否则就会报错。

1. 可执行程序位于 vm 目录下，名为 pvm
2. 系统库位于 lib 目录下。
3. 测试用例位于 test 目录下，经过编译以后都是 pyc 文件，可以通过
```
./vm/pvm test/alltest.pyc
```
这样的命令加载执行。

#### 参与贡献

1. Fork 本项目
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request


#### 码云特技

1. 使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2. 码云官方博客 [blog.gitee.com](https://blog.gitee.com)
3. 你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解码云上的优秀开源项目
4. [GVP](https://gitee.com/gvp) 全称是码云最有价值开源项目，是码云综合评定出的优秀开源项目
5. 码云官方提供的使用手册 [http://git.mydoc.io/](http://git.mydoc.io/)
6. 码云封面人物是一档用来展示码云会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
