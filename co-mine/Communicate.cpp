#include "Communicate.h"
#include <sys/types.h>
#include<iostream>	
#include<stdexcept>
#include<string.h>
#include <sys/socket.h>


Communicate::Communicate(int max_epoll):MAX_EVENT_NUMBER(max_epoll){
	//
	epollfd = epoll_create(5);
	events = new epoll_event[MAX_EVENT_NUMBER];
	buf=new char[MAX_BUF];
}

void Communicate::addfd(int fd, bool ET_enable){
		epoll_event event;
		event.data.fd = fd;
		if (ET_enable)
			event.events |= EPOLLET;
		event.events |= EPOLLIN;
		epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
		setNonBlock(fd);
	}

void Communicate::addRWfd(int readfd, int writefd, bool ET_R_enable)
{
		writeTo = writefd;
		addfd(readfd, ET_R_enable);
}

void Communicate::process()
{
	//lock_u.unlock();
	char innerBuf[MAX_BUF];
	while (true) {
		int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);

		for (auto i = 0; i < ret; i++) {
			int fd = events[i].data.fd;
			//数据的源FD与writeTo不相等，则是来自内部的数据，需要转发
			int Rcount = 0;
			std::unique_lock<std::mutex>  lock_u(mt);
			if (fd != writeTo && events[i].events&EPOLLIN) {
					Rcount = read(fd,  innerBuf,MAX_BUF);
					cv.wait(lock_u, [this, Rcount]() {
						auto remain=MAX_BUF-static_cast<int>((this->end + MAX_BUF - this->start) % MAX_BUF) ;
						return remain>Rcount;
					});
				send(writeTo, innerBuf, Rcount, 0);
			}
			else if (fd == writeTo && events[i].events&EPOLLIN) {
					Rcount = recv(fd,  innerBuf,MAX_BUF, 0);
						cv.wait(lock_u, [this, Rcount]() {
						auto remain=MAX_BUF-static_cast<int>((this->end + MAX_BUF - this->start) % MAX_BUF) ;
						//std::cout << remain<< std::endl;
						return remain>Rcount;
					});
					std::cout <<"socket"<< innerBuf[0]<< "\t"<<(int)innerBuf[1]<<std::endl;
			}
			int count = 0;
			while (count < Rcount) {
				buf [end]= innerBuf[count];
				end = ( end + 1) % MAX_BUF;
				count++;
			}
			//end = (end + Rcount) % MAX_BUF;
			try{
				lock_u.unlock();
			}
			catch (std::system_error &e) {
				std::cout << e.what() << std::endl;
			}
			memset(innerBuf, 0, MAX_BUF);
			cv.notify_one();
		}
	}
}

std::string Communicate::waitAndGetData(size_t count)
{	
	std::unique_lock<std::mutex> lock_u(mt);

	cv.wait(lock_u, [this]() {return this->start != this->end; });
	auto retS = std::string();
	if (start < end) {
		retS = std::string(buf + start, end-start);
	}
	else {
		auto retS1 = std::string(buf + start, MAX_BUF-start);
		auto retS2 = std::string(buf , end);
		retS = retS1 + retS2;
	}
	start = end;
	return retS;
}


Communicate::~Communicate()
{
	delete []events;
	delete []buf;
}

int Communicate::setNonBlock(int fd){
	int oldOption = fcntl(fd, F_GETFL);
	int newOption = oldOption | O_NONBLOCK;
	fcntl(fd, F_SETFL ,newOption);
	return oldOption;
}
