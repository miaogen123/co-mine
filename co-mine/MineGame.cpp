#include "MineGame.h"
#include"myutils.h"
#include<iostream>
#include<future>
#include"string.h"
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
	self_user = std::make_shared<UserStat>(userID);
	self_user->setFontColor(FontColor::GREEN);
	users[userID] = self_user;
	assignColor(FontColor::GREEN);
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

//TODO::��ϸ�����߼���÷��뵽������֮�⣬��Ϊ���������߼��Ƚϸ��ӣ����ܾ�����Ҫ��
void MineGame::process()
{
	std::string command("");
	int ind = 0;
	char state;
	int count = 0;
    while(true){
		command=Com->waitAndGetData(commandSize);
		if(ind>=command.size()){
			ind = 0;
		}

		if (command.size() < 2)
			continue;

		state = command[ind];
		auto netUserId=static_cast<unsigned char>(command[ind + 1]);
		currentUserId = netUserId;
		ind += 2;
		//cout << "userid " <<static_cast<unsigned char>(netUserId)<< endl;
		//cout << "command " << command<<endl;

		if (users.find(netUserId) == users.end()) {
			auto cursorColor= assignColor();
			auto netUser = std::make_shared<UserStat>(netUserId);
			netUser->setFontColor(cursorColor);
			users[netUserId] = netUser;
		}
		Display::before = users[netUserId]->getFontColor();
		int &row = users[netUserId]->getRow();
		int &col= users[netUserId]->getCol();
		//cout << row << "\t" << col << endl;
		//cout << static_cast<int>(netUserId)<< "\tcolor value\t" << static_cast<unsigned short>(userColor[netUserId])  << endl;

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
			if(count==0){
				do {
					InitializeMine();
				} while (mine[row - 1][col - 1] >0);
			}
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
			count++;
        }
        else if(state=='B'){
			//NOTE::��һ�ε����Ҫ���A
			if (count == 0)
				continue;
          if(flag[row-1][col-1]!=1&&flag[row-1][col-1]!=2){
            //Display::moveTo(row,col);
			  Display::showSthAt(row, col, "@");
			//printf("\033[40;31;1m@\033[40;30;0m");
            flag[row-1][col-1]=2;
            mineremain--;
            Display::reset();
            Display::moveTo(matrixDim+2, 1);
            printf("mineremain=%d     \n", mineremain  );
            if(mine[row-1][col-1]==MINE_VAL){
              true_mineremain--;
			  users[netUserId]->mineNum++;
			  if(true_mineremain==0){
				  //NOTE::����Ϊ��������չ�ԣ�ѡ���ҳ�ɨ��������һ����ΪӮ��
				  judgeWinOrFail(users);
			  }
            }
          }
		}
        Display::reset();
		Display::moveDown(matrixDim+2);
    }
}

void MineGame::judgeWinOrFail(std::map<unsigned char, std::shared_ptr<UserStat>>& user)
{
	  int maxNum = 0;
	  for (const auto & val : users) {
			  maxNum = maxNum >( val.second)->mineNum ? maxNum : (val.second)->mineNum;
		}
	  for (const auto & val : users) {
		  if ((val.second)->mineNum>=maxNum) {
			  YouWin();
		  }
		  else {
			  BomMine();
		  }
		}
}

FontColor MineGame::assignColor(FontColor fc)
{
	//8��flag��FontColor�İ˸���Ӧ�����к�ɫ����Ϊ�������ֲ���, ����Ĭ��ʹ����ɫ
	static std::array<bool, 8> colorUsed; 
	if (fc != FontColor::BLACK) {
		if (!colorUsed[static_cast<unsigned short>(fc)-30]) {
			colorUsed[static_cast<unsigned short>(fc) - 30] = true;
			return fc;
		}
	}

	for (auto i = 1; i < colorUsed.size(); i++) {
		if (!colorUsed[i]) {
			colorUsed[i] = true;
			return static_cast<FontColor>(30 + i);
		}
	}
	throw std::runtime_error("The num of users has reached the max limited\n");;

}

void MineGame::setSeedToGenerateMatrix(time_t seed)
{
	seedToGenerateMatrix = seed;
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


//��ʼ������ɲ��� ���� ����ֵ ��
void MineGame::InitializeMine()
{
    //����ı����ֱ�������¼ �н� �н� Ӧ���׵����� ����������� ʵ�ʵ�����Ŀ
    srand((unsigned long)seedToGenerateMatrix);
	//ÿ��ʹ�ù����Ӷ�Ҫ�����仯
	seedToGenerateMatrix += 5;
	//srand(time(0));
	memset(mine, 0, sizeof(mine));
    int countone=0, countsec=0,amount=0, minenum=0;
    amount=(matrixDim-9)*5+10;
	int r = 0, c = 0;
    while(minenum!=amount){          //ֱ���������㹻���׺�ֹͣѭ��
		r=rand()%matrixDim;
		c=rand()%matrixDim;
		if(mine[r][c]!=MINE_VAL){
			mine[r][c] = MINE_VAL;
			minenum++;
		}
    }
	//����ѭ�����������ҳ���Ӧ�������ܵ��׵���Ŀ
	for (countone = 0; countone<matrixDim; countone++){
		for (countsec = 0; countsec<matrixDim; countsec++){
			minenum = 0;
			if (mine[countone][countsec] != MINE_VAL)         //ͨ���ж��ҳ����ĸ���Χ������
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
				mine[countone][countsec] = minenum;         //���ĸ��ϼ�¼��Χ������
			}
		}
	}
}

