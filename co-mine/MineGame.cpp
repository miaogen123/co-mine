#include "MineGame.h"
#include"myutils.h"
#include<iostream>
#include<future>
#include<thread>
#include<termio.h>
#include<sys/ioctl.h>
#include<array>
#include<random>
#include<unistd.h>
#include<stdexcept>
#include<memory>


MineGame::MineGame(int dim):matrixDim(dim){
	Com=std::make_shared<Communicate>(2);
	userID = static_cast<unsigned char>(getRandomInt());

}

MineGame::~MineGame()
{
}

MineGame * MineGame::getMineGame(int dim){
	static MineGame instance(dim);
	return &instance;
}


bool MineGame::judge(int x, int y) {
	return x > -1 && x < matrixDim && y > -1 && y < matrixDim ? true : false;
}

void MineGame::writeTofd(int fd)
{
	//std::array<char, 2>buf;
	char buf[2];
	while(true){
		char state=(char)scanKeyboard();
		buf[0] = state;
		buf[1] = userID;

#ifdef DEBUG_
		std::cout << "\nget char" << buf[0] << "\t" << int(buf[1]) << endl;
#endif // DEBUG_

		write(fd, buf, 2);
	}
}

void MineGame::process()
{
	std::string command("");
	int ind = 0;
	char state;
	int count = 0;
    while(true){

		count++;
		command=Com->waitAndGetData(commandSize);
		if(ind>=command.size()){
			ind = 0;
		}

		if (command.size() < 2)
			continue;
		state = command[ind];
		ind += 2;
		//printf("...i\n");
		//if(count!=1){
		//	Sub_MoveCursor(row, col, 0);
		//}
		//count++;
		Sub_MoveCursor(row, col, 1);
		Sub_MoveCursor(row, col, 0);
		if(state=='I'){
			if(row==1){
				row=matrixDim;
				Sub_MoveCursor(row, col, 1);
			}
			else{
				row--;
				Sub_MoveCursor(row, col, 1);
			}
		}
		else if(state=='K'){
			if(row==matrixDim){
				row=1;
				Sub_MoveCursor(row, col, 1);
			}
			else{
				row++;
				Sub_MoveCursor(row, col, 1);
			}
		}
		else if(state=='J') {
			if(col==1){
				col=matrixDim;
				Sub_MoveCursor(row, col, 1);
			}
			else{
				col--;
				Sub_MoveCursor(row, col, 1);
			}
		}
		else if(state=='L'){
			if(col==matrixDim){
				col=1;
				Sub_MoveCursor(row, col, 1);
			}
			else{
				col++;
				Sub_MoveCursor(row, col, 1);
			}

		}
        else if(state=='A'){
          if(flag[row-1][col-1]!=1&&flag[row-1][col-1]!=2){
            DisplayCursor( row, col);
          }
          if(flag[row-1][col-1]==2){
            DisplayCursor( row, col);
			mineremain++;
			Display::reset();
            Display::moveTo(matrixDim+2, 1);
            printf("mineremain=%d     \n", mineremain  );
          }
        }
        else if(state=='B'){
          if(flag[row-1][col-1]!=1&&flag[row-1][col-1]!=2){
            Display::moveTo(row,col);
			printf("\033[40;31;1m@\033[40;30;0m");
            flag[row-1][col-1]=2;
            mineremain--;
            Display::reset();
            Display::moveTo(matrixDim+2, 1);
            printf("mineremain=%d     \n", mineremain  );
            if(mine[row-1][col-1]==MINE_VAL){
              true_mineremain--;
			  if(true_mineremain==0){
				  YouWin();
			  }
            }
          }
		}
        Display::reset();
		Display::moveDown(matrixDim+2);
    }
}

