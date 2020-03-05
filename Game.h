#pragma once
#include "stdafx.h"
#include "Team.h"

class Game
{
public:
	Game() = default;
	~Game() = default;
	void play();
private:
	Player player = Player();
	Agent agent = Agent();
	bool gameOver = false;
};