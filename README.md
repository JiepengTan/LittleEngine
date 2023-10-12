
# 1. Little Engine
该项目是作为学习使用的一个玩具级别游戏引擎

该项目是在 开源库 [Overload](https://github.com/adriengivry/Overload) 的基础上，开发而来，增加以下特性：

- 完善 Editor 界面
- c++ 端增加反射功能 参考自[Piccolo](https://github.com/BoomingTech/Piccolo)  
- 场景的序列化，Inspector面板都是基于反射来实现
- 目录比较大的重构


## 编译环境
1. Windows 10 x64
2. 需要安装 VS2022 ,  需要支持 .NET Framework 4.8  和 MSVC v143 C++ 开发
3. 打开 GameEngine.sln, 点击 Build/Build Solution ，编译整个工程 (注意第一次编译会有编译错误，再次编译即可)
4. 再次编译整个工程，这次会调用上一次编译的工具，生成目标代码，从而修正编译错误










