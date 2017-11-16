#include "Bot.h"
using namespace std;

Bot::Bot() : board() {

	srand(int(time(0))+816);
}

void Bot::MakeMove(BoardMoves boardMove) {
	switch (boardMove) {
		case UP:    printf("up\n"); break;
		case DOWN:  printf("down\n"); break;
		case LEFT:  printf("left\n"); break;
		case RIGHT: printf("right\n"); break;
		case NO_MOVE:	printf("no_move\n"); break;
	}
}

void Bot::updateMoveStack() {
	pair<int, int> enemyPos = board.playerPos[enemyId];
	if (lastEnemyPos.first == -1) {
		lastEnemyPos = enemyPos;
		return;
	}
	std::pair<int, int> diff = make_pair(enemyPos.first - lastEnemyPos.first, enemyPos.second - lastEnemyPos.second);
	BoardMoves lastMove = NO_MOVE;
	if (diff.first == -1)		lastMove = LEFT;
	else if (diff.first == 1)	lastMove = RIGHT;
	else if (diff.second == 1)	lastMove = DOWN;
	else if (diff.second == -1)	lastMove = UP;
	enemyMoves.push_back(lastMove);
	lastEnemyMove = lastMove;
	lastEnemyPos = enemyPos;
}

vector<BoardMoves> Bot::getPreferedLegalMoves(vector<BoardMoves> preferedMoves) {
	vector<BoardMoves> legal_moves = board.LegalMoves(board.playerPos[playerId]);
	vector<BoardMoves> prefered_legal_moves;
	if (legal_moves.size() == 0) {
		MakeMove(UP);//no legal moves, just go up and lose
		return prefered_legal_moves;
	}
	//this loop is to make sure our prefered moves are legal
	for (uint8_t i = 0; i < preferedMoves.size(); i++) {
		for (uint8_t j = 0; j < legal_moves.size(); j++) {
			BoardMoves move = preferedMoves[i];
			if (move == legal_moves[j]) {
				prefered_legal_moves.push_back(move);
				legal_moves.erase(legal_moves.begin() + j);
				j--;
			}
		}
	}
	if (prefered_legal_moves.size() == 0 && legal_moves.size() > 0) return legal_moves;//all prefered moves are illegal, return all legal moves																			
	prefered_legal_moves.insert(prefered_legal_moves.end(), legal_moves.begin(), legal_moves.end());//add the legal moves that were not prefered to the end of the vector
	return prefered_legal_moves;
}

void Bot::makeBestMove(vector<BoardMoves> preferedMoves) {
	vector<BoardMoves> prefered_legal_moves = getPreferedLegalMoves(preferedMoves);
	vector<pair<BoardMoves, int>> areas = board.getAreaOfLegalMoves(playerId);
	pair<BoardMoves, int> move_with_biggest_area = make_pair(UP, 0);
	for (uint8_t i = 0; i < prefered_legal_moves.size(); i++) {
		BoardMoves preferedMove = prefered_legal_moves[i];
		for (uint8_t j = 0; j < areas.size(); j++) {
			pair<BoardMoves, int> area = areas[j];
			if (preferedMove == area.first) {
				/*if (area.second  1) {
					MakeMove(preferedMove);
					return;
				}*/
				if (area.second > move_with_biggest_area.second) {
					move_with_biggest_area = area;
				}
			}
		}
	}
	MakeMove(move_with_biggest_area.first);
}

vector<double> Bot::considerRelativePosition(vector<double> moves) {
	pair<double, double> relativePos = board.getRelativePos(playerId, enemyId);
	double hyp = hypot(relativePos.first, relativePos.second);
	moves[UP] *= 1 + (-relativePos.second / hyp / 4);
	moves[DOWN] *= 1 + (relativePos.second / hyp / 4);
	moves[LEFT] *= 1 + (-relativePos.first / hyp / 4);
	moves[RIGHT] *= 1 + (relativePos.first / hyp / 4);
	return moves;
}

