#include "myutils.h"

int getRandomInt()
{
	//以当前时间作为种子，吐槽:真的长，
	std::default_random_engine e(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::uniform_int_distribution<unsigned int> uig;
	return uig(e);
}
