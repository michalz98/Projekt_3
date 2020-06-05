/*
 * AI.cpp
 *
 *  Created on: 30 maj 2020
 *      Author: Michał
 */

#include "AI.h"

AIMove::AIMove() {
	move = NULL;
	chosen = NULL;
	value = 0;
}

AIMove::AIMove(int result) {
	this->value = result;
	this->chosen = NULL;
	this->move = NULL;
}

void AI::initialise(int HumanPlayer) {
	if (HumanPlayer == white) {
		AIplayer = black;
		Humanplayer = white;
	} else if (HumanPlayer == black) {
		AIplayer = white;
		Humanplayer = black;
	}
}

// perform a move //

void AI::AImove(Position *board[board_dim][board_dim], list<Checker> *WhiteTeam,
		list<Checker> *BlackTeam, bool *sidechange, bool *moveMade) {

	int alpha = -10000;
	int beta = 10000;
	list<Checker> teamW(*WhiteTeam);
	list<Checker> teamB(*BlackTeam);
	AIMove bestMove = PerformBestMove(board, teamW, teamB, AIplayer,deapth_val + 1, alpha, beta);

	bestMove.chosen->ClearMoves();
	bestMove.chosen->ClearToCapture();
	if (bestMove.chosen->GetOccupant()->GetTeam() == white) {
		if (bestMove.chosen->GetCenterY() == 700) {
			bestMove.chosen->GetOccupant()->SetStatus(king);
			bestMove.chosen->GetOccupant()->MakeKing();
		}
	} else {
		if (bestMove.chosen->GetCenterY() == 0) {
			bestMove.chosen->GetOccupant()->SetStatus(king);
			bestMove.chosen->GetOccupant()->MakeKing();
		}

	}

	CheckCaptures(bestMove.chosen, board);
	if (!bestMove.chosen->CanCapture()) {
		*sidechange = true;
	}

	if (bestMove.chosen->CanCapture()) {
		*sidechange = false;
		Capture(bestMove.move, bestMove.chosen);

		for (int i = 0; i < board_dim; i++) {
			for (int j = 0; j < board_dim; j++) {
				board[i][j]->ClearToCapture();
			}
		}
	}
	bestMove.chosen->MoveOccupand(bestMove.move);

	CheckCaptures(bestMove.move, board);
	if (!bestMove.move->CanCapture()) {
		*moveMade = true;
	} else {
		*moveMade = false;
	}
	teamW.clear();
	teamB.clear();
}

// Calculate best move for AI //

