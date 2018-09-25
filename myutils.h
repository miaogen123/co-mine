/*************************************************************************
    > File Name: myutils.h
    > Author :miaogen
    > Mail: miaogen156@outlook.com 
    > Created Time: Wed 19 Sep 2018 04:03:20 PM CST
 ************************************************************************/
#include<string>
#include<netinet/in.h>
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
	SocketManager(std::string ipAddr="127.0.0.1", std::string port="53286");
	void setIpAddrPort(std::string ipaddr, std::string port="53286");
	bool connect(std::string ipAddr, std::string port="53286");
	int getSocketOpt(int level, int optname, void *optval, socklen_t *optlen) const;
	int setSokcetOpt(int level, int optname, void *optval, socklen_t *optlen);
	~SocketManager();
	int bindAndListenSocket(std::string ipaddr, std::string port="53286");
	int getListenfd() const ;
};


//TODO::设置一个用函数而非模板作为参数的inputUntilTrue
template<typename T>
void inputUntilTrue(T& tobeInput, std::string& msg, std::function<bool(T)> judgeFunc){
	std::cout<<"in the function"<<std::endl;
	while(!(std::cin>>tobeInput)||!judgeFunc(tobeInput)){
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		std::cout<<msg;
	}
}

template<typename T,  typename F>
void inputUntilTrue(T& tobeInput, std::string msg, F judgeFunc){
	std::cout<<"in the template"<<std::endl;
	while(!(std::cin>>tobeInput)||!judgeFunc(tobeInput)){
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		std::cout<<msg;
	}
}

