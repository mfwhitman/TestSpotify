/*
 * Vote.h
 *
 *  Created on: 14/12/2013
 *      Author: michael
 */

#ifndef VOTE_H_
#define VOTE_H_

enum Pet {cat, dog};

class Vote {
	Pet type;
	int loves;
	int hates;
public:
	Vote (Pet,int,int);
	Vote (std::string);
};

bool checkVote(const std::string, const int, const int);

void writeTest(std::string);

void runTest(std::string, bool);


#endif /* VOTE_H_ */
