/*
 * Checker.cpp
 *
 *  Created on: 21 maj 2020
 *      Author: Michał
 */

#include "Checker.h"

Checker::Checker(int t_num, float c_x, float c_y)
{
	this->team = t_num;
	this->status = normal;
	this->shape.setRadius(this->CheckerRadius);
	this->shape.setPosition(c_x + this->CheckerRadius+5, c_y + this->CheckerRadius+5);
	this->shape.setOrigin(this->CheckerRadius, this->CheckerRadius);
	if (this->team == 0)
	{
		this->shape.setFillColor(Color::Yellow);
	}
	if (this->team == 1)
	{
		this->shape.setFillColor(Color::Red);
	}
}

int Checker::GetTeam()
{
	return this->team;
}

void Checker::Move(float x, float y)
{
	this->shape.setPosition(x+ this->CheckerRadius+5, y+ this->CheckerRadius+5);
}

int Checker::CheckStatus()
{
	return this->status;
}

void Checker::SetStatus(int stat)
{
	this->status = stat;
}

void Checker::GetCoordinates()
{
	Vector2f xy = this->shape.getPosition();
	cout << xy.y - this->CheckerRadius-5 << " " << xy.x - this->CheckerRadius-5<< " t: "<< this->team <<endl;
}

void Checker::draw(RenderTarget& target, RenderStates state) const
{
	target.draw(this->shape, state);
}

void Checker::MakeKing(){
	if(this->team == 0){
		if(this->texture.loadFromFile("white_king.png")){
			this->shape.setTexture(&texture);
		}
	} else 	if(this->team == 1){
		if(this->texture.loadFromFile("black_king.png")){
			this->shape.setTexture(&texture);
		}
	}

}