vector<double> Bot::considerLegalMoves(vector<double> moves) {
	pair<double, double> currentPos = board.playerPos[playerId];
	for (uint8_t i = 0; i < 4; i++) {
		BoardMoves move = BoardMoves(i);
		pair<int, int> pos = board.getPosAfterMove(currentPos, move);
		float modifier = 1.0;
		for (uint8_t j = 0; j < 5; j++) {
			if (pos.first >= 0 && pos.first < 16 && pos.second >= 0 && pos.second < 16) {
				int size = board.LegalMoves(pos).size();
				if (size == 0) {
					modifier = .1;
					break;
				}
				else if (size == 1) modifier *= .6;
				else if (size == 2) modifier *= 1;
				else if (size = 3) modifier *= 1;
				pos = board.getPosAfterMove(pos, move);
			}
			else break;
		}
		moves[move] *= modifier;
	}
	return moves;
}

vector<double> Bot::considerTheWalls(vector<double> moves) {
	pair<double, double> currentPos = board.playerPos[playerId];
	for (uint8_t i = 0; i < 4; i++) {
		BoardMoves move = BoardMoves(i);
		if ((move == UP || move == DOWN) && (currentPos.first == 0 || currentPos.first == 15)) {//if i want to move up/down and i am already on the wall
			moves[move] *= .75;
		}
		else if ((move == RIGHT || move == LEFT) && (currentPos.second == 0 || currentPos.second == 15)) {//if i want to move right/left and i am already on the wall
			moves[move] *= .75;
		}
	}
	return moves;
}

BoardMoves Bot::getNextMovePrediction() {
	vector<BoardMoves> legalMoves = board.LegalMoves(board.playerPos[enemyId]);
	vector<BoardMoves> pattern;
	const int look_back = 10;
	int index = int(enemyMoves.size()) - 1 - look_back;
	for (int i = index < 0 ? 0 : index; i < enemyMoves.size(); i++) {
		BoardMoves move = enemyMoves[i];
		if (pattern.size() == 0 || pattern.size() == 1) {
			pattern.push_back(move);
			continue;
		}
		if (pattern[0] == pattern[1] && pattern[0] == move) {
			pattern.push_back(move);
		}
		else if (pattern[0] != pattern[1] && pattern[pattern.size() % 2] == move) {
			pattern.push_back(move);
		}
		else {
			i -= int(pattern.size());
			pattern.clear();
		}
	}
	BoardMoves nextMove = NO_MOVE;
	if (pattern.size() >= 3) {
		cout << "found pattern" << endl;
		nextMove = pattern[pattern.size() % 2];
	}
	if (nextMove != NO_MOVE) {
		for (int i = 0; i < legalMoves.size(); i++) {
			if (nextMove == legalMoves[i]) {
				cout << "expected next move in pattern is legal" << endl;
				return nextMove;
			}
		}
	}
	for (int i = 0; i < legalMoves.size(); i++) {
		if (legalMoves[i] == enemyMoves[enemyMoves.size() - 1]) {
			cout << "expecting last move" << endl;
			return legalMoves[i];
		}	
	}
	cout << "predicting random legal move" << endl;
	if(legalMoves.size() > 0) return legalMoves[rand() % legalMoves.size()];
	else return NO_MOVE;
}

