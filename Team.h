#pragma once
#include "stdafx.h"

class Coord;

enum TeamType
{
	none = 0,
	player = 1,
	agent = 2
};

class Team
{
public:
	Team() = default;
	~Team() = default;

	bool isPlayer = false;
	virtual bool makeMove();
protected:
	vector<Coord> chips;
};

class Player : public Team
{
public:
	Player();
	bool makeMove();
};

class Agent : public Team
{
public:
	Agent();
	bool makeMove();
};
