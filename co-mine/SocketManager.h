#pragma once
#include<string>
#include<netinet/in.h>
#include"globalConfig.h"
#include <netinet/tcp.h> 
#include<limits>
#include<functional>
#include<iostream>

class SocketManager{
private:
	std::string ipAddr, port;
	struct sockaddr_in address;
	int socketfd;
	// true 表示connect使用, false 表示
	bool used;

	int createSocket();
public:	
	//自动使用connect
	int set_tcp_keepAlive( int start, int interval, int count);
	SocketManager(std::string ipAddr="127.0.0.1", std::string port="53286");
	void setIpAddrPort(std::string ipaddr, std::string port="53286");
	int connect(std::string ipAddr, std::string port="53286");
	int getSocketOpt(int level, int optname, void *optval, socklen_t *optlen) const;
	int setSokcetOpt(int level, int optname, void *optval, socklen_t *optlen);
	~SocketManager();
	int bindAndListenSocket(std::string ipaddr, std::string port="53286");
	int getListenfd() const ;
};