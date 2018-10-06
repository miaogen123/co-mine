#pragma once
//����ʵ��
#include<memory>

#include"Communicate.h"
#include"Display.h"

#define  MAX_DIM 16           //����ɨ�׵�������
#define MIN_DIM  9           //������С��ɨ�׽���

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
	void Sub_MoveCursor(int row, int col, int flag);//flag���������� ��1ʱ��ʾ��� ��0ʱ�ػ���Ļ
	void Initialize(void);
	void InitializeMine(int mine[MAX_DIM][MAX_DIM]);
	bool judge(int x, int y);
	void writeTofd(int fd);
	void process();


private:
	//����ģʽֻ����һ��
	const int matrixDim;
	const int  MINE_VAL = 20;			//�׵���ֵ
	unsigned int userID;
	//һ��ָ�������ֽڣ��ַ�+userID
	const size_t commandSize = 2;

	MineGame(int dim);
	Display* toScreen;
	std::shared_ptr<Communicate> Com;

    int count=0;
	//λ��
    int row=1, col=1;
    char  state;
    int mineremain=0, true_mineremain=0;

};
