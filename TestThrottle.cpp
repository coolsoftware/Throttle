// TestThrottle.cpp : Defines the entry point for the console application.
//

// Test application for Throttle class
// by Vitaly Yakovlev, November 28, 2014
// http://blog.coolsoftware.ru/

#include "stdafx.h"
#include "Throttle.h"
#include <time.h>
#include <iostream>

int main(int argc, char * argv[])
{
	throttle t(15);

	clock_t cbegin = clock();

	for (int i = 0; i < 1000; i++)
	{
		while (!t.acquire());

		clock_t now = clock();
		std::cout << "[" << i << "] " << double(now - cbegin) / CLOCKS_PER_SEC << " sec." << std::endl;
	}

	clock_t cend = clock();

	std::cout << "Completed. " << double(cend - cbegin) / CLOCKS_PER_SEC << " sec." << std::endl;

	system("pause");

	return 0;
}

