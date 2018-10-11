### 对战版扫雷：填了大一时埋下的坑

当时C语言大作业做的这个，开题的时候，我说过自己可以做一个对战版的，现在大三了，在学习网络编程，回过头把这个坑给填了。

***
- 程序主体:默认是使用53286作为端口

- **保证第一次不会点到雷**，但第一次排雷不能直接进行标记雷

- 不同的用户使用不同的光标以及颜色来表示, 默认绿色光标是自己操作的

- TODO::使用优雅的关闭socket的方式：5s以后关闭

***
####  程序框架图
![程序框架](https://github.com/miaogen123/co-mine/raw/master/程序框架.png)
***
#### 操作指南：

扫雷阶段的操作：**(大写字母)  A--点击，B-- 标记雷， JKLI分别对应左下右上移动**

#### 效果图：

##### 单机模式：
![单机模式](https://github.com/miaogen123/co-mine/raw/master/单机模式.gif)
##### 对战模式：

右侧的虚拟机里面设置了随机移动(为了展现效果)，同时由于gif图压掉了一些帧，看起来要比实际的快

![对战模式](https://github.com/miaogen123/co-mine/raw/master/对战模式.gif)

***

#### 暂未解决的问题

- 同步 : 也就是网络延迟的问题， 在上图对战模式的最后可以看到已经出现了不一致的位置，有了解决思路，还没有写，具体可以看这里 [co-mine_TODO](https://github.com/miaogen123/co-mine/projects/1)
- 大量重复发包会卡死：同样见这里[co-mine_TODO](https://github.com/miaogen123/co-mine/projects/1)