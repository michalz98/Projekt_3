/*
 * Position.cpp
 *
 *  Created on: 21 maj 2020
 *      Author: Michał
 */

#include "Position.h"


Position::Position(float x, float y, int color)
{
	this->tile.setSize(Vector2f(b_dim, b_dim));
	this->center_x = x ;
	this->center_y = y ;
	this->occupied = false;
	this->occupant = NULL;
	this->tile.setPosition(x,y);
	if (color == 0)
	{
		this->tile.setFillColor(Color:: White);
	}
	if (color == 1)
	{
		this->tile.setFillColor(Color:: Black);
	}
}

bool Position::IsOccupied()
{
	return this->occupied;
}

float Position::GetCenterX()
{
	return this->center_x;
}

float Position::GetCenterY()
{
	return this->center_y;
}

Checker *Position::GetOccupant()
{
	return this->occupant;
}

void Position::SetOccupation(bool occupation)
{
	this->occupied = occupation;
}

void Position::SetOccupant(Checker *man)
{
	this->occupant = man;
}

void Position::ChangeColor(int color)
{
	if (color == 0)
	{
		this->tile.setFillColor(Color::White);
	}

	if (color == 1)
	{
		this->tile.setFillColor(Color::Black);
	}

	if (color == 2)
	{
		this->tile.setFillColor(Color::Green);
	}

}


void Position::draw(RenderTarget& target, RenderStates state) const
{
	target.draw(this->tile, state);
}

void Position::AddMove(Position *position)
{
	this->moves.push_back(position);
}

Position *Position::GetMove(Position *position)
{
	for(auto& iter: this->moves)
	{
		if(iter == position)
		{
			return iter;
		}
	}
	return NULL;
}

Position *Position::Move(int i)
{
	return this->moves[i];
}

void Position::MoveOccupand(Position *toPosition)
{
	if(this->occupied == true)
	{
		toPosition->SetOccupant(this->GetOccupant());
		this->occupant->Move(toPosition->GetCenterX(), toPosition->GetCenterY());
		toPosition->SetOccupation(true);
		this->occupied = false;
		this->occupant = NULL;
	}
}

void Position::ToCapture(Position *position)
{
	this->toCapture.push_back(position);
}

Position *Position::CaptureTarget(float x, float y)
{
	for(vector<Position*>::iterator iter = this->toCapture.begin(); iter != this->toCapture.end();iter++)
	{
		if((*iter)->GetCenterX() == x && (*iter)->GetCenterY() == y)
		{
			return *iter;
		}
	}
	return NULL;
}

bool Position::CanCapture()
{
	if(this->toCapture.size() != 0)
	{
		return true;
	}
	return false;
}

void Position::ClearToCapture()
{
	this->toCapture.clear();
}

void Position::ClearMoves()
{
	this->moves.clear();
}

int Position::MovesNum()
{
	return (int)this->moves.size();
}

int Position::CaptureNum()
{
	return (int)this->toCapture.size();
}

void Position::showmoves()
{
	for(int x = 0; x < (int)this->moves.size(); x++)
	{
		cout << this->moves[x]->GetCenterY() << " " << this->moves[x]->GetCenterX() << endl;
	}
}
void Position::ShowCaptures()
{
	for(int x = 0; x < (int)this->toCapture.size(); x++)
	{
		cout << "toCapture: " << this->toCapture[x]->GetCenterY() << " " << this->toCapture[x]->GetCenterX() << endl;
	}
}
