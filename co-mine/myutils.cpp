#include "myutils.h"

int getRandomInt()
{
	//�Ե�ǰʱ����Ϊ���ӣ��²�:��ĳ���
	std::default_random_engine e(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::uniform_int_distribution<unsigned int> uig;
	return uig(e);
}
