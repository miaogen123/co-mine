/*************************************************************************
    > File Name: myutils.cpp
    > Author :miaogen
    > Mail: miaogen156@outlook.com 
    > Created Time: Wed 19 Sep 2018 09:43:27 PM CST
 ************************************************************************/

#include<string>
#include<pthread.h>
#include<unistd.h>
#include<stdexcept>
#include<netinet/in.h>
#include<memory.h>
#include<string.h>
#include"myutils.h"
#include<arpa/inet.h>


int SocketManager::getSocketOpt(int level, int optname, void *optval, socklen_t *optlen) const{
	return true;

}
int SocketManager::setSokcetOpt(int level, int optname, void *optval, socklen_t *optlen){
	return true;

}


void inline SocketManager::setIpAddrPort(std::string ipaddr, std::string port_argu){
	ipAddr=ipaddr, port=port_argu;
}

bool SocketManager::connect(std::string ipAddr, std::string port){
	if(used==false){
		return -1;
	}

	return true;
}

int SocketManager::bindAndListenSocket(std::string ipaddr, std::string port_argu){
	if(used==true){
		return -1;
	}
	ipAddr=ipaddr, port=port_argu;
	bzero(&address, sizeof(address));
	address.sin_family=AF_INET;
	inet_pton(AF_INET, ipAddr.c_str() , &address.sin_addr);
	address.sin_port=htons(atoi(port.c_str()));
	listenfd=socket(PF_INET, SOCK_STREAM, 0);

	struct linger linger;
	linger.l_onoff = 1;
	//指定调用close后 5s回收资源
	linger.l_linger = 5;
	setsockopt(listenfd, SOL_SOCKET, SO_LINGER, (char *) &linger, sizeof(linger));

	if(listenfd<0)
		throw std::runtime_error("socket 创建失败\n");
	int ret=bind(listenfd, (struct sockaddr*)&address, sizeof(address));
	if(ret==-1)
		throw std::runtime_error("socket 绑定失败\n");
	ret=listen(listenfd, 5);
	if(ret==-1)
		throw std::runtime_error("socket 监听失败\n");
	return listenfd;
}
int SocketManager::getListenfd() const {
	return listenfd;
}


SocketManager::SocketManager(std::string ipAddr, std::string port):ipAddr(ipAddr), port(port){
	listenfd=-1;
}

SocketManager::~SocketManager(){
	if(listenfd>=0)
		close(listenfd);
}
