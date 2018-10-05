#include "Communicate.h"



Communicate::Communicate(int max_epoll):MAX_EVENT_NUMBER(max_epoll){
	//
	epollfd = epoll_create(5);
	events = new epoll_event[MAX_EVENT_NUMBER];
}

void Communicate::addfd(int fd, bool ET_enable){
	epoll_event event;
	event.data.fd = fd;
	if (ET_enable)
		event.events |= EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setNonBlock(fd);
}

void Communicate::addRWfd(int readfd, int writefd, bool ET_R_enable){
	writeTo = writefd;
	addfd(readfd, ET_R_enable);
}

void Communicate::process()
{
}


Communicate::~Communicate()
{
	delete[]events;
}

int Communicate::setNonBlock(int fd){
	int oldOption = fcntl(fd, F_GETFL);
	int newOption = oldOption | O_NONBLOCK;
	fcntl(fd, F_SETFL ,newOption);
	return oldOption;
}
