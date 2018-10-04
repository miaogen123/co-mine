#pragma once
//µ¥ÀýÊµÏÖ

#include<memory>
#include"Communicate.h"
#include"Display.h"
class MineGame
{
public:
	~MineGame();
	MineGame* getMineGame();
	void run();
private:
	MineGame();
	std::shared_ptr<Display> toScreen;
	std::shared_ptr<Communicate> Com;
};

