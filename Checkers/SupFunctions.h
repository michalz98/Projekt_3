/*
 * SupFunctions.h
 *
 *  Created on: 25 maj 2020
 *      Author: Michał
 */

#include <stdlib.h>
#include <iostream>
#include <list>
#include "Position.h"

#ifndef SUPFUNCTIONS_H_
#define SUPFUNCTIONS_H_

int CheckWin(list<Checker> *teamA, list<Checker> *teamB, Position *board[board_dim][board_dim]);

void ClearCaptured(list<Checker> *team);

bool IsInBorders(Vector2f mouse_pos, Position *position);

bool CheckCaptures(Position *position, Position *board[board_dim][board_dim]);

void Moves(Position *position, Position *board[board_dim][board_dim]);

void AvailableMoves(Position *position, Position *board[board_dim][board_dim]);

void DefaultColors(Position *board[board_dim][board_dim]);

void Capture(Position *move_target,Position *active);

bool TeamCanCapture(list<Checker> *team, Position *board[board_dim][board_dim]);

void ReverseCapture(Position *move_target, Position *active, Position *board[board_dim][board_dim], int player);

#endif /* SUPFUNCTIONS_H_ */
