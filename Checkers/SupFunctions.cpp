/*
 * SupFunctions.cpp
 *
 *  Created on: 25 maj 2020
 *      Author: Michał
 */

#include "SupFunctions.h"

int CheckWin(list<Checker> *teamA, list<Checker> *teamB, Position *board[board_dim][board_dim])
{
	int black_num {0}, white_num {0};
	for(int i = 0; i < board_dim; i++)
	{
		for(int j = 0; j < board_dim; j++)
		{
			if(board[i][j]->IsOccupied())
			{
				if(board[i][j]->GetOccupant()->GetTeam() == white)
				{
					white_num ++;
				}
				if(board[i][j]->GetOccupant()->GetTeam() == black)
				{
					black_num++;
				}
			}
		}
	}
	int resultA, resultB;
	if(black_num == 0)
	{
		return white;
	}
	else if(white_num == 0)
	{
		return black;
	}
	resultA = white;
	resultB = black;
	for(int i = 0; i < board_dim; i++)
	{
		for(int j = 0; j < board_dim; j++)
		{
			//cout<< i << " " << j <<endl;
			if(board[i][j]->IsOccupied())
			{
				Moves(board[i][j], board);
				//cout << "WIN" << endl;
				if(board[i][j]->GetOccupant()->GetTeam() == white)
				{
					if(board[i][j]->MovesNum() != 0)
					{
						resultA = 100;
					}
				}
				if(board[i][j]->GetOccupant()->GetTeam() == black)
				{
					if(board[i][j]->MovesNum() != 0)
					{
						resultB = 100;
					}
				}
			}
		}
	}

	if(resultA != white && resultB == black)
	{
		return white;
	}
	if(resultB != black && resultA == white)
	{
		return black;
	}
	return 100;
}

void ClearCaptured(list<Checker> *team)
{
	if(team->size() != 0)
	{
		Checker tmp(white,0,0);
		tmp.SetStatus(captured);
		team->remove(tmp);
	}
}

bool IsInBorders(Vector2f mouse_pos, Position *position)
{
	float x = mouse_pos.x;
	float y = mouse_pos.y;

	if (x >= position->GetCenterX() && x <= position->GetCenterX() + b_dim && y >= position->GetCenterY() && y <= position->GetCenterY() + b_dim)
	{
		return true;
	}
	return false;
}

