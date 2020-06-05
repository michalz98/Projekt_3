/*
 * AI.h
 *
 *  Created on: 30 maj 2020
 *      Author: Michał
 */

#ifndef AI_H_
#define AI_H_

#include <stdlib.h>
#include <iostream>
#include <list>
#include "SupFunctions.h"

using namespace sf;
using namespace std;
const int deapth_val {5};

struct AIMove {
	AIMove ();
	AIMove (int result);
	Position *move;
	Position *chosen;
	int value;
};

class AI {
private:
	int AIplayer;
	int Humanplayer;
public:
	void initialise(int HumanPlayer);
	void AImove(Position *board[board_dim][board_dim],list<Checker> *WhiteTeam,
			list<Checker> *BlackTeam,bool *sidechange ,bool *moveMade);
	AIMove PerformBestMove(Position *board[board_dim][board_dim],list<Checker> WhiteTeam,
			list<Checker> BlackTeam, int player, int deapth, int alpha, int beta);

};

#endif /* AI_H_ */

