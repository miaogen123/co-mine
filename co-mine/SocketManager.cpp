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



//TODO::Ӧ�ø��û���ȡsocket��ѡ���public �ӿ�
int SocketManager::getSocketOpt(int level, int optname, void *optval, socklen_t *optlen) const{
	return true;

}
//TODO::Ӧ�ø��û�����socket��public �ӿ�
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

	//ָ������close�� 5s������Դ
	struct linger linger;
	linger.l_onoff = 1;
	linger.l_linger = 5;
	setsockopt(socketfd, SOL_SOCKET, SO_LINGER, (char *) &linger, sizeof(linger));

	if(socketfd<0)
		throw std::runtime_error("socket ����ʧ��\n");
	int ret=bind(socketfd, (struct sockaddr*)&address, sizeof(address));
	if(ret==-1)
		throw std::runtime_error("socket ��ʧ��\n");
	ret=listen(socketfd, 5);
	if(ret==-1)
		throw std::runtime_error("socket ����ʧ��\n");
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
		throw std::runtime_error("socket ����ʧ��,exit....");
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

//fd:��������������
//start:�״�������������֮��Ŀ���ʱ��  
 //interval:������������֮��ļ��ʱ�� 
//count:̽���������������̽��ʧ���ж�ΪTCP�Ͽ�
int SocketManager::set_tcp_keepAlive( int start, int interval, int count)   
{   
    int keepAlive = 1;   
    if (socketfd < 0 || start < 0 || interval < 0 || count < 0) return -1;   //��ڲ������ ����̵ĺ�ϰ�ߡ�
    //�����������ƣ��������رգ���keepAlive���㼴��   
    if(setsockopt(socketfd,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive)) == -1){   
        perror("setsockopt");   
        return -1;   
    }   
    //�����������ƿ�ʼ���״�������������֮��Ŀ���ʱ��   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPIDLE,(void *)&start,sizeof(start)) == -1)   
    {   
        perror("setsockopt");   
        return -1;   
    }   
    //������������֮��ļ��ʱ��   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPINTVL,(void *)&interval,sizeof(interval)) == -1)   
    {   
        perror("setsockopt");   
        return -1;   
    }   
    //̽���������������̽��ʧ���ж�ΪTCP�Ͽ�   
    if(setsockopt(socketfd,SOL_TCP,TCP_KEEPCNT,(void *)&count,sizeof(count)) == -1)   
    {   
        perror("setsockopt");   
        return -1;   
    }   
    return 0;   
} 