AIMove AI::PerformBestMove(Position *board_real[board_dim][board_dim],
		list<Checker> WhiteTeam, list<Checker> BlackTeam, int player,
		int depth, int alpha, int beta) {

	depth--;
	Position *board_sim[board_dim][board_dim];
	Checker *tmp = NULL;
	int moves_num, gr;
	int bonus {0};
	bool captr { false };
	bool Isking_p { false };
	bool Isking_c { false };
	bool TeamCanCap { false };
	int black_num { 0 }, white_num { 0 };

	vector<AIMove> moves;
	bool sidechange { false }, moveMade { false }, change { false };

	for (int i = 0; i < board_dim; i++) {
		for (int j = 0; j < board_dim; j++) {
			board_sim[i][j] = board_real[i][j];
		}
	}

	gr = CheckWin(&BlackTeam, &WhiteTeam, board_sim);

	if (gr == AIplayer) {
		return AIMove(50 + (deapth_val - depth));
	} else if (gr == Humanplayer) {
		return AIMove(-50 - (deapth_val - depth));
	}
	AIMove score;
	if (depth == 0) {
		for (int i = 0; i < board_dim; i++) {
			for (int j = 0; j < board_dim; j++) {
				if (board_sim[i][j]->IsOccupied()) {
					if (board_sim[i][j]->GetOccupant()->GetTeam() == white) {
						white_num++;
					}
					if (board_sim[i][j]->GetOccupant()->GetTeam() == black) {
						black_num++;
					}
				}
			}
		}
		if (AIplayer == white) {
			return AIMove(white_num - black_num);
		}
		if (AIplayer == black) {
			return AIMove(black_num - white_num);
		}
	}

			// Get best move //
	for (int i = 0; i < board_dim; i++) {
		for (int j = 0; j < board_dim; j++) {
			if (player == white) {
				if (TeamCanCapture(&WhiteTeam, board_sim)) {
					TeamCanCap = true;
				}
			} else if (player == black) {
				if (TeamCanCapture(&BlackTeam, board_sim)) {
					TeamCanCap = true;
				}
			}
			if (TeamCanCap) {
				if (board_sim[i][j]->IsOccupied() && board_sim[i][j]->GetOccupant()->GetTeam() == player) {

					Moves(board_sim[i][j], board_sim);
					if (board_sim[i][j]->MovesNum() != 0
							&& board_sim[i][j]->CanCapture()) {

								// ..........Chose man............//

						AIMove aimove;
						aimove.chosen = board_sim[i][j];
						moves_num = aimove.chosen->MovesNum();

							// ........... Move man............ //

						for (int x = 0; x < moves_num; x++) {
							if(alpha < beta){
								aimove.chosen->ClearMoves();
								Moves(aimove.chosen, board_sim);
								moves_num = aimove.chosen->MovesNum();
								if (!aimove.chosen->Move(x)->IsOccupied()) {

									if (aimove.chosen->GetOccupant()->CheckStatus() == king) {
										Isking_p = true;
									}
									aimove.move = aimove.chosen->Move(x);
									if (!aimove.chosen->CanCapture())
									{
										sidechange = true;
									}
									aimove.chosen->MoveOccupand(aimove.move);
									if (aimove.chosen->CanCapture()) {
										sidechange = false;
										float x_val, y_val;
										if (aimove.move->GetCenterX()
												- aimove.chosen->GetCenterX() < 0) {
											x_val = aimove.move->GetCenterX()+ t_dim;
										} else {
											x_val = aimove.move->GetCenterX()- t_dim;
										}
										if (aimove.move->GetCenterY()
												- aimove.chosen->GetCenterY() < 0) {
											y_val = aimove.move->GetCenterY()+ t_dim;
										} else {
											y_val = aimove.move->GetCenterY()- t_dim;
										}

										int row = (int) y_val / t_dim;
										int col = (int) x_val / t_dim;
										tmp = board_sim[row][col]->GetOccupant();
										if (board_sim[row][col]->GetOccupant()->CheckStatus() == king) {
											Isking_c = true;
											bonus = 10;
										}
										board_sim[row][col]->GetOccupant()->SetStatus(captured);
										board_sim[row][col]->SetOccupant(NULL);
										board_sim[row][col]->SetOccupation(false);
										captr = true;
									}

									CheckCaptures(aimove.move, board_sim);
									if (!aimove.move->CanCapture()) {
										moveMade = true;
									} else {
										moveMade = false;
									}
									if ((moveMade && sidechange)
											|| (sidechange && !moveMade)
											|| (!sidechange && moveMade)) {
										change = true;
									}
									if((player == black && aimove.move->GetCenterY() == 0 )
											|| (player == white && aimove.move->GetCenterY() == 700 )){
										bonus = 20;
									}
									if (change) {
										if (player == AIplayer) {
											aimove.value = PerformBestMove(board_sim, WhiteTeam, BlackTeam,
													Humanplayer, depth, alpha, beta).value + bonus;
											if(aimove.value > alpha)
											{
												alpha = aimove.value;
											}
										} else {
											aimove.value = PerformBestMove(board_sim, WhiteTeam, BlackTeam,
													AIplayer, depth, alpha, beta).value - bonus;
											if(aimove.value < beta)
											{
												beta = aimove.value;
											}
										}
										moveMade = false;
										change = false;
									} else {
										if (player == AIplayer) {
											aimove.value = PerformBestMove(board_sim, WhiteTeam, BlackTeam,
													AIplayer, depth, alpha, beta).value + bonus;
											if(aimove.value > alpha)
											{
												alpha = aimove.value;
											}
										} else {
											aimove.value = PerformBestMove(board_sim, WhiteTeam, BlackTeam,
													Humanplayer, depth, alpha, beta).value - bonus;
											if(aimove.value < beta)
											{
												beta = aimove.value;
											}
										}
										moveMade = false;
										change = false;
									}

									moves.push_back(aimove);
									if (captr) {
										float x_val, y_val;
										if (aimove.move->GetCenterX()- aimove.chosen->GetCenterX() < 0) {
											x_val = aimove.move->GetCenterX()+ t_dim;
										} else {
											x_val = aimove.move->GetCenterX()- t_dim;
										}
										if (aimove.move->GetCenterY()- aimove.chosen->GetCenterY() < 0) {
											y_val = aimove.move->GetCenterY()+ t_dim;
										} else {
											y_val = aimove.move->GetCenterY()- t_dim;
										}

										int row = (int) y_val / t_dim;
										int col = (int) x_val / t_dim;
										board_sim[row][col]->SetOccupant(tmp);
										board_sim[row][col]->SetOccupation(true);

										if (Isking_c) {
											board_sim[row][col]->GetOccupant()->SetStatus(king);
										} else {
											board_sim[row][col]->GetOccupant()->SetStatus(normal);
										}
										captr = false;
									}
									aimove.move->MoveOccupand(aimove.chosen);

									if (!Isking_p) {
										aimove.chosen->GetOccupant()->SetStatus(normal);
									}
								}
							}
						}
					}
				} //isOccupied
			} else {
				if (board_sim[i][j]->IsOccupied() && board_sim[i][j]->GetOccupant()->GetTeam() == player) {
					bool correct {false};
					Moves(board_sim[i][j], board_sim);
					if (board_sim[i][j]->MovesNum() != 0) {
						AIMove aimove;

							// ..........Chose man............//
						aimove.chosen = board_sim[i][j];
						moves_num = aimove.chosen->MovesNum();

						// ........... Move MAN............ //
						for (int x = 0; x < moves_num; x++) {
							if(alpha < beta){
								aimove.chosen->ClearMoves();
								Moves(aimove.chosen, board_sim);
								moves_num = aimove.chosen->MovesNum();
								if (!aimove.chosen->Move(x)->IsOccupied()) {

									if (aimove.chosen->GetOccupant()->CheckStatus() == king) {
										Isking_p = true;
									}
									aimove.move = aimove.chosen->Move(x);
									if(player == white)
									{
										if(!Isking_p && aimove.move->GetCenterY() > aimove.chosen->GetCenterY()){
											correct = true;
										}
									}
									if(player == black)
									{
										if((!Isking_p && aimove.move->GetCenterY() < aimove.chosen->GetCenterY()) || Isking_p){
											correct = true;
										}
									}

									if(correct){
										if (!aimove.chosen->CanCapture())
										{
											sidechange = true;
										}
										aimove.chosen->MoveOccupand(aimove.move);
										if (aimove.chosen->CanCapture()) {
											sidechange = false;
											float x_val, y_val;
											if (aimove.move->GetCenterX()
													- aimove.chosen->GetCenterX() < 0) {
												x_val = aimove.move->GetCenterX()
														+ t_dim;
											} else {
												x_val = aimove.move->GetCenterX()
														- t_dim;
											}
											if (aimove.move->GetCenterY()
													- aimove.chosen->GetCenterY() < 0) {
												y_val = aimove.move->GetCenterY()
														+ t_dim;
											} else {
												y_val = aimove.move->GetCenterY()
														- t_dim;
											}

											int row = (int) y_val / t_dim;
											int col = (int) x_val / t_dim;
											tmp = board_sim[row][col]->GetOccupant();
											if (board_sim[row][col]->GetOccupant()->CheckStatus() == king) {
												Isking_c = true;
											}
											board_sim[row][col]->GetOccupant()->SetStatus(
													captured);
											board_sim[row][col]->SetOccupant(NULL);
											board_sim[row][col]->SetOccupation(false);
											captr = true;
										}

										CheckCaptures(aimove.move, board_sim);
										if (!aimove.move->CanCapture()) {
											moveMade = true;
										} else {
											moveMade = false;
										}
										if ((moveMade && sidechange)
												|| (sidechange && !moveMade)
												|| (!sidechange && moveMade)) {
											change = true;
										}
										if((player == black && aimove.move->GetCenterY() == 0 )
												|| (player == white && aimove.move->GetCenterY() == 700 )){
											bonus = 20;
										}
										if (change) {
											if (player == AIplayer) {
												aimove.value = PerformBestMove(board_sim, WhiteTeam, BlackTeam,
														Humanplayer, depth, alpha, beta).value + bonus;
												if(aimove.value > alpha)
												{
													alpha = aimove.value;
												}
											} else {
												aimove.value = PerformBestMove(board_sim, WhiteTeam, BlackTeam,
														AIplayer, depth, alpha, beta).value - bonus;
												if(aimove.value < beta)
												{
													beta = aimove.value;
												}
											}
											moveMade = false;
											change = false;
										} else {
											if (player == AIplayer) {
												aimove.value = PerformBestMove(board_sim, WhiteTeam, BlackTeam,
														AIplayer, depth, alpha, beta).value + bonus;
												if(aimove.value > alpha)
												{
													alpha = aimove.value;
												}
											} else {
												aimove.value = PerformBestMove(board_sim, WhiteTeam, BlackTeam,
														Humanplayer, depth, alpha, beta).value - bonus;
												if(aimove.value < beta)
												{
													beta = aimove.value;
												}
											}
											moveMade = false;
											change = false;
										}

										moves.push_back(aimove);
										if (captr) {
											float x_val, y_val;
											if (aimove.move->GetCenterX()
													- aimove.chosen->GetCenterX() < 0) {
												x_val = aimove.move->GetCenterX()
														+ t_dim;
											} else {
												x_val = aimove.move->GetCenterX()
														- t_dim;
											}
											if (aimove.move->GetCenterY()
													- aimove.chosen->GetCenterY() < 0) {
												y_val = aimove.move->GetCenterY()
														+ t_dim;
											} else {
												y_val = aimove.move->GetCenterY()
														- t_dim;
											}

											int row = (int) y_val / t_dim;
											int col = (int) x_val / t_dim;
											board_sim[row][col]->SetOccupant(tmp);
											board_sim[row][col]->SetOccupation(true);

											if (Isking_c) {
												board_sim[row][col]->GetOccupant()->SetStatus(
														king);
											} else {
												board_sim[row][col]->GetOccupant()->SetStatus(
														normal);
											}
											captr = false;
										}
										aimove.move->MoveOccupand(aimove.chosen);

										if (!Isking_p) {
											aimove.chosen->GetOccupant()->SetStatus(normal);
										}
									}
								}
							}
						}
					}
				} //isOccupied
			}
		}

	}	// main loop
	int bestmove = 0;
	if (player == AIplayer) {
		int bestvalue = -10000;
		for (int i = 0; i < (int) moves.size(); i++) {

			if (moves[i].value > bestvalue) {
				bestmove = i;
				bestvalue = moves[i].value;
			}
		}
	} else {
		int bestvalue = 10000;
		for (int i = 0; i < (int) moves.size(); i++) {
			if (moves[i].value < bestvalue) {
				bestmove = i;
				bestvalue = moves[i].value;
			}
		}
	}
	return moves[bestmove];
}

