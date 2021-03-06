#ifndef __DUNGEON_CRAWL_H__
#define __DUNGEON_CRAWL_H__

#include <iostream>
#include <string>
#include <time.h>
#include <conio.h>
#include "Console.h"

using namespace std;
using namespace Course;

class DungeonCrawl
{

private:
	
	int nTraps;

	string board[100][100];

	struct BoardSize
	{
		int row;
		int col;
	} maxBoard;

	struct Position
	{
		int x;
		int y;
	} player, trap, goal;


public:

	DungeonCrawl();
	DungeonCrawl(int, int);

	~DungeonCrawl() { cout << "\nDeconstructing class\n"; }

	bool active;
	bool mainloop;

	void checkPosition();
	void drawMap();

	void playerWon(bool);
	void playerMove();

	void createTraps();
	void createMap();

	void placeTraps();
	void placePlayer();
	void placeGoal();

};

#endif // !__DUNGEON_CRAWL_H__
