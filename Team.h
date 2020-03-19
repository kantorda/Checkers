#pragma once
#include "stdafx.h"

class Coord;
class Chip;

enum TeamType
{
	none = 0,
	player = 1,
	agent = 2
};

struct Move
{
public:
	int score = 0;
	vector<Coord> tiles;

	bool operator<(Move& other);
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
private:
	vector<Move> moves;

	void calculateMoves(Coord chip);
	int calculateThreat(Coord chip);
	void calculateGain(vector<vector<Chip>> board, vector<Move> &moves);
};
