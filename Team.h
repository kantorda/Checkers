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
	void surrenderChips(vector<Coord> tiles);
protected:
	vector<Coord> chips;
	static void removeSpaces(string& input);
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