int MineGame::scanKeyboard()
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
void MineGame::InitializeMine()
{
    //下面的变量分别用来记录 行阶 列阶 应有雷的数量 产生的随机数 实际的雷数目
    int countone=0, countsec=0,amount=0,randnum=0, minenum=0;
    amount=(matrixDim-9)*5+10;
    while(minenum!=amount){          //直到生成了足够的雷后停止循环
        for(countone=0;countone<matrixDim;countone++){
            for(countsec=0;countsec<matrixDim;countsec++){
                randnum=rand()%8+19;//MINE_VAL有雷
                if(mine[countone][countsec]==MINE_VAL){
                  continue;
                }
                else if(randnum==MINE_VAL ){
                    if(minenum==amount){
                        mine[countone][countsec]=randnum-1;
                    }
                    else{
                        mine[countone][countsec]=randnum;
                        minenum++;
                    }
                }
                else{
                  mine[countone][countsec] = randnum;
                }
              }
          }
    }
	//进行循环遍历，查找出对应雷区四周的雷的数目
	for (countone = 0; countone<matrixDim; countone++){
		for (countsec = 0; countsec<matrixDim; countsec++){
			minenum = 0;
			if (mine[countone][countsec] != MINE_VAL)         //通过判断找出中心格周围的雷数
			{
				if (judge(countone - 1, countsec - 1)){
					if (mine[countone-1][countsec-1] == MINE_VAL)
						minenum++;
				}
				if (judge(countone - 1, countsec)){
					if (mine[countone - 1][countsec] == MINE_VAL)
						minenum++;
				}
				if (judge(countone - 1, countsec + 1) ){
					if (mine[countone - 1][countsec + 1] == MINE_VAL)
						minenum++;
				}
				if (judge(countone, countsec - 1)){
					if (mine[countone][countsec - 1]==MINE_VAL)
						minenum++;
				}
				if (judge(countone, countsec + 1)){
					if (mine[countone][countsec + 1] == MINE_VAL)
						minenum++;
				}
				if (judge(countone + 1, countsec - 1)){
					if (mine[countone + 1][countsec - 1] == MINE_VAL)
						minenum++;
				}
				if (judge(countone + 1, countsec)){
					if (mine[countone + 1][countsec] == MINE_VAL)
						minenum++;
				}
				if (judge(countone + 1, countsec + 1)){
					if (mine[countone + 1][countsec + 1] == MINE_VAL)
						minenum++;
				}
				mine[countone][countsec] = minenum;         //中心格上记录周围的雷数
			}
		}
	}
}

//FUNC：显示初始化界面 参数:无 返回值：无
void MineGame::Initialize(void)
{
  int countone=0, countsec=0;
  for(countone=0;countone<matrixDim;countone++){
        for(countsec=0;countsec<matrixDim;countsec++){
              printf("#   ");
        }
        if(countsec==matrixDim){
            printf("%d", countone+1);
            printf("\n");
        }
    }
    for(countone=1;countone<=matrixDim;countone++){
      if(countone<10){
              printf("%d   ", countone);
      }
      else{
        printf("%d  ", countone);
      }
    }
    printf("\n");
}
//显示光标   参数：屏幕上的行列  以及标志 1或0 返回值：空
void MineGame::Sub_MoveCursor(int row ,int col, int flag)//flag的作用在于 是1时显示光标 是0时重画屏幕
{
	Display::reset();
	Display::moveTo(row, col);
	Display::moveRight();

	//fflush(stdout);
	if(flag==1){
		printf("\033[44;34;1m|\033[40;30;0m" );
	}
	else if(flag==0){
		printf(" ");
	}
	Display::moveTo(matrixDim+2, 1);
	printf("\n");
}

