//程序在linux下实现字符扫雷，独立完成
//这个程序之前是纯c写的，后来改成C++
#include<iostream>
#include<functional>
#include<limits>
#include<pthread.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<termio.h>
#include <sys/ioctl.h>
#include<curses.h>
#include<pthread.h>
#include<stdlib.h>
#include<exception>
#include"myutils.h"

using namespace std;

#define TRUE 1
#define FALSE 0
#define MATRIX 16           //定义扫雷的最大阶数
#define MINIMUN 9           //定义最小的扫雷阶数
#define MINE_VAL 20			//雷的数值
//判断（x，y）是否在矩阵内部
#define JUDGE(x,y)  ((x)>-1&&(x)<MATRIX&&(y)>-1&&(y)<MATRIX?TRUE:FALSE)
int matrix;  //全局变量用来记录用户的输入
#define JUDGEMINE(x, y) ((mine[x][y])==MINE_VAL?'A':mine[x][y])
// 清除屏幕
#define CLEAR() printf("\033[2J")
// 上移光标
#define MOVEUP(x) printf("\033[%dA", (x))
// 下移光标
#define MOVEDOWN(x) printf("\033[%dB", (x))
// 左移光标
#define MOVELEFT(y) printf("\033[%dD", (y))
// 右移光标
#define MOVERIGHT(y) printf("\033[%dC",(y))
// 光标复位
#define RESET_CURSOR() printf("\033[H")
//xianshi 
#define SHOW_CURSOR() printf("\033[?25h")
//用于操作光标， 无需键入回车即可    参数：空  返回值：返回输入的键的 acsii码

