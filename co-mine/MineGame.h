#pragma once
//单例实现
#include<map>
#include<memory>
#include<iostream>
#include"globalConfig.h"
#include"UserStat.h"
#include"Communicate.h"
#include"Display.h"

using std::endl;
using std::cout;

#define  MAX_DIM 16           //定义扫雷的最大阶数
#define MIN_DIM  9           //定义最小的扫雷阶数

class MineGame
{
public:
	std::shared_ptr<Communicate> Com;
	unsigned char currentUserId;

	~MineGame();
	static MineGame* getMineGame(int dim);
	int scanKeyboard(); 
	void run();
	void DisplayCursor(int row, int col);
	void YouWin(void);
	void Set_Blank(int row, int col);
	void BomMine();
	void Sub_MoveCursor(int row, int col, int flag);//flag的作用在于 是1时显示光标 是0时重画屏幕
	void Initialize(void);
	void InitializeMine();
	bool judge(int x, int y);
	void writeTofd(int fd);
	void process();
	void judgeWinOrFail( std::map<unsigned char, std::shared_ptr<UserStat>> &user);
	FontColor assignColor(FontColor fc=FontColor::BLACK);
	void setSeedToGenerateMatrix(time_t seed);


private:

	//单例模式只调用一次
	const int matrixDim;
	const int  MINE_VAL = 20;			//雷的数值

	//用户的ID和user对象的映射
	std::map<unsigned char, std::shared_ptr<UserStat>>  users;
	std::shared_ptr<UserStat> self_user;
	unsigned char userID;

	//一条指令两个字节：字符+userID
	const size_t commandSize = 2;
	time_t seedToGenerateMatrix;
	int mine[MAX_DIM][MAX_DIM] = { 0 };
	int flag[MAX_DIM][MAX_DIM];

	MineGame(int dim);

    int count=0;
	//位置
    //int row=1, col=1;
    char  state;
    int mineremain=0, true_mineremain=0;
};
