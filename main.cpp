#include "Game.h";
#include "sqlite/Database.h"


int main()
{
	Database::init();

	// game stuff
	Game game;


	sqlite3_close(Database::db);
	return 0;
}

