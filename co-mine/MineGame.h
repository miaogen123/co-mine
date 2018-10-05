#pragma once
//单例实现
#include<memory>

#include"Communicate.h"
#include"Display.h"

#define  MAX_DIM 16           //定义扫雷的最大阶数
#define MIN_DIM  9           //定义最小的扫雷阶数

class MineGame
{
public:
	~MineGame();
	static MineGame* getMineGame(int dim);
	int scanKeyboard(); 
	void run();
	void DisplayCursor(int mine[MAX_DIM][MAX_DIM], int flag[MAX_DIM][MAX_DIM], int row, int col);
	void YouWin(void);
	void Set_Blank(int row, int col, int mine[MAX_DIM][MAX_DIM], int flag[MAX_DIM][MAX_DIM]);
	void BomMine(int mine[MAX_DIM][MAX_DIM]);
	void Sub_MoveCursor(int row, int col, int flag);//flag的作用在于 是1时显示光标 是0时重画屏幕
	void Initialize(void);
	void InitializeMine(int mine[MAX_DIM][MAX_DIM]);
	bool judge(int x, int y);


private:
	//单例模式只调用一次
	const int matrixDim;
	MineGame(int dim);

	Display* toScreen;
	std::shared_ptr<Communicate> Com;

    int count=0;
	//位置
    int row=1, col=1;
    char  state;
    int mineremain=0, true_mineremain=0;

	const int  MINE_VAL = 20;			//雷的数值
};

//静态变量提前声明
//MineGame* MineGame::uni_instance = nullptr;

