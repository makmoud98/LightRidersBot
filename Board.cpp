	#include "Board.h"
	using namespace std;

	Board::Board() {}

	Board::Board(stringstream &stream){
	  int x = 0, y = 0;
	  string line;
	  while (getline(stream, line, ',')){
		b[x][y] = (line == ".");
		if (line == "0"){
		  playerPos[Pl1] = make_pair(x,y);
		}
		if (line == "1")
		  playerPos[Pl2] = make_pair(x,y);
		x = (x+1)%16;
		if (x == 0)
		  y++;
	  }
	}

	vector<BoardMoves> Board::LegalMoves(pair<int, int> pos){
	  vector<BoardMoves> moves;
	  if (pos.second > 0  && b[pos.first][pos.second-1]) moves.push_back(UP);
	  if (pos.second < 15 && b[pos.first][pos.second+1]) moves.push_back(DOWN);
	  if (pos.first > 0   && b[pos.first-1][pos.second]) moves.push_back(LEFT);
	  if (pos.first < 15  && b[pos.first+1][pos.second]) moves.push_back(RIGHT);
	  return moves;
	}

	pair<double, double> Board::getRelativePos(Player me, Player enemy) {
		pair<int, int> myPos = playerPos[me];
		pair<int, int> enemyPos = playerPos[enemy];
		return make_pair(double(enemyPos.first - myPos.first), double(enemyPos.second - myPos.second));
	}

	pair<int, int> Board::getPosAfterMove(pair<int, int> currentPos, BoardMoves move) {
		switch (move) {
			case UP:    currentPos.second--; break;
			case DOWN:  currentPos.second++; break;
			case LEFT:  currentPos.first--; break;
			case RIGHT: currentPos.first++; break;
		}
		return currentPos;
	}

	vector<pair<BoardMoves, int>> Board::getAreaOfLegalMoves(Player pl) {
		vector<pair<BoardMoves, int>> areas;
		pair<int, int> myPos = playerPos[pl];
		pair<int, int> currentPos = myPos;
		vector<BoardMoves> moves = LegalMoves(playerPos[pl]);
		for (uint8_t i = 0; i < moves.size(); i++) {
			areas.push_back(make_pair(moves[i], LegalMoves(getPosAfterMove(currentPos, moves[i])).size()));
		}
		return areas;
	}