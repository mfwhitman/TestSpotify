/*
 * Vote.h
 *
 *  Created on: 14/12/2013
 *      Author: michael
 */

#ifndef VOTE_H_
#define VOTE_H_

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

bool checkVote(const std::string, const int, const int, Pet&, int&, int&);

void writeTest(std::string);

void runTest(std::string, bool);

void initGraph(int, int);

void addEdge(int, int);

void breadthFirstSearch();

bool depthFirstSearch();

void hopcroftKarp();


#endif /* VOTE_H_ */
