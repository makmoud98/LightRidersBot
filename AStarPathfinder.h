#define MAP_WIDTH 16
#define MAP_HEIGHT 16

class AStarPathFinder {
public:
	const int notfinished = 0, notStarted = 0;// path-related constants
	const int found = 1, nonexistent = 2;
	const int walkable = 0, unwalkable = 1;

	int onClosedList = 10;
	char walkability[MAP_WIDTH][MAP_HEIGHT];
	int openList[MAP_WIDTH * MAP_HEIGHT + 2]; //1 dimensional array holding ID# of open list items
	int whichList[MAP_WIDTH + 1][MAP_HEIGHT + 1];  //2 dimensional array used to record whether a cell is on the open list or on the closed list.
	int openX[MAP_WIDTH * MAP_HEIGHT + 2]; //1d array stores the x location of an item on the open list
	int openY[MAP_WIDTH * MAP_HEIGHT + 2]; //1d array stores the y location of an item on the open list
	int parentX[MAP_WIDTH + 1][MAP_HEIGHT + 1]; //2d array to store parent of each cell (x)
	int parentY[MAP_WIDTH + 1][MAP_HEIGHT + 1]; //2d array to store parent of each cell (y)
	int Fcost[MAP_WIDTH * MAP_HEIGHT + 2];	//1d array to store F cost of a cell on the open list
	int Gcost[MAP_WIDTH + 1][MAP_HEIGHT + 1]; 	//2d array to store G cost for each cell.
	int Hcost[MAP_WIDTH * MAP_HEIGHT + 2];	//1d array to store H cost of a cell on the open list
	int pathLength[2];     //stores length of the found path for critter
	int pathLocation[2];   //stores current position along the chosen path for critter		
	int* pathBank[2];
	int pathStatus[2];
	int xPath[2];
	int yPath[2];

	AStarPathFinder();
	~AStarPathFinder();
	int FindPath(int pathfinderID, int startingX, int startingY, int targetX, int targetY);
	void ReadPath(int pathfinderID, int currentX, int currentY, int pixelsPerFrame);
	int ReadPathX(int pathfinderID, int pathLocation);
	int ReadPathY(int pathfinderID, int pathLocation);
};