#ifndef BOARD_h
#define BOARD_h

#include <sstream>
#include <vector>
#include <cmath>
#include "GameEnums.h"

class Board {
public:
	char b[16][16];
	std::pair<int, int> playerPos[2];
	Board();
	Board(std::stringstream &stream);
	std::vector<BoardMoves> LegalMoves(std::pair<int, int> pos);
	std::pair<double, double> getRelativePos(Player me, Player enemy);
	std::pair<int, int> getPosAfterMove(std::pair<int, int> currentPos, BoardMoves move);
	std::vector<std::pair<BoardMoves, int>> getAreaOfLegalMoves(Player pl);
	uint8_t calculateArea(std::pair<int, int> pos);
};

#endif
