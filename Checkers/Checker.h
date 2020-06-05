/*
 * Checker.h
 *
 *  Created on: 21 maj 2020
 *      Author: Michał
 */

#ifndef CHECKER_H_
#define CHECKER_H_

#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>


using namespace std;
using namespace sf;

#define captured 0
#define normal 1
#define king 2

class Checker: public Drawable {
private:
	const float CheckerRadius {45.f};
	Texture texture;
	int status;
	int team;								// 0 - White, 1 - Black
	CircleShape shape;
	void draw(RenderTarget& target, RenderStates state) const override;
public:
    bool operator == (const Checker& m) const { return status == m.status; }
    bool operator != (const Checker& m) const { return !operator==(m); }
	Checker(int t_num, float c_x, float c_y);
	Checker() = delete;
	~Checker() = default;
	void Move(float x, float y);
	int GetTeam();
	int CheckStatus();
	void SetStatus(int stat);
	void GetCoordinates();
	void MakeKing();
};

#endif /* CHECKER_H_ */
