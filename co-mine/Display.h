#pragma once
#include"globalConfig.h"
#include<cstdio>
#include<string>


//仅支持替换字体颜色
enum class FontColor :unsigned short {
	BLACK = 30,
	RED = 31,
	GREEN=32,
	YELLOW=33,
	BLUE=34,
	PURPPLE=35,
	DEEPGREE=36,
	WHITE=37
};

class Display
{
public:
	Display();
	static void clear();
	static void reset();
	static void showCursor();
	static void moveUp(unsigned int x=1);
	static void moveTo(unsigned int x=1, unsigned int y=1);
	static void moveDown(unsigned int x=1);
	static void moveLeft(unsigned int x=1);
	static void moveRight(unsigned int x=1);
	//static void showSthAt(unsigned int x, unsigned int y,std::string content); 
	static void showSthAt(unsigned int x, unsigned int y,std::string content, FontColor before=before, FontColor after=after); 
	//static void showSth(std::string content); 
	static void showSth(std::string content,  FontColor before=before, FontColor after=after);
	~Display();
	static FontColor before;
	static FontColor after;
};