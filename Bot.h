#ifndef BOT_h
#define BOT_h

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "AStarPathfinder.h"
#include "Board.h"

class Bot {
private:
	Player playerId;
	Player enemyId;
	int width, height;
	Board board;
	int round;
	AStarPathFinder pathfinder;
	std::pair<int, int> lastEnemyPos;
	BoardMoves lastEnemyMove = NO_MOVE;
	BoardMoves lastEnemyPrediction = NO_MOVE;
	std::vector<BoardMoves> enemyMoves;

public:
	void MakeMove(BoardMoves boardMove);
	void updateMoveStack();
	std::vector<BoardMoves> getPreferedLegalMoves(std::vector<BoardMoves> preferedMoves);
	void makeBestMove(std::vector<BoardMoves> preferedMoves);
	std::vector<double> considerRelativePosition(std::vector<double> moves);

	std::vector<double> considerLegalMoves(std::vector<double> moves);

	BoardMoves getBestLegalMove();

	std::vector<double> considerTheWalls(std::vector<double> moves);

	BoardMoves getNextMovePrediction();

	Bot();
	// Action
	virtual void Move(int time);
	BoardMoves getFirstMove();
	BoardMoves pathfind(std::pair<int, int> from, std::pair<int, int> to);
	// Update
	virtual void Round(int time);
	virtual void Update(Board board);
	// Settings
	virtual void Timebank(int time);
	virtual void TimePerMove(int time);
	virtual void YourBot(std::string name);
	virtual void YourBotId(Player playerId);
	virtual void FieldWidth(int width);
	virtual void FieldHeight(int height);
	virtual void PlayerNames(std::string player1, std::string player2);
};

#endif
