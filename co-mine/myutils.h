#pragma once
#include<string>
#include<netinet/in.h>
#include <netinet/tcp.h> 
#include"globalConfig.h"
#include<limits>
#include<functional>
#include<iostream>


//一个用函数而非模板作为参数的inputUntilTrue
template<typename T>
void inputUntilTrue(T& tobeInput, std::string& msg, std::function<bool(T)> judgeFunc){
	//std::cout<<"in the function"<<std::endl;
	while(!(std::cin>>tobeInput)||!judgeFunc(tobeInput)){
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		std::cout<<msg;
	}
}

template<typename T,  typename F>
void inputUntilTrue(T& tobeInput, std::string msg, F judgeFunc){
	//std::cout<<"in the template"<<std::endl;
	while(!(std::cin>>tobeInput)||!judgeFunc(tobeInput)){
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		std::cout<<msg;
	}
}
