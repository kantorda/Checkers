#include "stdafx.h"
#include "Board.h"

Coord::Coord(int A, int N) : alpha(A), numeric(N) {}

Coord::Coord(string input)
{
	alpha = 0;
	numeric = input.at(1) - 1;

	switch (toupper(input[0], locale()))
	{
	case 'A':
		alpha = 0;
		break;
	case 'B':
		alpha = 1;
		break;
	case 'C':
		alpha = 2;
		break;
	case 'D':
		alpha = 3;
		break;
	case 'E':
		alpha = 4;
		break;
	case 'F':
		alpha = 5;
		break;
	case 'G':
		alpha = 6;
		break;
	case 'H':
		alpha = 7;
		break;
	}
}

void Coord::indicies(int& A, int& N)
{
	A = alpha;
	N = numeric;
}

Board::Board()
{
	board = vector<vector<Chip>>(8, vector<Chip>(8, Chip()));

	// Agent
	board[1][0].owner =
	board[3][0].owner =
	board[5][0].owner =
	board[7][0].owner =
	board[0][1].owner =
	board[2][1].owner =
	board[4][1].owner =
	board[6][1].owner =
	board[1][2].owner =
	board[3][2].owner =
	board[5][2].owner =
	board[7][2].owner = agent;

	// Player
	board[0][5].owner =
	board[2][5].owner =
	board[4][5].owner =
	board[6][5].owner =
	board[1][6].owner =
	board[3][6].owner =
	board[5][6].owner =
	board[7][6].owner =
	board[0][7].owner =
	board[2][7].owner =
	board[4][7].owner =
	board[6][7].owner = player;
	
}

Board& Board::Instance()
{
	static Board instance;
	return instance;
}

void Board::print()
{
	cout << "   | A | B | C | D | E | F | G | H |";
	cout << "\n   ---------------------------------\n";
	for (int i = 0; i < 8; ++i)
	{
		cout << " " << i + 1 << " |";
		for (int j = 0; j < 8; ++j)
		{
			if (board[j][i].owner == none)
			{
				cout << "   |";
			}
			else if (board[j][i].owner == player)
			{
				cout << " O |";
			}
			else
			{
				cout << " X |";
			}
		}
		cout << " " << i + 1;
		cout << "\n   ---------------------------------\n";
	}
	cout << "   | A | B | C | D | E | F | G | H |\n\n";

}
