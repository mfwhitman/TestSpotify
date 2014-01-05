/*
 * Vote.cpp
 *
 *  Created on: 14/12/2013
 *      Author: michael
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

enum Pet {cat, dog};

class Vote {
	Pet type;
	int loves;
	int hates;
public:
	Vote (Pet,int,int);
	Vote (string);
};

Vote::Vote (Pet _type, int _loves, int _hates) {
	type = _type;
	loves = _loves;
	hates = _hates;
}

Vote::Vote (const string input) {
	signed int spacePos = input.find(" ");
	string first;

	first.assign(input, 0, 1);

	if (first == "C")
	{
		type = cat;
	}
	else
	{
		type = dog;
	}

	string tempLove = input.substr(1,spacePos);
	string tempHate = input.substr((spacePos+1),input.length());
	loves = stoi(tempLove);
	hates = stoi(tempHate);

}

bool checkVote(const string input, const int countCat, const int countDog) {
	//split string based on ' '
	signed int spacePos = input.find(" ");
	if (spacePos == -1)
	{
		std::cerr << "Invalid string spacing: " << spacePos << std::endl;
	}
	string first, second;

	first.assign(input, 0, 1);
	second.assign(input, (spacePos + 1), 1);

	// extract numbers
	string tempLike = input.substr(1,spacePos - 1);
	string tempHate = input.substr((spacePos + 2),input.length());

	// test conversion
	int like, hate;

	try {
		like = stoi(tempLike);
		hate = stoi(tempHate);
	}
	catch  (const std::invalid_argument& ia) {
		std::cerr << "Invalid string conversion: "
				<< ia.what() << " ("
				<< tempLike << ") ("
				<< tempHate << ")" << std::endl;
		return false;
	}


	// verify first letter, verify and compare second letter,
	// then verify both numbers are within range
	if (first == "C")
	{
		if (second == "D")
		{
			if ( (like  > countCat) || (hate > countDog) )
			{
				std::cerr << "Invalid vote numbering:"
						<< like << "/" << countCat << " , "
						<< hate << "/" << countDog << std::endl;
				return false;
			}
		}
		else
		{
			std::cerr << "Invalid vote match:" << first << " " << second << std::endl;
			return false;
		}
	}
	else if (first == "D")
	{
		if (second == "C")
		{
			if ( (like > countDog) || (hate > countCat) )

			{
				std::cerr << "Invalid vote numbering:"
						<< like << "/" << countDog << " , "
						<< hate << "/" << countCat << std::endl;
				return false;
			}
		}
		else
		{
			std::cerr << "Invalid vote match:" << first << " " << second << std::endl;
			return false;
		}
	}
	else
	{
		std::cerr << "Invalid vote lettering:"
				<< first << ", "
				<< second << std::endl;
		return false;
	}

	return true;
}

void writeTest(string voteFile) {
	int numCases = rand() % 100 + 1;

	ofstream writeFile(voteFile);

	if (!writeFile.is_open()) {
		cout << "Unable to open file." << endl;
		return;
	}

	writeFile << numCases << endl;

	for (int i = 0; i < numCases; i++)
	{
		int numCats = rand() % 100 + 1;
		int numDogs = rand() % 100 + 1;
		int numVotes = rand() % 500 + 1;
		writeFile << numCats << " " << numDogs << " " << numVotes << endl;

		for (int j = 0; j < numVotes; j++)
		{
			bool valence = rand() % 2;
			int pickedCat = rand() % numCats + 1;
			int pickedDog = rand() % numDogs + 1;
			if (valence == 0)
			{
				writeFile << "C" << pickedCat << " ";
				writeFile << "D" << pickedDog << endl;
			}
			else
			{
				writeFile << "D" << pickedDog << " ";
				writeFile << "C" << pickedCat << endl;
			}
		}
	}
	writeFile.close();
}

void runTest(string voteFilepath, bool verbose = false) {

	ifstream testFile(voteFilepath, ios_base::in);

	if (!testFile.is_open()) {
		cout << "Unable to open file." << endl;
		return;
	}

	int numCases, numCats, numDogs, numVotes;

	testFile >> numCases;
	if (verbose)
		cout << "Testcases: " << numCases << endl;

	//loop per testcase *****************************************************
	for (int i = 0; i < numCases; i++) {
		if (verbose)
			cout << endl << "Testcase no." << (i + 1) << endl;
		testFile >> numCats >> numDogs >> numVotes;
		if (verbose)
			cout << "Cats: " << numCats << endl;
		if (verbose)
			cout << "Dogs: " << numDogs << endl;
		if (verbose)
			cout << "Votes: " << numVotes << endl << endl;

		int * cats = new int[numCats];
		int * dogs = new int[numDogs];

		for (int j = 0; j < numCats; j++)
			cats[j] = 0;
		for (int j = 0; j < numDogs; j++)
			dogs[j] = 0;

		testFile.ignore(); //one more endl?

		//loop per vote *****************************************************
		for (int j = 0; j < numVotes; j++) {
			string voteline;
			getline(testFile, voteline);
			if (checkVote(voteline, numCats, numDogs))
			{
				//TODO Add vote to graph.
			}

		}

		int max = 0;

		if (verbose)
			cout << endl << "Cat votes: ";
		for (int z = 0; z < numCats; z++) {
			if (verbose)
				cout << cats[z] << " ";
			if (cats[z] > max) {
				max = cats[z];
			}
		}
		if (verbose)
			cout << endl;
		if (verbose)
			cout << "Dog votes: ";
		for (int z = 0; z < numDogs; z++) {
			if (verbose)
				cout << dogs[z] << " ";
			if (dogs[z] > max) {
				max = dogs[z];
			}
		}
		if (verbose)
			cout << endl;
		if (verbose)
			cout << "Most Votes: " << max << endl;

		delete[] cats;
		delete[] dogs;
		numCats = numDogs = numVotes = 0;
	}

	testFile.close();
}