int scanKeyboard()
{
	int in;
	struct termios new_settings;
	struct termios stored_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(0,&stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	in = getchar();
	tcsetattr(0,ICANON,&stored_settings);
	return in;
}
//初始化，完成布雷 参数 返回值 空
void InitializeMine(int mine[MATRIX][MATRIX])
{
    //下面的变量分别用来记录 行阶 列阶 应有雷的数量 产生的随机数 实际的雷数目
    int countone=0, countsec=0,amount=0,randnum=0, minenum=0;
    amount=(matrix-9)*5+10;
    while(minenum!=amount)          //直到生成了足够的雷后停止循环
    {
        for(countone=0;countone<matrix;countone++)
        {
            for(countsec=0;countsec<matrix;countsec++)
            {
                randnum=rand()%8+19;//MINE_VAL有雷
                if(mine[countone][countsec]==MINE_VAL)
                {
                  continue;
                }
                else if(randnum==MINE_VAL )
                {
                    if(minenum==amount)
                    {
                        mine[countone][countsec]=randnum-1;
                    }
                    else
                    {
                        mine[countone][countsec]=randnum;
                        minenum++;
                    }
                }
                else
                {
                  mine[countone][countsec] = randnum;
                }
              }
          }
    }
	//进行循环遍历，查找出对应雷区四周的雷的数目
	for (countone = 0; countone<matrix; countone++)
	{
		for (countsec = 0; countsec<matrix; countsec++)
		{
			minenum = 0;
			if (mine[countone][countsec] != MINE_VAL)         //通过判断找出中心格周围的雷数
			{
				if (JUDGE(countone - 1, countsec - 1))
				{
					if (mine[countone-1][countsec-1] == MINE_VAL)
						minenum++;
				}
				if (JUDGE(countone - 1, countsec))
				{
					if (mine[countone - 1][countsec] == MINE_VAL)
						minenum++;
				}
				if (JUDGE(countone - 1, countsec + 1) )
				{
					if (mine[countone - 1][countsec + 1] == MINE_VAL)
						minenum++;
				}
				if (JUDGE(countone, countsec - 1))
				{
					if (mine[countone][countsec - 1]==MINE_VAL)
						minenum++;
				}
				if (JUDGE(countone, countsec + 1))
				{
					if (mine[countone][countsec + 1] == MINE_VAL)
						minenum++;
				}
				if (JUDGE(countone + 1, countsec - 1))
				{
					if (mine[countone + 1][countsec - 1] == MINE_VAL)
						minenum++;
				}
				if (JUDGE(countone + 1, countsec))
				{
					if (mine[countone + 1][countsec] == MINE_VAL)
						minenum++;
				}
				if (JUDGE(countone + 1, countsec + 1))
				{
					if (mine[countone + 1][countsec + 1] == MINE_VAL)
						minenum++;
				}
				mine[countone][countsec] = minenum;         //中心格上记录周围的雷数
			}
		}
	}
}
//FUNC：显示初始化界面 参数:无 返回值：无
void Initialize(void)
{
  int countone=0, countsec=0;
  for(countone=0;countone<matrix;countone++)
    {
        for(countsec=0;countsec<matrix;countsec++)
        {
              printf("#   ");
        }
        if(countsec==matrix)
        {
            printf("%d", countone+1);
            printf("\n");
        }
    }
    for(countone=1;countone<=matrix;countone++)
    {
      if(countone<10)
      {
              printf("%d   ", countone);
      }
      else
      {
        printf("%d  ", countone);
      }
    }
    printf("\n");
}
//光标移动函数  参数：行列 返回值：空
void MoveCursor(int row ,int col) {
  RESET_CURSOR();
  if(row==1)
    {
      if(col==1)
      {
        return ;
      }
      else
      {
        MOVERIGHT(4*(col-1));
      }
    }
    else
    {
      MOVEDOWN(row-1);
      if(col==1)
      {
        return ;
      }
      else
      {
        MOVERIGHT(4*(col-1));
      }
    }
}
//显示光标   参数：屏幕上的行列  以及标志 1或0 返回值：空
void Sub_MoveCursor(int row ,int col, int flag)//flag的作用在于 是1时显示光标 是0时重画屏幕
{
  RESET_CURSOR();
  if(row==1)
    {
      if(col==1)
      {
		  MOVERIGHT(1);
      }
      else
      {
        MOVERIGHT(4*(col-1)+1);
      }
    }
    else
    {
      MOVEDOWN(row-1);
      if(col==1)
      {
		  MOVERIGHT(1);
      }
      else
      {
        MOVERIGHT(4*(col-1)+1);
      }
    }
	if(flag==1)
	{
		printf("\033[44;34;1m|\033[40;30;0m" );
	}
	else if(flag==0)
	{
		printf(" ");
	}
}
//雷被引爆 显示所有的雷 并输出爆炸特效  参数：无 返回：空
void BomMine(int mine[MATRIX][MATRIX])
{
      struct winsize ws;
      ioctl(0, TIOCGWINSZ, &ws);
      int countone=0, countsec=0;
      for(countone=0;countone<matrix;countone++)
      {
        for(countsec=0;countsec<matrix;countsec++)
        {
              if(mine[countone][countsec]==MINE_VAL)
              {
                  MoveCursor(countone+1, countsec+1);
                  printf("\033[40;31;1mMN\033[40;30;0m" );
              }
          }
      }
      fflush(stdout);
      sleep(3);
      RESET_CURSOR();
      for(countone=0;countone<matrix+10;countone++)
      {
          for(countsec=0;countsec<(int)ws.ws_col;++countsec)
          {
            printf(" ");
          }
          printf("\n");
          usleep(40000);
      }
      sleep(1);
      CLEAR();
      RESET_CURSOR();
      printf("       ***********            ***********                   \n");
      printf("     ***********            ***********                    \n");
      printf("    ****                  ****                          \n");
      printf("   ***                   ***                           \n");
      printf("   ***       **           ***       **                  \n");
      printf("    ****    ****            ****    ****                   \n");
      printf("     ******   ***             ******   ***                 \n");
      exit(1);
}
//点击后找出所有的周围无雷的格子并显示为空   参数：行，列，雷的数组， 标志数组
void Set_Blank(int row, int col, int mine[MATRIX][MATRIX], int flag[MATRIX][MATRIX])
{
  if(mine[row][col]>0)
  {
    MoveCursor(row+1, col+1);
    flag[row][col]=1;
    printf("\033[40;32;1m%d \033[40;30;0m" , mine[row][col]);
    return ;
  }
  else if(mine[row][col]==-1)
  {
    return ;
  }
  else if(mine[row][col]==0)
  {
    MoveCursor(row+1,  col+1);
    flag[row][col]=1;
    fprintf(stdout, " ");
    mine[row][col]=-1;
    if(row==0&&col==0)
    {
      Set_Blank(row+1, col, mine, flag);
      Set_Blank(row, col+1, mine, flag);
    }
    if(row==0&&(col>0&&col<matrix-1))
    {
      Set_Blank(row, col-1, mine, flag);
      Set_Blank(row, col+1, mine, flag);
      Set_Blank(row+1, col, mine, flag);
    }
    if(row==0&&col==matrix-1)
    {
      Set_Blank(row, col-1, mine, flag);
      Set_Blank(row+1, col, mine, flag);
    }
    if((row<matrix-1&&row>0)&&col==0)
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row+1, col, mine, flag);
      Set_Blank(row, col+1, mine, flag);
    }
    if((row<matrix-1&&row>0)&&(col>0&&col<matrix-1))
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row+1, col, mine, flag);
      Set_Blank(row, col+1, mine, flag);
      Set_Blank(row, col-1, mine, flag);
    }
    if((row<matrix-1&&row>0)&&col==matrix-1)
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row+1, col, mine, flag);
      Set_Blank(row, col-1, mine, flag);
    }
    if(row==matrix-1&&col==0)
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row, col+1, mine, flag);
    }
    if(row==matrix-1&&(col>0&&col<matrix-1))
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row, col-1, mine, flag);
      Set_Blank(row, col+1, mine, flag);
    }
    if(row==matrix-1&&col==matrix-1)
    {
      Set_Blank(row, col-1, mine, flag);
      Set_Blank(row-1, col, mine, flag);
    }
  }
}
//显示某位置的内容，有雷就炸，以此格子为中心的九宫格内有雷，则显示雷的数目，没雷则显示相连的空白  
//参数 雷阵，标志数组， 行列
void DisplayCursor(int mine[MATRIX][MATRIX], int flag[MATRIX][MATRIX],  int row, int col)
{
    if(mine[row-1][col-1]==MINE_VAL)
    {
      BomMine(mine);
    }
    else if(mine[row-1][col-1]!=0)
    {
      MoveCursor(row, col);
      flag[row-1][col-1]=1;
      printf("\033[40;32;1m%d \033[40;30;0m", mine[row-1][col-1]);
    }
    else// diguibianlikongge MINE[row-1][col-1]==0
    {
        Set_Blank(row-1,  col-1, mine, flag);
    }
}
//显示youwin   参数返回值空
void YouWin(void)
{
  CLEAR();
  RESET_CURSOR();
  printf(" ***       ***      ***       @      *** *******           *****                 \n");
  printf("  ***     ** **    ***      ****    ****    ***           ****               \n");
  printf("   ***   ***  ***  ***      ****     ***    ***            ***               \n");
  printf("     ****      ****        ****     ***    ***            @            \n");
  exit(1);
}
//游戏入口函数   参数返回值：空
void StartGame()
{
    int count=0;
    int row=1, col=1;
    char tempchar, state;
    int mineremain=0, true_mineremain=0;
	pthread_t tid;
    int mine[MATRIX][MATRIX]={0};
    int flag[MATRIX][MATRIX]={0};
    int minenumber=0;
    srand((unsigned long)time(0));
    printf("please input the dimension of the matrix(9~16)\n");
    while(!(cin>>matrix)||(matrix>MATRIX||matrix<MINIMUN))
    {
		cin.clear();
		cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        printf("input error , please iput again\n");
    }
    printf("please wait a monment.....\n");
    CLEAR();
    RESET_CURSOR();
    InitializeMine(mine);
    sleep(1);
    mineremain=(matrix-9)*5+10;
    true_mineremain=mineremain;
    Initialize();
    printf("mineremain=%d     \n", mineremain  );
	count=0;
	int err=0;
    while(1)
    {
		/*err=pthread_creat(&tid, NULL, (void*)getch, NULL);*/
		/*if(err==0)*/
		/*{*/
			/*printf("pthread create error, prggram adbort\n");*/
			/*return 0;*/
		/*}*/
		state=(char)scanKeyboard();
		printf("...i\n");
		if(count!=1)
		{
			Sub_MoveCursor(row, col, 0);
		}
		count++;
		if(state=='I')
		{
			if(row==1)
			{
				row=matrix;
				Sub_MoveCursor(row, col, 1);
			}
			else
			{
				row--;
				Sub_MoveCursor(row, col, 1);
			}
		}
		else if(state=='K')
		{
			if(row==matrix)
			{
				row=1;
				Sub_MoveCursor(row, col, 1);
			}
			else
			{
				row++;
				Sub_MoveCursor(row, col, 1);
			}
		}
		else if(state=='J') 
		{
			if(col==1)
			{
				col=matrix;
				Sub_MoveCursor(row, col, 1);
			}
			else
			{
				col--;
				Sub_MoveCursor(row, col, 1);
			}
		}
		else if(state=='L')
		{
			if(col==matrix)
			{
				col=1;
				Sub_MoveCursor(row, col, 1);
			}
			else
			{
				col++;
				Sub_MoveCursor(row, col, 1);
			}
		}
        else if(state=='A')
        {
          if(flag[row-1][col-1]!=1&&flag[row-1][col-1]!=2)
          {
            DisplayCursor(mine, flag, row, col);
          }
          if(flag[row-1][col-1]==2)
          {
            DisplayCursor(mine, flag, row, col);
			mineremain++;
			RESET_CURSOR();
            MoveCursor(matrix+2, 1);
            printf("mineremain=%d     \n", mineremain  );
          }
        }
        else if(state=='B')
        {
          if(flag[row-1][col-1]!=1&&flag[row-1][col-1]!=2)
          {
            MoveCursor(row,col);
			printf("\033[40;31;1m@\033[40;30;0m");
            flag[row-1][col-1]=2;
            mineremain--;
            RESET_CURSOR();
            MoveCursor(matrix+2, 1);
            printf("mineremain=%d     \n", mineremain  );
            if(mine[row-1][col-1]==MINE_VAL)
            {
              true_mineremain--;
			  if(true_mineremain==0)
			  {
				  YouWin();
			  }
            }
          }
		}
        RESET_CURSOR();
		MOVEDOWN(matrix+2);
    }
}
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
	while(!(cin>>mode)||!(mode==1||mode==2)){
		if(!cin){
			cin.clear();
			cin.ignore( numeric_limits<streamsize>::max(), '\n' );
			printf("input error , please iput again\n");
		}
		else
			break;
		//throw Runtime
	}
	int pipeToSocket1[2], pipeToSocket2[2];
	pipe(pipeToSocket1);
	pipe(pipeToSocket2);
	SocketManager sock;
	if(mode==1){
		//TODO::扫描端口,空闲使用
		//TODO::这里是否可以选择不用if-else 语句来实现
		std::string ipAddr, port;
		cout<<"请输入对方Ip(如果输入为1 ,则进行本地监听,等待对方连接)";
		inputUntilTrue(ipAddr, "输入判断失败,请重试|input error, try again", [](decltype(ipAddr) mode){return true;});
		if(ipAddr!="1"){
			cout<<"请输入对方端口";
			inputUntilTrue(port, "输入判断失败,请重试|input error, try again", [](decltype(ipAddr) mode){return true;});
			auto fd=sock.connect(ipAddr, port);
		}else{
			auto fd=sock.bindAndListenSocket("127.0.0.1");
			cout<<fd<<endl;
		}
	}
	pid_t pid;
	pid=fork();
	if(pid<0){
		printf("进程创建失败");
	}else if(pid>0){
		StartGame();
	}else{
		wait();
	}
    return 0;
}
