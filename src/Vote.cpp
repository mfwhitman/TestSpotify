/*
 * Vote.cpp
 *
 *  Created on: 14/12/2013
 *      Author: michael
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <set>

using namespace std;

const int MAXN1 = 100;
const int MAXN2 = 100;
const int MAXM = 500;

enum Pet {cat, dog};

class Graph {
public:
	int n1, n2, edges, last[MAXN1];
	int previous[MAXM];
	int head[MAXM];
	int matching[MAXN2], dist[MAXN1], Q[MAXN1];
	bool used[MAXN1], vis[MAXN1];

	void initGraph(int, int);
	void addEdge(int, int);
	void breadthFirstSearch();
	bool depthFirstSearch(int);
	int hopcroftKarp();
};

class Vote {
public:
	int id;
	Pet type;
	int loves;
	int hates;
	Vote (int,Pet,int,int);
	bool isIncompatible (Vote) const;
	bool operator<(const Vote&) const;
	void print();
};

Vote::Vote (int _id, Pet _type, int _loves, int _hates) {
	id = _id;
	type = _type;
	loves = _loves;
	hates = _hates;
}

bool Vote::isIncompatible (Vote right) const {
	if (type == right.type) return false;
	if ((loves == right.hates) || (hates == right.loves)) return true;
	else return false;
}

bool Vote::operator<(const Vote& rhs) const {
	return id < rhs.id;
}

void Vote::print() {
	std::cout << type << " " << loves << " " << hates << std::endl;
}

bool checkVote(const string input, const int countCat, const int countDog, Pet & voteType, int & voteFor, int & voteAgainst) {
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

			voteType = cat;
			voteFor = like;
			voteAgainst = hate;
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

			voteType = dog;
			voteFor = like;
			voteAgainst = hate;
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

		Graph testcase;

		if (verbose)
			cout << endl << "Testcase no." << (i + 1) << endl;
		testFile >> numCats >> numDogs >> numVotes;
		if (verbose)
			cout << "Cats: " << numCats << endl;
		if (verbose)
			cout << "Dogs: " << numDogs << endl;
		if (verbose)
			cout << "Votes: " << numVotes << endl << endl;


		std::set<Vote> catbox, dogbox;
		int catVotes, dogVotes = 0;

		testFile.ignore(); //one more endl?

		//loop per vote *****************************************************
		for (int j = 0; j < numVotes; j++) {
			string voteline;
			getline(testFile, voteline);
			Pet voteType;
			int voteFor, voteAgainst;
			if (checkVote(voteline, numCats, numDogs, voteType, voteFor, voteAgainst))
			{
				if (verbose)
					cout << "Vote Success: " << (voteType == cat ?"cat":"dog") << " " << voteFor << " " << voteAgainst << endl;
				if (voteType == cat) catbox.insert(catbox.end(), Vote(catVotes++, voteType, voteFor, voteAgainst));
				else dogbox.insert(dogbox.end(), Vote(dogVotes++, voteType, voteFor, voteAgainst));
			}

		}

		testcase.initGraph(catVotes, dogVotes);

		for (std::set<Vote>::iterator left = catbox.begin(); left != catbox.end(); ++left) {
			for (std::set<Vote>::iterator right = dogbox.begin(); right != dogbox.end(); ++right) {
				if (left->isIncompatible(*right)) {
					int cattemp = left->id;
					int dogtemp = right->id;
					if (verbose)
						cout << "Incompatibility between Cat vote " << cattemp << " and Dog vote " << dogtemp << endl;
					testcase.addEdge(cattemp, dogtemp);
				}
			}
		}

		if (verbose)
			std::cout << "Satisfied Voters: " << (numVotes - testcase.hopcroftKarp()) << std::endl;
		std::cout << (numVotes - testcase.hopcroftKarp()) << std::endl;



		numCats = numDogs = numVotes = 0;
	}

	testFile.close();
}

void Graph::initGraph(int _n1, int _n2) {
    n1 = _n1;
    n2 = _n2;
    edges = 0;
    fill(last, last + n1, -1);
}

void Graph::addEdge(int u, int v) {
    head[edges] = v;
    previous[edges] = last[u];
    last[u] = edges++;
}

void Graph::breadthFirstSearch() {
    fill(dist, dist + n1, -1);
    int sizeQ = 0;
    for (int u = 0; u < n1; ++u) {
        if (!used[u]) {
            Q[sizeQ++] = u;
            dist[u] = 0;
        }
    }
    for (int i = 0; i < sizeQ; i++) {
        int u1 = Q[i];
        for (int e = last[u1]; e >= 0; e = previous[e]) {
            int u2 = matching[head[e]];
            if (u2 >= 0 && dist[u2] < 0) {
                dist[u2] = dist[u1] + 1;
                Q[sizeQ++] = u2;
            }
        }
    }
}

bool Graph::depthFirstSearch(int u1) {
    vis[u1] = true;
    for (int e = last[u1]; e >= 0; e = previous[e]) {
        int v = head[e];
        int u2 = matching[v];
        if ((u2 < 0) || (!vis[u2] && (dist[u2] == dist[u1] + 1) && depthFirstSearch(u2))) {
            matching[v] = u1;
            used[u1] = true;
            return true;
        }
    }
    return false;
}

int Graph::hopcroftKarp() {
    fill(used, used + n1, false);
    fill(matching, matching + n2, -1);
    for (int res = 0;;) {
    	breadthFirstSearch();
        fill(vis, vis + n1, false);
        int f = 0;
        for (int u = 0; u < n1; ++u)
            if (!used[u] && depthFirstSearch(u))
                ++f;
        if (!f)
            return res;
        res += f;
    }
    return 0; // Unused
}
