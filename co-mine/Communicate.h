#pragma once
#include"globalConfig.h"
#include<fcntl.h>
#include<atomic>
#include<condition_variable>
#include<mutex>
#include<string>
#include<memory>
#include<array>
#include<sys/epoll.h>
#include<unistd.h> 

class Communicate
{
public:
	Communicate(int max_epoll=1024);
	void addfd(int fd, bool ET_enable=false); 
	//只调用一次
	void addRWfd(int readfd, int writefd, bool ET_R_enable = false); 
	void process();
	//block 
	std::string waitAndGetData(size_t count);
	//NONblock
	//TODO::待实现
	//std::string getData(size_t count);
	~Communicate();
private:
	const int MAX_EVENT_NUMBER = 1024;
	const unsigned int MAX_BUF = 10;

	std::condition_variable cv;
	std::mutex mt;
	//TODO::测试一下
	int start=0, end=0;
	char *buf;
	int setNonBlock(int fd);
	struct  epoll_event *events;
	//写的文件描述符是唯一的
	int writeTo;
	int epollfd;

};

