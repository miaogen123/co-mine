#pragma once
#include<fcntl.h>
#include<sys/epoll.h>
class Communicate
{
public:
	Communicate(int max_epoll);
	void addfd(int fd, bool ET_enable);
	//ֻ����һ��
	void addRWfd(int readfd,  int writefd, bool ET_R_enable=false);
	void process();
	~Communicate();
private:
	const int MAX_EVENT_NUMBER = 1024;
	const unsigned int MAX_BUF = 1024;

	int setNonBlock(int fd);
	struct  epoll_event *events;
	//д���ļ���������Ψһ��
	int writeTo;
	int epollfd;

};