void Bot::Move(int time) {
	updateMoveStack();
	if (lastEnemyPrediction != NO_MOVE) {
		if (lastEnemyPrediction == lastEnemyMove) {
			cout << "successful move prediction: " << lastEnemyPrediction << " " << lastEnemyMove << endl;
		}
		else cout << "failed move prediction: " << lastEnemyPrediction << " " << lastEnemyMove << endl;
	}

	if (round == 0) {
		MakeMove(Bot::getFirstMove());
		return;
	}
	else {
		vector<BoardMoves> legal_moves = board.LegalMoves(board.playerPos[playerId]);
		vector<double> moves;
		for (int i = 0; i < 4; i++) {
			bool legal = false;
			for (int j = 0; j < legal_moves.size(); j++) {
				if (i == legal_moves[j]) legal = true;
			}
			moves.push_back(legal ? 1.0 : 0);
		}

		BoardMoves nextEnemyMove = Bot::getNextMovePrediction();
		lastEnemyPrediction = nextEnemyMove;
		if (nextEnemyMove != NO_MOVE) {
			pair<double, double> currentPos = board.playerPos[playerId];
			pair<double, double> enemyPos = board.playerPos[enemyId];
			pair<double, double> target = board.getPosAfterMove(enemyPos, nextEnemyMove);
			pair<int, int> relativePos = board.getRelativePos(playerId, enemyId);
			BoardMoves myNextMove;
			if ((currentPos.second <= 7 && enemyPos.second < currentPos.second) || (currentPos.second >= 7 && enemyPos.second > currentPos.second)) {
				myNextMove = pathfind(currentPos, make_pair(target.first, currentPos.second));//stay at my current Y, but block them on the X axis
			}
			else if ((currentPos.first <= 7 && enemyPos.first > 7) || (currentPos.first >= 7 && enemyPos.first < 7)) {
				myNextMove = pathfind(currentPos, make_pair(currentPos.first, target.second));//stay at my current X, but block them on the Y axis
			}
			else {
				myNextMove = pathfind(currentPos, target);
			}
			if (myNextMove != NO_MOVE) {
				moves = considerRelativePosition(moves);
				moves = considerLegalMoves(moves);
				moves[myNextMove] *= 2.0;
				//pair<double, double> newPos = board.getPosAfterMove(currentPos, myNextMove);
				//pair<double, double> diff = make_pair(target.first - newPos.first, target.second - newPos.second);
			}
			else {
				//efficient movement algorithm:
				//make an efficient movve
				moves = considerLegalMoves(moves);
			}
		}

		double largest_value = 0;
		int largest_move = NO_MOVE;
		vector<BoardMoves> bestMoves;
		for (int j = 0; j < 4; j++) {
			//std::cout << moves[i] << endl;
			if (moves[j] >= largest_value) {
				largest_value = moves[j];
				largest_move = j;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (moves[i] == largest_value) {
				bestMoves.push_back(BoardMoves(i));
			}
		}
		MakeMove(BoardMoves(bestMoves[rand() % bestMoves.size()]));
	}
}

BoardMoves Bot::getFirstMove() {
	pair<double, double> myPos = board.playerPos[playerId];
	pair<int, int> relativePos = board.getRelativePos(playerId, enemyId);
	vector<BoardMoves> legalMoves = board.LegalMoves(myPos);
	if (myPos.second <= 6) return DOWN;
	else if (myPos.second >= 9) return UP;
	else if (std::abs(relativePos.first) > 1 && (myPos.second == 7 || myPos.second == 8)) {//left or right
		double hyp = hypot(relativePos.first, relativePos.second);
		double leftVal = (-relativePos.first / hyp);
		double rightVal = (relativePos.first / hyp);

		if (leftVal > rightVal) {
			return LEFT;
		}
		else return RIGHT;
	}
	else return NO_MOVE;
}

BoardMoves Bot::pathfind(pair<int,int> from, pair<int, int> to) {
	pathfinder.pathStatus[1] = pathfinder.FindPath(1, from.first, from.second, to.first, to.second);
	if (pathfinder.pathStatus[1] == 1) {
		pathfinder.ReadPath(1, from.first, from.second, 1);
		pair<int, int> newPos = make_pair(pathfinder.xPath[1], pathfinder.yPath[1]);
		pair<int, int> diff = make_pair(newPos.first - from.first, newPos.second - from.second);
		if (diff.first == 1)		return RIGHT;
		else if (diff.first == -1)	return LEFT;
		else if (diff.second == 1)	return DOWN;
		else if (diff.second == -1) return UP;
	}
	return NO_MOVE;
}

void Bot::Round(int time) { this->round = time; };
void Bot::Update(Board board) { 
	this->board = board; 
	for (uint8_t x = 0; x < 16; x++) {
		for (uint8_t y = 0; y < 16; y++) {
			pathfinder.walkability[x][y] = !board.b[x][y];
		}
	}
}
void Bot::Timebank(int time) { };
void Bot::TimePerMove(int time) { };
void Bot::YourBot(string name) { };
void Bot::YourBotId(Player playerId) { this->playerId = playerId; this->enemyId = Player(!playerId); };
void Bot::FieldWidth(int width) { this->width = width; };
void Bot::FieldHeight(int height) { this->height = height; };
void Bot::PlayerNames(string player1, string player2) { };
