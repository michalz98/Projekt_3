/*
 * mian.cpp
 *
 *  Created on: 21 maj 2020
 *      Author: Michał
 */

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Position.h"
#include "SupFunctions.h"
#include "AI.h"

using namespace std;
using namespace sf;


int main()
{
	Vector2f mouse_pos;
    vector<Position> positions;
    list<Checker> WhiteTeam;
    list<Checker> BlackTeam;
    Position *active = NULL;
    Position *board[board_dim][board_dim];
    bool moveMade {false};
    int activePlayer {white};
    int Player {white};
    bool sidechange {false};
    bool change {false};
    int gameResult;
    AI Ai;
	CircleShape outline(45.f);
	outline.setOutlineThickness(4.f);
	outline.setOutlineColor(Color::Green);
	int clickCounter {0};
	Ai.initialise(white);
	WhiteTeam.clear();
	BlackTeam.clear();

    for (int i = 0; i < board_dim; i++)
    {
    	for (int j = 0; j < board_dim; j++)
    	{
    		positions.emplace_back((j)*(t_dim), (i)*(t_dim), (i+j)%2);
    	}
    }

    int row {0}, col {0};
    for(int i = 0 ; i< board_dim*board_dim; i++)
    {
    	Position *position;
    	position = &positions[i];
    	board[row][col] = position;
		col++;
		if(col >= board_dim)
		{
			row++;
			col = 0;
		}
 	}


    int num {0}, it{0};

    for(int i = 0 ; i< board_dim; i++)
    {
    	for(int j = 0; j < board_dim ; j++)
    	{
			Position *position;
			position = &positions[it];
			if ( ((i+j)%2) == 1 && num < team_size)
			{
				WhiteTeam.emplace_back(white,position->GetCenterX(), position->GetCenterY());
				position->SetOccupant(&WhiteTeam.back());
				position->SetOccupation(true);
				num++;
			}
			it++;
		}
	}

    num = 0;
    it = 0;

    for(int i = 0 ; i< board_dim; i++)
    {
    	for(int j = 0; j < board_dim ; j++)
    	{
			Position *position;
			position = &positions[it];

			if ( (i+j)%2 == 1 && i >= board_dim - 3 && num < team_size)
			{
				BlackTeam.emplace_back(black,position->GetCenterX(), position->GetCenterY());
				position->SetOccupant(&BlackTeam.back());
				position->SetOccupation(true);
				num++;
			}
			it++;
    	}

	}
    char opt = 'o';
    while(opt != 's')
    {
    	switch(opt){
    	case 'o':
    		cout << "ZASADY GRY" << endl;
    		cout << "Bicia sa obowiazkowe" << endl;
    		cout << "Nie ma limitu bic pod rzad" << endl;
    		cout << "Mozna bic wstecz" << endl;
    		cout << "Aby zaczac nacisnij 's' "<< endl;
    		cin >> opt;
    		if(opt != 's'){
    			opt = 'o';
    		}
    		break;
    	}
    }
    if(opt == 's'){
    RenderWindow window(VideoMode(800, 800), "Checkers");
    window.setFramerateLimit(60);
    int won_cntr {0};
    while (window.isOpen())
    {


    	// ...........Check for WIN............ //

    	gameResult = CheckWin(&BlackTeam, &WhiteTeam, board);
    	if(gameResult == white)
    	{
    		if(won_cntr == 0){
    			cout << "White team won" <<endl;
    			won_cntr++;
    			break;
    		}
    	}
    	if (gameResult == black)
    	{
    		if(won_cntr == 0){
    			cout << "Black team won" <<endl;
    			won_cntr++;
    			break;
    		}
    	}

    	// ...........Mouse position registration............ //

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if(event.type == Event::MouseMoved)
            {
            	mouse_pos.x = (float)event.mouseMove.x;
            	mouse_pos.y = (float)event.mouseMove.y;
            }

        }


        window.clear(Color::Black);

    				// ..........Check Captures..........//

        for (auto &position: positions)
        {
        	window.draw(position);
    		if(position.IsOccupied())
    		{
    			CheckCaptures(&position,board);
    		}


    				// ..........Chose man..............//
    		if(activePlayer == Player)
    		{
				if (Player == white)
				{
					if(IsInBorders(mouse_pos, &position) && position.IsOccupied() && position.GetOccupant()->GetTeam() == activePlayer)
					{
						if(TeamCanCapture(&WhiteTeam, board))
						{
							if(position.CanCapture())
							{
								outline.setPosition(position.GetCenterX()+5, position.GetCenterY()+5);
								window.draw(outline);

								if(Mouse::isButtonPressed(Mouse::Left) && clickCounter == 0)
								{

									position.ClearMoves();
									DefaultColors(board);

									position.ChangeColor(green);
									active = &position;
									AvailableMoves(&position, board);

								}

								if(event.type == Event::MouseButtonReleased && clickCounter == 1)
								{
									clickCounter = 0;
								}
							}
						}
						else
						{
							outline.setPosition(position.GetCenterX()+5, position.GetCenterY()+5);
							window.draw(outline);

							if(Mouse::isButtonPressed(Mouse::Left) && clickCounter == 0)
							{
								clickCounter++;
								position.ClearMoves();
								DefaultColors(board);

								position.ChangeColor(green);
								active = &position;
								AvailableMoves(&position, board);

							}

							if(event.type == Event::MouseButtonReleased && clickCounter == 1)
							{
								sidechange = true;
								clickCounter = 0;
							}
						}
					}
				}

				if(Player == black)
				{
					if(IsInBorders(mouse_pos, &position) && position.IsOccupied() && position.GetOccupant()->GetTeam() == activePlayer)
					{
						if(TeamCanCapture(&BlackTeam, board))
						{
							if(position.CanCapture())
							{
								outline.setPosition(position.GetCenterX()+5, position.GetCenterY()+5);
								window.draw(outline);

								if(Mouse::isButtonPressed(Mouse::Left) && clickCounter == 0)
								{
									clickCounter++;

									position.ClearMoves();
									DefaultColors(board);

									position.ChangeColor(green);
									active = &position;
									AvailableMoves(active, board);

								}

								if(event.type == Event::MouseButtonReleased && clickCounter == 1)
								{
									clickCounter = 0;
								}
							}
						}
						else
						{
							outline.setPosition(position.GetCenterX()+5, position.GetCenterY()+5);
							window.draw(outline);

							if(Mouse::isButtonPressed(Mouse::Left) && clickCounter == 0)
							{
								clickCounter++;

								position.ClearMoves();
								DefaultColors(board);

								position.ChangeColor(green);
								active = &position;
								AvailableMoves(active, board);

							}

							if(event.type == Event::MouseButtonReleased && clickCounter == 1)
							{
								sidechange = true;
								clickCounter = 0;
							}
						}
					}
				}

							// ........... Move MAN............ //

				if(IsInBorders(mouse_pos, &position) && !position.IsOccupied() && active != NULL)
				{
					if(Mouse::isButtonPressed(Mouse::Left) && clickCounter == 0)
					{
						clickCounter++;
						if(active->GetMove(&position) != NULL)
						{
							if(!active->CanCapture())
							{
								sidechange = true;
							}
							active->MoveOccupand(&position);
							if(active->CanCapture())
							{
								sidechange = false;
								Capture(&position,active);
								for(auto &itr: positions)
								{
									itr.ClearToCapture();
								}
							}
							active = &position;
							CheckCaptures(active,board);
							if(!active->CanCapture())
							{
								moveMade = true;
							}
							else
							{
								moveMade = false;
							}

							DefaultColors(board);
							if((moveMade && sidechange) || (sidechange && !moveMade) || (!sidechange && moveMade))
							{
								change = true;
							}
						}
						position.ClearMoves();

					}
					if(event.type == Event::MouseButtonReleased && clickCounter == 1)
					{
						clickCounter = 0;
					}
				}
    		}

        }//for(positions)

		if(activePlayer != Player)
		{
			Ai.AImove(board, &WhiteTeam, &BlackTeam, &sidechange, &moveMade);
			if((moveMade && sidechange) || (sidechange && !moveMade) || (!sidechange && moveMade))
			{
				change = true;
			}

		}

		if(change)
		{

				if(activePlayer == white)
				{
					activePlayer = black;
				}
				else
				{
					activePlayer = white;
				}
				active = NULL;
				moveMade = false;
				change = false;

		}

    	for (auto &position: positions)
    	{
    				// .......... Make KINGS............. //
    		if(position.IsOccupied() && position.GetOccupant()->CheckStatus() == normal)
    		{
				if(position.GetOccupant()->GetTeam() == white )
				{
					if(position.GetCenterY() == 700)
					{
						position.GetOccupant()->SetStatus(king);
						position.GetOccupant()->MakeKing();
					}
				}
				if(position.GetOccupant()->GetTeam() == black )
				{
					if(position.GetCenterY() == 0)
					{
						position.GetOccupant()->SetStatus(king);
						position.GetOccupant()->MakeKing();
					}
				}
    		}
    	}

		ClearCaptured(&WhiteTeam);
		ClearCaptured(&BlackTeam);
    	if(WhiteTeam.size() !=0 )
    	{
        	for (auto &man: WhiteTeam)
        	{
        		window.draw(man);
        	}
    	}

    	if(BlackTeam.size() !=0 )
    	{
        	for (auto &man: BlackTeam)
        	{
        		window.draw(man);
        	}
    	}


        window.display();
    }
    }

    return 0;
}

