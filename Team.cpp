#include "stdafx.h"
#include "Team.h"
#include "Board.h"

bool Team::makeMove()
{
	return false;
}

Player::Player()
{
	isPlayer = true;

	chips.resize(12);
	chips.push_back(Coord("B1"));
	chips.push_back(Coord("D1"));
	chips.push_back(Coord("F1"));
	chips.push_back(Coord("H1"));
	chips.push_back(Coord("A2"));
	chips.push_back(Coord("C2"));
	chips.push_back(Coord("E2"));
	chips.push_back(Coord("G2"));
	chips.push_back(Coord("B3"));
	chips.push_back(Coord("D3"));
	chips.push_back(Coord("F3"));
	chips.push_back(Coord("H3"));
}

bool Player::makeMove()
{
	return false;
}

Agent::Agent()
{
	chips.resize(12);
	chips.push_back(Coord("A6"));
	chips.push_back(Coord("C6"));
	chips.push_back(Coord("E6"));
	chips.push_back(Coord("G6"));
	chips.push_back(Coord("B7"));
	chips.push_back(Coord("D7"));
	chips.push_back(Coord("F7"));
	chips.push_back(Coord("H7"));
	chips.push_back(Coord("A8"));
	chips.push_back(Coord("C8"));
	chips.push_back(Coord("E8"));
	chips.push_back(Coord("G8"));
}

bool Agent::makeMove()
{
	return false;
}
