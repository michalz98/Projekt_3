/*
 * Position.h
 *
 *  Created on: 21 maj 2020
 *      Author: Michał
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "Checker.h"

using namespace sf;
using namespace std;


#define white 0
#define black 1
#define green 2

const int board_dim {8};
const float t_dim {100};
const int team_size {12};

#define b_dim 100

class Position: public Drawable {
private:
	bool occupied;
	Checker *occupant;
	float center_x;
	float center_y;
	RectangleShape tile;
	vector<Position*> moves;
	vector<Position*> toCapture;
	void draw(RenderTarget& target, RenderStates state) const override;
public:
	Position(float x, float y, int color);
	Position() = delete;
	~Position() = default;
	bool IsOccupied();
	Checker *GetOccupant();
	float GetCenterX();
	float GetCenterY();
	void SetOccupation(bool occupation);
	void SetOccupant(Checker *man);
	void ChangeColor(int color);
	void AddMove(Position *position);
	Position *GetMove(Position *position);
	Position *Move(int i);
	void MoveOccupand(Position *toPosition);
	void ClearMoves();
	int MovesNum();
	int CaptureNum();
	void ToCapture(Position *position);
	Position *CaptureTarget(float x, float y);
	void ClearToCapture();
	bool CanCapture();
	void showmoves();
	void ShowCaptures();
};

#endif /* POSITION_H_ */
