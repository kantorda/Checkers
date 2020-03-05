#pragma once
#include "stdafx.h"
#include "Team.h"

struct Chip
{
	TeamType owner = TeamType::none;
	bool king = false;
};

class Coord
{
public:
	int alpha;
	int numeric;

	Coord() = default;
	Coord(int A, int N);
	Coord(string input);
	~Coord() = default;

	void indicies(int& alpha, int& numeric);
};

class Board
{
public:
	vector<vector<Chip>> board;

	static Board& Instance();
	void print();
private:
	Board();
	Board(Board const&) = delete;
	void operator=(Board const&) = delete;
};
