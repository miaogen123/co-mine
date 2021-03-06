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
	auto retnum=createSocket();
	if (retnum == -1) {
		std::cout << "socket create failed errno: " << errno << std::endl;
	}
	if(::connect(socketfd,(struct sockaddr*)&address, sizeof(address))<0){
		std::cout<<"connect error"<<std::endl;
		this->~SocketManager();
		return -1;
	}
	return socketfd;
}

int SocketManager::bindAndListenSocket(std::string ipaddr, std::string port_argu){
	ipAddr=ipaddr, port=port_argu;
	auto retnum=createSocket();
	if(retnum==-1){
		std::cout << "socket create failed errno: " << errno << std::endl;
	}

	//指定调用close后 5s回收资源
	struct linger linger;
	linger.l_onoff = 1;
	linger.l_linger = 5;
	setsockopt(socketfd, SOL_SOCKET, SO_LINGER, (char *) &linger, sizeof(linger));
	set_tcp_keepAlive(3, 50, 3);

	if(socketfd<0)
		throw std::runtime_error("socket create failed\n");
	int ret=bind(socketfd, (struct sockaddr*)&address, sizeof(address));
	if(ret==-1)
		throw std::runtime_error("socket bind failed\n");
	ret=listen(socketfd, 5);
	if(ret==-1)
		throw std::runtime_error("socket listen failed\n");
	auto connfd=accept(socketfd, (struct sockaddr*)&address, (unsigned int*	)(&(address)));
	return connfd;
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
	int reuse = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
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
        perror("setsockopt start error ");   
        return -1;   
    }   
    //启用心跳机制开始到首次心跳侦测包发送之间的空闲时间   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPIDLE,(void *)&start,sizeof(start)) == -1)   
    {   
        perror("setsockopt set first start interval error");   
        return -1;   
    }   
    //两次心跳侦测包之间的间隔时间   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPINTVL,(void *)&interval,sizeof(interval)) == -1)   
    {   
        perror("setsockopt set interval error");   
        return -1;   
    }   
    //探测次数，即将几次探测失败判定为TCP断开   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPCNT,(void *)&count,sizeof(count)) == -1)   
    {   
        perror("setsockopt set retry count error");   
        return -1;   
    }   
    return 0;   
} 