bool CheckCaptures(Position *position, Position *board[board_dim][board_dim])
{
	for(int i = 0; i < board_dim; i++)
	{
		for(int j = 0; j < board_dim; j++)
		{

					// ................MAN................. //

			if(position->GetOccupant()->CheckStatus() == normal
					&& board[i][j]->GetCenterX() == position->GetCenterX()
					&& board[i][j]->GetCenterY() == position->GetCenterY())
			{
				if(i+1 <board_dim && j+1 < board_dim )
				{
					if(board[i+1][j+1]->IsOccupied() && board[i+1][j+1]->GetOccupant()->GetTeam() != position->GetOccupant()->GetTeam())
					{
						if(i+2 <board_dim && j+2 < board_dim )
						{
							if(!board[i+2][j+2]->IsOccupied())
							{
								position->ToCapture(board[i+1][j+1]);
								position->AddMove(board[i+2][j+2]);
							}
						}
					}
				}
				if(i+1 <board_dim && j-1 >= 0 )
				{
					if(board[i+1][j-1]->IsOccupied()&& board[i+1][j-1]->GetOccupant()->GetTeam() != position->GetOccupant()->GetTeam())
					{
						if(i+2 <board_dim && j-2 >= 0 )
						{
							if(!board[i+2][j-2]->IsOccupied())
							{
								position->ToCapture(board[i+1][j-1]);
								position->AddMove(board[i+2][j-2]);
							}
						}
					}
				}
				if(i-1 >= 0 && j-1 >= 0 )
				{
					if(board[i-1][j-1]->IsOccupied()&& board[i-1][j-1]->GetOccupant()->GetTeam() != position->GetOccupant()->GetTeam())
					{
						if(i-2 >= 0 && j-2 >= 0 )
						{
							if(!board[i-2][j-2]->IsOccupied())
							{
								position->ToCapture(board[i-1][j-1]);
								position->AddMove(board[i-2][j-2]);
							}
						}
					}
				}
				if(i-1 >= 0 && j+1 < board_dim )
				{
					if(board[i-1][j+1]->IsOccupied()&& board[i-1][j+1]->GetOccupant()->GetTeam() != position->GetOccupant()->GetTeam())
					{
						if(i-2 >= 0 && j+2 < board_dim )
						{
							if(!board[i-2][j+2]->IsOccupied())
							{
								position->ToCapture(board[i-1][j+1]);
								position->AddMove(board[i-2][j+2]);
							}
						}
					}
				}
			}

						// ...............KING................ //

			else if(position->GetOccupant()->CheckStatus() == king
					&& board[i][j]->GetCenterX() == position->GetCenterX()
					&& board[i][j]->GetCenterY() == position->GetCenterY())
			{
				//cout << "KING CAptures" << endl;
				int x {1};
				while(i+x <board_dim && j+x < board_dim )
				{
					if(board[i+x][j+x]->IsOccupied() && board[i+x][j+x]->GetOccupant()->GetTeam() != position->GetOccupant()->GetTeam())
					{
						if(i+(x+1) <board_dim && j+(x+1) <board_dim)
						{
							if(board[i+(x+1)][j+(x+1)]->IsOccupied())
							{
								break;
							}
							else if(!board[i+(x+1)][j+(x+1)]->IsOccupied())
							{
								position->ToCapture(board[i+(x)][j+(x)]);
								position->AddMove(board[i+(x+1)][j+(x+1)]);
								break;
							}
						}
					}
					x++;
				}

				x = 1;
				while(i+x <board_dim && j-x >= 0 )
				{
					if(board[i+x][j-x]->IsOccupied() && board[i+x][j-x]->GetOccupant()->GetTeam() != position->GetOccupant()->GetTeam())
					{
						if(i+(x+1) <board_dim && j-(x+1) >= 0)
						{
							if(board[i+(x+1)][j-(x+1)]->IsOccupied())
							{
								break;
							}
							if(!board[i+(x+1)][j-(x+1)]->IsOccupied())
							{
								position->ToCapture(board[i+(x)][j-(x)]);
								position->AddMove(board[i+(x+1)][j-(x+1)]);
								break;
							}
						}
					}
					x++;
				}

				x = 1;
				while(i-x >= 0 && j+x < board_dim )
				{
					if(board[i-x][j+x]->IsOccupied() && board[i-x][j+x]->GetOccupant()->GetTeam() != position->GetOccupant()->GetTeam())
					{
						if(i-(x+1) >= 0 && j+(x+1) < board_dim)
						{
							if(board[i-(x+1)][j+(x+1)]->IsOccupied())
							{
								break;
							}
							if(!board[i-(x+1)][j+(x+1)]->IsOccupied())
							{
								position->ToCapture(board[i-(x)][j+(x)]);
								position->AddMove(board[i-(x+1)][j+(x+1)]);
								break;
							}
						}
					}
					x++;
				}

				x = 1;
				while(i-x >= 0 && j-x >= 0 )
				{
					if(board[i-x][j-x]->IsOccupied() && board[i-x][j-x]->GetOccupant()->GetTeam() != position->GetOccupant()->GetTeam())
					{
						if(i-(x+1) >= 0 && j-(x+1) >= 0)
						{
							if(board[i-(x+1)][j-(x+1)]->IsOccupied())
							{
								break;
							}
							if(!board[i-(x+1)][j-(x+1)]->IsOccupied())
							{
								position->ToCapture(board[i-(x)][j-(x)]);
								position->AddMove(board[i-(x+1)][j-(x+1)]);
								break;
							}
						}
					}
					x++;
				}
			}
		}
	}
	if(position->CaptureNum() != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Moves(Position *position, Position *board[board_dim][board_dim])
{
	position->ClearMoves();
	position->ClearToCapture();

	for(int i = 0; i < board_dim; i++)
	{
		for(int j = 0; j < board_dim; j++)
		{
			if(board[i][j]->GetCenterX() == position->GetCenterX() && board[i][j]->GetCenterY() == position->GetCenterY())
			{

						// .............MAN............. //

				if(position->GetOccupant()->CheckStatus() == normal
							&& board[i][j]->GetCenterX() == position->GetCenterX()
							&& board[i][j]->GetCenterY() == position->GetCenterY())
				{
					if(CheckCaptures(position, board))
					{
						return;
					}
					else if(position->GetOccupant()->GetTeam() == white)
					{
						if(i+1 < board_dim && j+1 < board_dim )
						{
							if(!(board[i+1][j+1]->IsOccupied()))
							{
								position->AddMove(board[i+1][j+1]);
							}
						}

						if(i+1 <board_dim && j-1 >=0 )
						{
							if(!(board[i+1][j-1]->IsOccupied()))
							{
								position->AddMove(board[i+1][j-1]);
							}
						}
					}
					else if(position->GetOccupant()->GetTeam() == black)
					{
						if(i-1 >=0 && j+1 < board_dim )
						{
							if(!(board[i-1][j+1]->IsOccupied()))
							{
								position->AddMove(board[i-1][j+1]);
							}
						}
						if(i-1 >=0 && j-1 >= 0 )
						{
							if(!(board[i-1][j-1]->IsOccupied()))
							{
								position->AddMove(board[i-1][j-1]);
							}
						}
					}
				}

						// ..............KING.............. //

				else if(position->GetOccupant()->CheckStatus() == king
						&& board[i][j]->GetCenterX() == position->GetCenterX()
						&& board[i][j]->GetCenterY() == position->GetCenterY())
				{
					int supCounter {0};
					if(CheckCaptures(position, board))
					{
						return;
					}
					else
					{
						int x {1};
						while(i+x <board_dim && j+x < board_dim )
						{
							if(!(board[i+x][j+x]->IsOccupied()) && supCounter == 0)
							{
								position->AddMove(board[i+x][j+x]);
							}
							else
							{
								supCounter++;
							}
							x++;
						}

						x=1;
						supCounter = 0;
						while(i+x <board_dim && j-x >= 0 )
						{
							if(!(board[i+x][j-x]->IsOccupied()) && supCounter == 0)
							{
								position->AddMove(board[i+x][j-x]);
							}
							else
							{
								supCounter++;
							}
							x++;
						}

						x=1;
						supCounter = 0;
						while(i-x >= 0 && j+x < board_dim )
						{
							if(!(board[i-x][j+x]->IsOccupied()) && supCounter == 0)
							{
								position->AddMove(board[i-x][j+x]);
							}
							else
							{
								supCounter++;
							}
							x++;
						}

						x=1;
						supCounter = 0;
						while(i-x >= 0 && j-x >= 0 )
						{
							if(!(board[i-x][j-x]->IsOccupied()) && supCounter == 0)
							{
								position->AddMove(board[i-x][j-x]);
							}
							else
							{
								supCounter++;
							}
							x++;
						}
					}
				}
			}
		}
	}
}

void AvailableMoves(Position *position, Position *board[board_dim][board_dim])
{
	Moves(position,board);
	for(int i = 0; i < board_dim; i++)
	{
		for(int j = 0; j < board_dim; j++)
		{
			if(position->GetMove(board[i][j])!= NULL)
			{
				board[i][j]->ChangeColor(green);
			}
		}
	}
}

void Capture(Position *move_target, Position *active)
{
	float x, y;
	if(move_target->GetCenterX() - active->GetCenterX() < 0)
	{
		x = move_target->GetCenterX() + t_dim;
	}
	else
	{
		x = move_target->GetCenterX() - t_dim;
	}
	if(move_target->GetCenterY() - active->GetCenterY() < 0)
	{
		y = move_target->GetCenterY() + t_dim;
	}
	else
	{
		y = move_target->GetCenterY() - t_dim;
	}
	active->CaptureTarget(x,y)->GetOccupant()->SetStatus(captured);
	active->CaptureTarget(x,y)->SetOccupant(NULL);
	active->CaptureTarget(x,y)->SetOccupation(false);
	active->CaptureTarget(x,y)->ClearToCapture();
}

void ReverseCapture(Position *move_target, Position *active, Position *board[board_dim][board_dim], int player)
{
	float x, y;
	int i {0}, j {0};
	if(move_target->GetCenterX() - active->GetCenterX() < 0)
	{
		x = move_target->GetCenterX() + t_dim;
	}
	else
	{
		x = move_target->GetCenterX() - t_dim;
	}
	if(move_target->GetCenterY() - active->GetCenterY() < 0)
	{
		y = move_target->GetCenterY() + t_dim;
	}
	else
	{
		y = move_target->GetCenterY() - t_dim;
	}

	if(player == white)
	{
		Checker tmp(white,x, y);
		i = (int)y/t_dim;
		j = (int)x/t_dim;
		board[i][j]->SetOccupant(&tmp);
		board[i][j]->SetOccupation(true);
	}
	if(player == black)
	{
		Checker tmp(black,x, y);
		i = (int)y/t_dim;
		j = (int)x/t_dim;
		board[i][j]->SetOccupant(&tmp);
		board[i][j]->SetOccupation(true);
	}
	cout << "reversed:" <<board[i][j]->GetCenterY() << " " << board[i][j]->GetCenterX() << " t: " <<board[i][j]->GetOccupant()->GetTeam() << endl;
}

void DefaultColors(Position *board[board_dim][board_dim])
{
	for(int i = 0; i < board_dim; i++)
	{
		for(int j = 0; j < board_dim; j++)
		{
			board[i][j]->ChangeColor((i+j)%2);
		}
	}
}

bool TeamCanCapture(list<Checker> *team, Position *board[board_dim][board_dim])
{
	for(int i = 0; i < board_dim; i++)
	{
		for(int j = 0; j <board_dim; j++)
		{
			for(auto& man: *team)
			{
				if(board[i][j]->IsOccupied())
				{
					if(board[i][j]->GetOccupant()->GetTeam() == man.GetTeam())
					{
						if(board[i][j]->CanCapture())
						{
							return true;
						}
					}
				}
			}

		}
	}
	return false;
}
