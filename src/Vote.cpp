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

const int MAXN1 = 10000;
const int MAXN2 = 10000;
const int MAXM = 50000;

enum Pet {cat, dog};

class Graph {
public:
	int n1, n2, edges, last[MAXN1];
	int previous[MAXM];
	int head[MAXM];
	int matching[MAXN2], dist[MAXN1], queue[MAXN1];
	bool used[MAXN1], visited[MAXN1];

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

void writeTest(string voteFile, int _cases, int _cats, int _dogs, int _votes) {
	int numCases = rand() % _cases + 1;

	ofstream writeFile(voteFile);

	if (!writeFile.is_open()) {
		cout << "Unable to open file." << endl;
		return;
	}

	writeFile << numCases << endl;

	for (int i = 0; i < numCases; i++)
	{
		int numCats = rand() % _cats + 1;
		int numDogs = rand() % _dogs + 1;
		int numVotes = rand() % _votes + 1;
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

void runTest(std::istream& testFile, bool verbose = false) {

	//if (!testFile.is_open()) {
		//cout << "Unable to open file." << endl;
		//return;
	//}

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

		testFile.ignore();

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

		int edgecount = 0;

		for (std::set<Vote>::iterator left = catbox.begin(); left != catbox.end(); ++left) {
			for (std::set<Vote>::iterator right = dogbox.begin(); right != dogbox.end(); ++right) {
				if (left->isIncompatible(*right)) {
					int cattemp = left->id;
					int dogtemp = right->id;
					if (verbose)
						cout << "Edge " <<edgecount++ << ": Cat vote " << cattemp << ", Dog vote " << dogtemp << endl;
					testcase.addEdge(cattemp, dogtemp);
				}
			}
		}

		std::cout << (numVotes - testcase.hopcroftKarp()) << std::endl;

		numCats = numDogs = numVotes = 0;
	}

	//testFile.close();
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

void Graph::breadthFirstSearch() {		//finds free vertices, then
    fill(dist, dist + n1, -1);			//reset distances to infinity
    int queueSize = 0;					//initialize queue
    for (int u = 0; u < n1; ++u) {		//for each vertex in the U partition,
        if (!used[u]) {					//if it's not in the partial matching,
            queue[queueSize++] = u;		//add it to the queue
            dist[u] = 0;				//reset its distance to 0. it's a starting point.
        }
    }
    for (int i = 0; i < queueSize; i++) {
        int u1 = queue[i];									//for each vert in the queue,
        for (int e = last[u1]; e >= 0; e = previous[e]) {	//get edges that are adjacent to the U vert,
            int u2 = matching[head[e]];						//from that edge's V vert, find what U vert its connected to in the partial matching (otherwise ignore)
            if (u2 >= 0 && dist[u2] < 0) {					//if the u vert its distance is unmeasured, (i.e. its distance is zero)
                dist[u2] = dist[u1] + 1;					//set its distance from the starting vert in this augmenting path
                queue[queueSize++] = u2;					//and add the new U vert to the queue.
            }
        }
    }
}

bool Graph::depthFirstSearch(int u1) {						//check a U vert for viability in a augmenting path
    visited[u1] = true;										//set it as visited
    for (int e = last[u1]; e >= 0; e = previous[e]) {		//for each edge connected to the U vert,
        int v = head[e];									//get the V vertex of that edge
        int u2 = matching[v];								//get the U vertex of the V vertex's match
        if ((u2 < 0) || (!visited[u2] && (dist[u2] == dist[u1] + 1) && depthFirstSearch(u2))) {
            matching[v] = u1;					//if its a free vert. OR if its unvisited, its one hop away from the 1st U vert,
            used[u1] = true;					//AND the vert can continue an augmenting path, put the edge in the matching.
            return true;						//mark the U vert as used in the matching.
        }
    }
    return false;
}

int Graph::hopcroftKarp() {
    fill(used, used + n1, false);					//uncheck U verts from being used in the matching
    fill(matching, matching + n2, -1);				//remove all V verts from the partial match
    for (int res = 0;;) {
    	breadthFirstSearch();						//find free U verts and measure
        fill(visited, visited + n1, false);			//set all the U verts as being unvisited
        int f = 0;
        for (int u = 0; u < n1; ++u)				//for each U vert,
            if (!used[u] && depthFirstSearch(u))	//check if its not already used and test its viability for a augmenting path
                ++f;
        if (!f)										//if there are no more augmenting paths,
            return res;								//berge's lemma states that is a maximum :)
        res += f;
    }
    return 0;
}
