/*
 * main.cpp
 *
 *  Created on: 10/12/2013
 *      Author: michael
 */

#include <iostream>
#include <fstream>
#include <chrono>

#include "Vote.h"

int main() {

	//auto t1 = std::chrono::high_resolution_clock::now();

	//writeTest("random.txt");
	runTest("testfile.txt", false);

	auto t2 = std::chrono::high_resolution_clock::now();

	//std::cout << "Elapsed: "
	//	 << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
	//	 << " ms\n";

	return 0;
}


