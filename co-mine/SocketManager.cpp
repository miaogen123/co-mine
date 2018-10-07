#include"SocketManager.h"

#include<string>
#include<stdexcept>
#include<pthread.h>
#include <netinet/tcp.h> 
#include<unistd.h>
#include<stdexcept>
#include<memory.h>
#include<string.h>
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


void  SocketManager::setIpAddrPort(std::string ipaddr, std::string port_argu){
	ipAddr=ipaddr, port=port_argu;
}

int SocketManager::connect(std::string ipaddr, std::string port_argu){
	ipAddr=ipaddr, port=port_argu;
	if(::connect(socketfd,(struct sockaddr*)&address, sizeof(address))<0){
		std::cout<<"connect error"<<std::endl;
		this->~SocketManager();
		return -1;
	}
	//while(true){
	//	char buf[10];
	//	memset(buf, 0,10*sizeof(int));
	//	recv(socketfd, buf, 10, MSG_DONTWAIT);
	//	std::cout<<buf<<std::endl;
	//	send(socketfd, "quuuuu", 7, MSG_NOSIGNAL);
	//}
	return socketfd;
}

int SocketManager::bindAndListenSocket(std::string ipaddr, std::string port_argu){
	ipAddr=ipaddr, port=port_argu;

	//指定调用close后 5s回收资源
	struct linger linger;
	linger.l_onoff = 1;
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
	auto connfd=accept(socketfd, (struct sockaddr*)&address, (unsigned int*	)(&(address)));
	//while(true){
	//	send(connfd, "quuuuu", 7, MSG_NOSIGNAL);
	//	char buf[10];
	//	recv(connfd, buf, 10, MSG_DONTWAIT);
	//	std::cout<<buf<<std::endl;
	//}
	return connfd;
}
int SocketManager::getListenfd() const {
	return socketfd;
}

SocketManager::SocketManager(std::string ipAddr, std::string port):ipAddr(ipAddr), port(port){
	socketfd=-1;
	if(createSocket()==-1){
		std::cout<<" this socket has been  used as connect socket "<<std::endl;
		throw std::runtime_error("socket 创建失败,exit....");
	}
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

//fd:网络连接描述符
//start:首次心跳侦测包发送之间的空闲时间  
 //interval:两次心跳侦测包之间的间隔时间 
//count:探测次数，即将几次探测失败判定为TCP断开
int SocketManager::set_tcp_keepAlive( int start, int interval, int count)   
{   
    int keepAlive = 1;   
    if (socketfd < 0 || start < 0 || interval < 0 || count < 0) return -1;   //入口参数检查 ，编程的好习惯。
    //启用心跳机制，如果您想关闭，将keepAlive置零即可   
    if(setsockopt(socketfd,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive)) == -1){   
        perror("setsockopt");   
        return -1;   
    }   
    //启用心跳机制开始到首次心跳侦测包发送之间的空闲时间   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPIDLE,(void *)&start,sizeof(start)) == -1)   
    {   
        perror("setsockopt");   
        return -1;   
    }   
    //两次心跳侦测包之间的间隔时间   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPINTVL,(void *)&interval,sizeof(interval)) == -1)   
    {   
        perror("setsockopt");   
        return -1;   
    }   
    //探测次数，即将几次探测失败判定为TCP断开   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPCNT,(void *)&count,sizeof(count)) == -1)   
    {   
        perror("setsockopt");   
        return -1;   
    }   
    return 0;   
} 
