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

	bool operator==(const Coord& rhs);

	void indicies(int& alpha, int& numeric);
	static bool isValid(string coord);
	static Coord middle(Coord start, Coord end);
};

class Board
{
public:
	vector<vector<Chip>> board;
	Player player = Player();
	Agent agent = Agent();

	static Board& Instance();
	void print();
	bool isValidMove(TeamType team, string move, vector<Coord> &tiles);
private:
	Board();
	Board(Board const&) = delete;
	void operator=(Board const&) = delete;
};
