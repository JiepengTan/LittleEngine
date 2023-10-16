
# Little Engine
该项目是练习游戏引擎 ，尝试较大规模重构和改进引擎 

## 声明
该项目是在 开源库 [Overload](https://github.com/adriengivry/Overload) 的基础上，开发而来，增加以下特性：

- 完善 Editor 界面，朝Unity靠拢
- 增加动画 & 阴影 
- c++ 端增加反射功能
	- Gameplay 生命期完善，修正一些内存泄露，越界等问题
	- 场景的序列化，Inspector等界面 重新基于反射来实现，更加简洁
	- 得益于反射， 能动态添加新的Component类型，并获取对应的属性，或调用函数
	- Inspector 界面，支持 类似Unity Odin插件 那种 对函数增加tag ，自动添加按钮界面的功能

## 效果图
1. 动画和阴影
<p align="center"><img src="https://github.com/JiepengTan/JiepengTan.github.io/blob/master/assets/img/blog/LittleEngine/animation.gif?raw=true" width="768"></p> 

2. 反射-界面和函数调用
<p align="center"><img src="https://github.com/JiepengTan/JiepengTan.github.io/blob/master/assets/img/blog/LittleEngine/reflection.gif?raw=true" width="768"></p> 

3. 反射-Enum 支持，序列化
<p align="center"><img src="https://github.com/JiepengTan/JiepengTan.github.io/blob/master/assets/img/blog/LittleEngine/reflection_add_component.gif?raw=true" width="768"></p> 

## 编译和运行
1. Windows 10 x64
2. 需要安装 VS2022 ,  需要支持 .NET Framework 4.8  和 MSVC v143 C++ 开发
3. 打开 GameEngine.sln, 点击 Build/Build Solution ，编译整个工程 (注意第一次编译会有编译错误，没关系，再次编译即可修正)
4. 再次编译整个工程，这次会调用上一次编译的工具，生成目标代码，从而修正编译错误


