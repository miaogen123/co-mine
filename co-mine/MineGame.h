#pragma once
//����ʵ��
#include<map>
#include<memory>
#include<iostream>
#include"globalConfig.h"
#include"UserStat.h"
#include"Communicate.h"
#include"Display.h"

using std::endl;
using std::cout;

#define  MAX_DIM 16           //����ɨ�׵�������
#define MIN_DIM  9           //������С��ɨ�׽���

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
	void Sub_MoveCursor(int row, int col, int flag);//flag���������� ��1ʱ��ʾ��� ��0ʱ�ػ���Ļ
	void Initialize(void);
	void InitializeMine();
	bool judge(int x, int y);
	void writeTofd(int fd);
	void process();
	void judgeWinOrFail( std::map<unsigned char, std::shared_ptr<UserStat>> &user);
	FontColor assignColor(FontColor fc=FontColor::BLACK);


private:

	//����ģʽֻ����һ��
	const int matrixDim;
	const int  MINE_VAL = 20;			//�׵���ֵ

	//�û���ID��user�����ӳ��
	std::map<unsigned char, std::shared_ptr<UserStat>>  users;
	std::shared_ptr<UserStat> self_user;
	unsigned char userID;

	//һ��ָ�������ֽڣ��ַ�+userID
	const size_t commandSize = 2;
	int mine[MAX_DIM][MAX_DIM];
	int flag[MAX_DIM][MAX_DIM];

	MineGame(int dim);

    int count=0;
	//λ��
    //int row=1, col=1;
    char  state;
    int mineremain=0, true_mineremain=0;
};
