### 对战版扫雷：是填了大一时埋下的坑

当时C语言大作业做的这个，开题的时候，我说过自己可以做一个对战版的，现在大三了，在学习网络编程，回过头把这个坑给填了。

***
- 程序主体:默认是使用53286作为端口
- myutils:对于socket的关闭,5s后回收资源 
- **保证第一次不会点到雷**，但第一次排雷不能直接进行标记
- 不同的用户使用不同的光标来表示
- TODO::使用优雅的关闭socket的方式：5s以后关闭
***
####  程序框架图
![程序框架](https://github.com/miaogen123/co-mine/raw/master/程序框架.png)
***
#### 效果图：

##### 单机模式：
![单机模式](https://github.com/miaogen123/co-mine/raw/master/单机模式.gif)
##### 对战模式：
![对战模式](https://github.com/miaogen123/co-mine/raw/master/对战模式.gif)
