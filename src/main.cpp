/*
 * main.cpp
 *
 *  Created on: 10/12/2013
 *      Author: Michael Whitman
 */

#include <iostream>
#include <fstream>
//#include <chrono>

#include "Vote.h"

int main() {

	//auto t1 = std::chrono::high_resolution_clock::now();

	//writeTest("./Debug/random.txt", 3, 100, 100, 500);

	//std::filebuf fb;
	//fb.open ("random.txt", std::ios_base::in);
	//std::istream testFile (&fb);
	//runTest(testFile, true);

	//auto t2 = std::chrono::high_resolution_clock::now();

	//std::cout << "Elapsed: "
	//	 << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
	//	 << " ms\n";

	//fb.close();

	runTest(std::cin, false);

	return 0;
}


