#ifndef GAMEENUMS_h
#define GAMEENUMS_h

enum Player {
	Pl1 = 0, Pl2 = 1
};

enum Strategy {
	FIRSTMOVE, BLOCKENEMY, EFFICIENT
};

enum BoardMoves {
	UP = 0,DOWN = 1,LEFT = 2,RIGHT = 3, NO_MOVE=4
};

#endif