//雷被引爆 显示所有的雷 并输出爆炸特效  参数：无 返回：空
void MineGame::BomMine()
{
      struct winsize ws;
      ioctl(0, TIOCGWINSZ, &ws);
      int countone=0, countsec=0;
      for(countone=0;countone<matrixDim;countone++){
        for(countsec=0;countsec<matrixDim;countsec++){
              if(mine[countone][countsec]==MINE_VAL){
                  Display::moveTo(countone+1, countsec+1);
                  printf("\033[;31;1mMN\033[;30;0m" );
              }
          }
      }
      fflush(stdout);
      sleep(3);
      Display::reset();
      for(countone=0;countone<matrixDim+10;countone++){
          for(countsec=0;countsec<(int)ws.ws_col;++countsec){
            printf(" ");
          }
          printf("\n");
          usleep(40000);
      }
      sleep(1);
      Display::clear();
      Display::reset();
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
void MineGame::Set_Blank(int row, int col)
{
  if(mine[row][col]>0){
    Display::moveTo(row+1, col+1);
    flag[row][col]=1;
    printf("\033[40;32;1m%d \033[40;30;0m" , mine[row][col]);
    return ;
  }
  else if(mine[row][col]==-1){
    return ;
  }
  else if(mine[row][col]==0){
    Display::moveTo(row+1,  col+1);
    flag[row][col]=1;
    fprintf(stdout, " ");
    mine[row][col]=-1;
    if(row==0&&col==0){
      Set_Blank(row+1, col);
      Set_Blank(row, col+1);
    }
    if(row==0&&(col>0&&col<matrixDim-1)){
      Set_Blank(row, col-1);
      Set_Blank(row, col+1);
      Set_Blank(row+1, col);
    }
    if(row==0&&col==matrixDim-1){
      Set_Blank(row, col-1);
      Set_Blank(row+1, col);
    }
    if((row<matrixDim-1&&row>0)&&col==0){
      Set_Blank(row-1, col);
      Set_Blank(row+1, col);
      Set_Blank(row, col+1);
    }
    if((row<matrixDim-1&&row>0)&&(col>0&&col<matrixDim-1)){
      Set_Blank(row-1, col);
      Set_Blank(row+1, col);
      Set_Blank(row, col+1);
      Set_Blank(row, col-1);
    }
    if((row<matrixDim-1&&row>0)&&col==matrixDim-1){
      Set_Blank(row-1, col);
      Set_Blank(row+1, col);
      Set_Blank(row, col-1);
    }
    if(row==matrixDim-1&&col==0){
      Set_Blank(row-1, col);
      Set_Blank(row, col+1);
    }
    if(row==matrixDim-1&&(col>0&&col<matrixDim-1)){
      Set_Blank(row-1, col);
      Set_Blank(row, col-1);
      Set_Blank(row, col+1);
    }
    if(row==matrixDim-1&&col==matrixDim-1){
      Set_Blank(row, col-1);
      Set_Blank(row-1, col);
    }
  }
}

//显示某位置的内容，有雷就炸，以此格子为中心的九宫格内有雷，则显示雷的数目，没雷则显示相连的空白  
//参数 雷阵，标志数组， 行列
void MineGame::DisplayCursor(int row, int col)
{
    if(mine[row-1][col-1]==MINE_VAL){
      BomMine();
    }
    else if(mine[row-1][col-1]!=0){
      Display::moveTo(row, col);
      flag[row-1][col-1]=1;
      printf("\033[40;32;1m%d \033[40;30;0m", mine[row-1][col-1]);
    }
    else// diguibianlikongge MINE[row-1][col-1]==0
	{
        Set_Blank(row-1,  col-1);
    }
}
//显示youwin   参数返回值空
void MineGame::YouWin(void)
{
  Display::clear();
  Display::reset();
  printf(" ***       ***      ***       @      *** *******           *****                 \n");
  printf("  ***     ** **    ***      ****    ****    ***           ****               \n");
  printf("   ***   ***  ***  ***      ****     ***    ***            ***               \n");
  printf("     ****      ****        ****     ***    ***            @            \n");
  exit(1);
}
//游戏入口函数   参数返回值：空
void MineGame::run()
{
    srand((unsigned long)time(0));
    printf("please wait a monment.....\n");
    Display::clear();
    Display::reset();
    InitializeMine();
    sleep(1);
    mineremain=(matrixDim-9)*5+10;
    true_mineremain=mineremain;
    Initialize();
    printf("mineremain=%d     \n", mineremain  );
	//count=0;
	int err=0;
	int stdinToListenSource[2];
	{	int tmpCountTry = 0;
		while (pipe(stdinToListenSource) != 0) {

			tmpCountTry++;
			if (tmpCountTry >= 5)
				throw std::runtime_error("管道映射失败, 程序无法继续运行");
			sleep(0.5);
	}    }
	this->Com->addfd(stdinToListenSource[0]);

 	//TODO::搞明白下面 代码
	//auto retFu = std::async(std::launch::async, writeTofd, stdinToListenSource[1]);
	std::thread getChar(&MineGame::writeTofd,this, stdinToListenSource[1]);
	getChar.detach();
	std::thread processInput(&MineGame::process, this);
	processInput.detach();
	Com->process();
}
