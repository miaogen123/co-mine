//程序在linux下实现字符扫雷，独立完成
//这个程序之前是纯c写的，后来改成C++

#include<iostream>
#include<cstdio>
#include"myutils.h"
#include<stdexcept>
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
	//mode = 2;
	int fd = -1;

	SocketManager sock;
	time_t seedToGenerateMatrix;
	int matrixDim = -1;
	if(mode==1){
		//TODO::扫描端口,空闲使用
		//TODO::这里是否可以选择不用if-else 语句来实现
		std::string ipAddr, port;
		cout<<"请输入对方Ip(如果输入为1 ,则进行本地监听,等待对方连接)";
		inputUntilTrue(ipAddr, "输入判断失败,请重试|input error, try again", [](decltype(ipAddr) mode){return true;});
		if(ipAddr!="1"){
			cout<<"请输入对方端口";
			inputUntilTrue(port, "输入判断失败,请重试|input error, try again", [](decltype(ipAddr) port) { int iport = atoi(port.c_str()); return iport > 0 && iport < 65535; });

			fd=sock.connect(ipAddr, port);
			//NOTE:100bytes的大小应该够了
			const int syncMessageMaxSize = 100;
			char smessage[syncMessageMaxSize];
			cout << "waiting the command from the room host" << endl;
			int Rcount= recv(fd, smessage, syncMessageMaxSize, 0);
			if (Rcount < 0) {
				throw std::runtime_error("can't connect to the dest,can't get the matrix dim");
				exit(-1);
			}
			auto dim=std::string("");
			try {
					//std::cout << "size" << smessage.size() << endl;
				for (int i = 0; i < Rcount; i++) {
					std::cout << "processing " << i << endl;
					if (smessage[i] == '\0') {
						dim = std::string(smessage, i + 1);
						//dim = std::string(smessage, 0, i+1);
						auto seed = std::string(smessage+i+1 , Rcount - i - 1);
						seedToGenerateMatrix = std::atol(seed.c_str());
						break;

					}
				}
				matrixDim = std::atoi(dim.c_str());
			}
			//TODO::修复一下这里的异常处理
			catch (std::exception &e){
				std::cout << e.what() << endl;
				exit(-1);
			}
#ifdef DEBUG_
			for (auto i = 0; i < Rcount;i++) {
				cout << smessage[i];
			}
			std::cout << endl;
#endif // DEBUG_
		}else{
			fd=sock.bindAndListenSocket("127.0.0.1");
			printf("please input the dimension of the matrix(9~16)\n");
			inputUntilTrue(matrixDim, "input error , please iput again\n", [](decltype(matrixDim) a) {return a <= MAX_DIM&& a >= MIN_DIM; });

			auto messageToSync1 = std::to_string(matrixDim);
			//NOTE::这里种子分发，在cs结构下应该由服务器
			seedToGenerateMatrix = time(0);
			auto messageToSync2 = std::to_string(seedToGenerateMatrix);
			auto messageToSync = messageToSync1 + std::string(1, '\0') + messageToSync2;
			//TODO::这里最好处理一下，保证在能够拿到数据的情况下
			//TODO::传递一个相同的种子
			int dim = messageToSync.size();
			send(fd, messageToSync.c_str(),dim , 0);

#ifdef DEBUG_
			for (auto val : messageToSync) {
				cout << val;
			}
			std::cout << endl;
#endif // DEBUG_

		}
	}
	else {
		printf("please input the dimension of the matrix(9~16)\n");
		inputUntilTrue(matrixDim, "input error , please iput again\n", [](decltype(matrixDim) a) {return a <= MAX_DIM&& a >= MIN_DIM; });
		//matrixDim = 12;
	}
	std::cout << "the seed " << std::to_string(seedToGenerateMatrix) << std::endl;
	MineGame *pMG=MineGame::getMineGame(matrixDim);
	pMG->Com->addRWfd(fd, fd);
	pMG->setSeedToGenerateMatrix(seedToGenerateMatrix); 
	pMG->run();
    return 0;
}
