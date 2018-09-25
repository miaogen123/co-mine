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
#include<sys/types.h>
#include<sys/socket.h>


//TODO::应该给用户获取socket的选项的public 接口
int SocketManager::getSocketOpt(int level, int optname, void *optval, socklen_t *optlen) const{
	return true;

}
//TODO::应该给用户设置socket的public 接口
int SocketManager::setSokcetOpt(int level, int optname, void *optval, socklen_t *optlen){
	//setsockopt(socketfd, SOL_SOCKET, )
	return true;

}


void inline SocketManager::setIpAddrPort(std::string ipaddr, std::string port_argu){
	ipAddr=ipaddr, port=port_argu;
}

bool SocketManager::connect(std::string ipaddr, std::string port_argu){
	ipAddr=ipaddr, port=port_argu;
	if(createSocket()==-1){
		std::cout<<" this socket has been  used as listen socket "<<std::endl;
		return -1;
	}
	::connect(socketfd,(struct sockaddr*)&address, sizeof(address));
	return socketfd;
}

int SocketManager::bindAndListenSocket(std::string ipaddr, std::string port_argu){
	ipAddr=ipaddr, port=port_argu;
	if(createSocket()==-1){
		std::cout<<" this socket has been  used as connect socket "<<std::endl;
		return -1;
	}
	

	struct linger linger;
	linger.l_onoff = 1;
	//指定调用close后 5s回收资源
	linger.l_linger = 5;
	setsockopt(socketfd, SOL_SOCKET, SO_LINGER, (char *) &linger, sizeof(linger));

	if(socketfd<0)
		throw std::runtime_error("socket 创建失败\n");
	int ret=bind(socketfd, (struct sockaddr*)&address, sizeof(address));
	if(ret==-1)
		throw std::runtime_error("socket 绑定失败\n");
	ret=listen(socketfd, 5);
	if(ret==-1)
		throw std::runtime_error("socket 监听失败\n");
	return socketfd;
}
int SocketManager::getListenfd() const {
	return socketfd;
}

SocketManager::SocketManager(std::string ipAddr, std::string port):ipAddr(ipAddr), port(port){
	socketfd=-1;
}

SocketManager::~SocketManager(){
	if(socketfd>=0)
		close(socketfd);
}

int SocketManager::createSocket(){
	if(used==false){
		bzero(&address, sizeof(address));
		address.sin_family=AF_INET;
		inet_pton(AF_INET, ipAddr.c_str() , &address.sin_addr);
		address.sin_port=htons(atoi(port.c_str()));
		socketfd=socket(PF_INET, SOCK_STREAM, 0);
		used=true;
	}else{
		return -1;
	}
	return 1;
}
