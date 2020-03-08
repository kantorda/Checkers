#include "stdafx.h"
#include "Team.h"
#include "Board.h"

bool Team::makeMove()
{
	return false;
}

void Team::removeSpaces(string& input)
{
	input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());
}

void Team::surrenderChips(vector<Coord> tiles)
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		chips.erase(find(chips.begin(), chips.end(), tiles[i]));
	}
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
	cin.ignore();
	cout << "Your move: ";
	string move = "";
	getline(cin, move);
	Team::removeSpaces(move);
	vector<Coord> tiles;
	
	while (!Board::Instance().isValidMove(TeamType::player, move, tiles))
	{
		if (move == "quit" || move == "q")
			return false;
		
		cout << "Invalid move, please try again\n";
		getline(cin, move);
		Team::removeSpaces(move);
	}

	// Move chip
	Board::Instance().board[tiles.front().alpha][tiles.front().numeric].owner = TeamType::none;
	Board::Instance().board[tiles.back().alpha][tiles.back().numeric].owner = TeamType::player;

	// Capture oponent's chips
	if (tiles.size() > 2)
	{
		vector<Coord> captures;

		for (int i = 1; i < tiles.size(); ++i)
		{
			captures.push_back(Coord::middle(tiles[i - 1], tiles[i]));
		}

		Board::Instance().agent.surrenderChips(captures);
	}
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
