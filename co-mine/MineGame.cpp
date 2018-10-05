#include "MineGame.h"
#include<termio.h>
#include<sys/ioctl.h>
#include<array>
#include<unistd.h>
#include<curses.h>
#include<memory>


MineGame::MineGame(int dim):matrixDim(dim){
	toScreen=new Display();
	Com=std::make_shared<Communicate>();
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
void MineGame::InitializeMine(int mine[MAX_DIM][MAX_DIM])
{
    //����ı����ֱ�������¼ �н� �н� Ӧ���׵����� ����������� ʵ�ʵ�����Ŀ
    int countone=0, countsec=0,amount=0,randnum=0, minenum=0;
    amount=(matrixDim-9)*5+10;
    while(minenum!=amount){          //ֱ���������㹻���׺�ֹͣѭ��
        for(countone=0;countone<matrixDim;countone++){
            for(countsec=0;countsec<matrixDim;countsec++){
                randnum=rand()%8+19;//MINE_VAL����
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
	//����ѭ�����������ҳ���Ӧ�������ܵ��׵���Ŀ
	for (countone = 0; countone<matrixDim; countone++)
	{
		for (countsec = 0; countsec<matrixDim; countsec++)
		{
			minenum = 0;
			if (mine[countone][countsec] != MINE_VAL)         //ͨ���ж��ҳ����ĸ���Χ������
			{
				if (judge(countone - 1, countsec - 1))
				{
					if (mine[countone-1][countsec-1] == MINE_VAL)
						minenum++;
				}
				if (judge(countone - 1, countsec))
				{
					if (mine[countone - 1][countsec] == MINE_VAL)
						minenum++;
				}
				if (judge(countone - 1, countsec + 1) )
				{
					if (mine[countone - 1][countsec + 1] == MINE_VAL)
						minenum++;
				}
				if (judge(countone, countsec - 1))
				{
					if (mine[countone][countsec - 1]==MINE_VAL)
						minenum++;
				}
				if (judge(countone, countsec + 1))
				{
					if (mine[countone][countsec + 1] == MINE_VAL)
						minenum++;
				}
				if (judge(countone + 1, countsec - 1))
				{
					if (mine[countone + 1][countsec - 1] == MINE_VAL)
						minenum++;
				}
				if (judge(countone + 1, countsec))
				{
					if (mine[countone + 1][countsec] == MINE_VAL)
						minenum++;
				}
				if (judge(countone + 1, countsec + 1))
				{
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
        if(countsec==matrixDim)
        {
            printf("%d", countone+1);
            printf("\n");
        }
    }
    for(countone=1;countone<=matrixDim;countone++)
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
//��ʾ���   ��������Ļ�ϵ�����  �Լ���־ 1��0 ����ֵ����
void MineGame::Sub_MoveCursor(int row ,int col, int flag)//flag���������� ��1ʱ��ʾ��� ��0ʱ�ػ���Ļ
{
	Display::reset();
	Display::moveTo(row, col);
	Display::moveRight();
	if(flag==1){
		printf("\033[44;34;1m|\033[40;30;0m" );
	}
	else if(flag==0){
		printf(" ");
	}
}

//�ױ����� ��ʾ���е��� �������ը��Ч  �������� ���أ���
void MineGame::BomMine(int mine[MAX_DIM][MAX_DIM])
{
      struct winsize ws;
      ioctl(0, TIOCGWINSZ, &ws);
      int countone=0, countsec=0;
      for(countone=0;countone<matrixDim;countone++)
      {
        for(countsec=0;countsec<matrixDim;countsec++)
        {
              if(mine[countone][countsec]==MINE_VAL)
              {
                  Display::moveTo(countone+1, countsec+1);
                  printf("\033[;31;1mMN\033[;30;0m" );
              }
          }
      }
      fflush(stdout);
      sleep(3);
      Display::reset();
      for(countone=0;countone<matrixDim+10;countone++)
      {
          for(countsec=0;countsec<(int)ws.ws_col;++countsec)
          {
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
void MineGame::Set_Blank(int row, int col, int mine[MAX_DIM][MAX_DIM], int flag[MAX_DIM][MAX_DIM])
{
  if(mine[row][col]>0)
  {
    Display::moveTo(row+1, col+1);
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
    Display::moveTo(row+1,  col+1);
    flag[row][col]=1;
    fprintf(stdout, " ");
    mine[row][col]=-1;
    if(row==0&&col==0)
    {
      Set_Blank(row+1, col, mine, flag);
      Set_Blank(row, col+1, mine, flag);
    }
    if(row==0&&(col>0&&col<matrixDim-1))
    {
      Set_Blank(row, col-1, mine, flag);
      Set_Blank(row, col+1, mine, flag);
      Set_Blank(row+1, col, mine, flag);
    }
    if(row==0&&col==matrixDim-1)
    {
      Set_Blank(row, col-1, mine, flag);
      Set_Blank(row+1, col, mine, flag);
    }
    if((row<matrixDim-1&&row>0)&&col==0)
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row+1, col, mine, flag);
      Set_Blank(row, col+1, mine, flag);
    }
    if((row<matrixDim-1&&row>0)&&(col>0&&col<matrixDim-1))
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row+1, col, mine, flag);
      Set_Blank(row, col+1, mine, flag);
      Set_Blank(row, col-1, mine, flag);
    }
    if((row<matrixDim-1&&row>0)&&col==matrixDim-1)
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row+1, col, mine, flag);
      Set_Blank(row, col-1, mine, flag);
    }
    if(row==matrixDim-1&&col==0)
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row, col+1, mine, flag);
    }
    if(row==matrixDim-1&&(col>0&&col<matrixDim-1))
    {
      Set_Blank(row-1, col, mine, flag);
      Set_Blank(row, col-1, mine, flag);
      Set_Blank(row, col+1, mine, flag);
    }
    if(row==matrixDim-1&&col==matrixDim-1)
    {
      Set_Blank(row, col-1, mine, flag);
      Set_Blank(row-1, col, mine, flag);
    }
  }
}
//��ʾĳλ�õ����ݣ����׾�ը���Դ˸���Ϊ���ĵľŹ��������ף�����ʾ�׵���Ŀ��û������ʾ�����Ŀհ�  
//���� ���󣬱�־���飬 ����
void MineGame::DisplayCursor(int mine[MAX_DIM][MAX_DIM], int flag[MAX_DIM][MAX_DIM],  int row, int col)
{
    if(mine[row-1][col-1]==MINE_VAL)
    {
      BomMine(mine);
    }
    else if(mine[row-1][col-1]!=0)
    {
      Display::moveTo(row, col);
      flag[row-1][col-1]=1;
      printf("\033[40;32;1m%d \033[40;30;0m", mine[row-1][col-1]);
    }
    else// diguibianlikongge MINE[row-1][col-1]==0
    {
        Set_Blank(row-1,  col-1, mine, flag);
    }
}
//��ʾyouwin   ��������ֵ��
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
//��Ϸ��ں���   ��������ֵ����
void MineGame::run()
{
    int mine[MAX_DIM][MAX_DIM]={0};
    int flag[MAX_DIM][MAX_DIM]={0};
    srand((unsigned long)time(0));
    printf("please wait a monment.....\n");
    Display::clear();
    Display::reset();
    InitializeMine(mine);
    sleep(1);
    mineremain=(matrixDim-9)*5+10;
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
				row=matrixDim;
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
			if(row==matrixDim)
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
				col=matrixDim;
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
			if(col==matrixDim)
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
			Display::reset();
            Display::moveTo(matrixDim+2, 1);
            printf("mineremain=%d     \n", mineremain  );
          }
        }
        else if(state=='B')
        {
          if(flag[row-1][col-1]!=1&&flag[row-1][col-1]!=2)
          {
            Display::moveTo(row,col);
			printf("\033[40;31;1m@\033[40;30;0m");
            flag[row-1][col-1]=2;
            mineremain--;
            Display::reset();
            Display::moveTo(matrixDim+2, 1);
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
        Display::reset();
		Display::moveDown(matrixDim+2);
    }
}

