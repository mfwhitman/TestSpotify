/*
 * Vote.h
 *
 *  Created on: 14/12/2013
 *      Author: michael
 */

#ifndef VOTE_H_
#define VOTE_H_

enum Pet {cat, dog};

bool checkVote(const std::string, const int, const int, Pet&, int&, int&);

void writeTest(std::string,int,int,int,int);

void runTest(std::istream&, bool);

void initGraph(int, int);

void addEdge(int, int);

void breadthFirstSearch();

bool depthFirstSearch();

void hopcroftKarp();


#endif /* VOTE_H_ */
