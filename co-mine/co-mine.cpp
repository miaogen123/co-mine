//程序在linux下实现字符扫雷，独立完成
//这个程序之前是纯c写的，后来改成C++

#include<iostream>
#include<cstdio>
#include"myutils.h"
#include"MineGame.h"
#include"SocketManager.h"

using namespace std;

int main(void)
{
	ios::sync_with_stdio(false);
	printf("wel come to the game\n");
	printf("attention: this game use \"I K J L\" to move up or down or left or right\n");
	printf("use A to implement mouse left click,  use B to implement mouse right click\n");
	printf("首先请选择你要进行单机模式,还是对战模式\n");
	printf("1, 对战模式\n");
	printf("2, 单机模式\n");
	int mode=0;
	inputUntilTrue(mode, "请输入1或者2", [](decltype(mode) a) {return a == 1 || a == 2; });
	SocketManager sock;
	if(mode==1){
		//TODO::扫描端口,空闲使用
		//TODO::这里是否可以选择不用if-else 语句来实现
		std::string ipAddr, port;
		cout<<"请输入对方Ip(如果输入为1 ,则进行本地监听,等待对方连接)";
		inputUntilTrue(ipAddr, "输入判断失败,请重试|input error, try again", [](decltype(ipAddr) mode){return true;});
		if(ipAddr!="1"){
			cout<<"请输入对方端口";
			inputUntilTrue(port, "输入判断失败,请重试|input error, try again", [](decltype(ipAddr) port) { int iport = atoi(port.c_str()); return iport > 0 && iport < 65535; });
			auto fd=sock.connect(ipAddr, port);
		}else{
			auto fd=sock.bindAndListenSocket("127.0.0.1");
		}
	}

	//game
	int matrixDim = 9;
	printf("please input the dimension of the matrix(9~16)\n");
	inputUntilTrue(matrixDim, "input error , please iput again\n", [](decltype(matrixDim) a) {return a <= MAX_DIM&& a >= MIN_DIM; });
	MineGame *pMG=MineGame::getMineGame(matrixDim);
	pMG->run();

    return 0;
}