//FUNC����ʾ��ʼ������ ����:�� ����ֵ����
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

//��ʾ���   ��������Ļ�ϵ�����  �Լ���־ 1��0 ����ֵ����
void MineGame::Sub_MoveCursor(int row ,int col, int flag)//flag���������� ��1ʱ��ʾ��� ��0ʱ�ػ���Ļ
{
	Display::reset();
	Display::moveTo(row, col);
	Display::moveRight();

	//fflush(stdout);
	if(flag==1){
		//printf("\033[44;34;1m|\033[40;30;0m" );
		Display::showSth("B");
	}
	else if(flag==0){
		printf(" ");
	}
	Display::showSthAt(matrixDim + 2, 1, "\n");
	//Display::moveTo(matrixDim+2, 1);
	//printf("\n");
}

//�ױ����� ��ʾ���е��� �������ը��Ч  �������� ���أ���
void MineGame::BomMine()
{
      struct winsize ws;
      ioctl(0, TIOCGWINSZ, &ws);
      int countone=0, countsec=0;
      for(countone=0;countone<matrixDim;countone++){
        for(countsec=0;countsec<matrixDim;countsec++){
              if(mine[countone][countsec]==MINE_VAL){
				  Display::showSthAt(countone + 1, countsec + 1, "MN");
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
//������ҳ����е���Χ���׵ĸ��Ӳ���ʾΪ��   �������У��У��׵����飬 ��־����
void MineGame::Set_Blank(int row, int col)
{
  if(mine[row][col]>0){
    Display::moveTo(row+1, col+1);
    flag[row][col]=1;
    //printf("\033[40;32;1m%d \033[40;30;0m" , mine[row][col]);
	Display::showSth(std::to_string(mine[row][col]) );
	  Display::moveTo(matrixDim+2, 1);
	  printf("\n");
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

//��ʾĳλ�õ����ݣ����׾�ը���Դ˸���Ϊ���ĵľŹ��������ף�����ʾ�׵���Ŀ��û������ʾ�����Ŀհ�  
//���� ���󣬱�־���飬 ����
void MineGame::DisplayCursor(int row, int col)
{
    if(mine[row-1][col-1]==MINE_VAL){
		if (currentUserId == userID) {
			users.erase(userID);
			BomMine();
		}
		else
			judgeWinOrFail(users);
    }
    else if(mine[row-1][col-1]!=0){
      Display::moveTo(row, col);
      flag[row-1][col-1]=1;
      //printf("\033[40;32;1m%d \033[40;30;0m", mine[row-1][col-1]);
	  Display::showSth(std::to_string(mine[row-1][col-1]) );
	  Display::moveTo(matrixDim+2, 1);
	  printf("\n");
    }
    else
	{
        Set_Blank(row-1,  col-1);
    }
}
//��ʾyouwin   ��������ֵ��
void MineGame::YouWin(void)
{
      struct winsize ws;
      ioctl(0, TIOCGWINSZ, &ws);
      int countone=0, countsec=0;
      for(countone=0;countone<matrixDim;countone++){
        for(countsec=0;countsec<matrixDim;countsec++){
              if(mine[countone][countsec]==MINE_VAL){
				  Display::showSthAt(countone + 1, countsec + 1, "MN");
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
  printf(" ***       ***      ***       @      *** *******           *****                 \n");
  printf("  ***     ** **    ***      ****    ****    ***           ****               \n");
  printf("   ***   ***  ***  ***      ****     ***    ***            ***               \n");
  printf("     ****      ****        ****     ***    ***            @            \n");
  exit(1);
}
//��Ϸ��ں���   ��������ֵ����
void MineGame::run()
{
    printf("please wait a monment.....\n");
    Display::clear();
    Display::reset();
    sleep(0.5);
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
				throw std::runtime_error("�ܵ�ӳ��ʧ��, �����޷���������");
			sleep(0.5);
	}    }
	this->Com->addfd(stdinToListenSource[0]);

 	//TODO::���������� ����
	//auto retFu = std::async(std::launch::async, writeTofd, stdinToListenSource[1]);
	std::thread getChar(&MineGame::writeTofd,this, stdinToListenSource[1]);
	getChar.detach();
	std::thread processInput(&MineGame::process, this);
	processInput.detach();
	Com->process();
}
