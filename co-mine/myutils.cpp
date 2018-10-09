#include "myutils.h"
#include<chrono>
#include<unistd.h>


int getRandomInt()
{
	//errror
	//std::default_random_engine e(std::chrono::high_resolution_clock::to_time_t(std::chrono::high_resolution_clock::now())+static_cast<long long>((&temp))&65535 );


	std::default_random_engine e(clock());
	std::uniform_int_distribution<unsigned int> uig;
	return uig(e);

	//srand(std::chrono::high_resolution_clock::to_time_t(std::chrono::high_resolution_clock::now()));
	//return static_cast<int>(rand());
}
