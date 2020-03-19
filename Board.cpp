#include "stdafx.h"
#include "Board.h"

Coord::Coord(int A, int N) : alpha(A), numeric(N) {}

Coord::Coord(string input)
{
	alpha = 0;
	numeric = (input.at(1) - '0') - 1;

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

bool Coord::operator==(const Coord& rhs)
{
	return alpha == rhs.alpha && 
		numeric == rhs.numeric;
}

void Coord::indicies(int& A, int& N)
{
	A = alpha;
	N = numeric;
}

string Coord::toString()
{
	string out = "";

	switch (alpha)
	{
	case 0:
		out.append("A");
		break;
	case 1:
		out.append("B");
		break;
	case 2:
		out.append("C");
		break;
	case 3:
		out.append("D");
		break;
	case 4:
		out.append("E");
		break;
	case 5:
		out.append("F");
		break;
	case 6:
		out.append("G");
		break;
	case 7:
		out.append("H");
		break;
	default:
		break;
	}


	out.append(to_string(numeric + 1));

	return out;
}

bool Coord::isValid(string coord)
{
	if (coord.length() != 2)
		return false;

	string validAlphas = "ABCDEFGH", validNumerics = "12345678";

	char alpha = toupper(coord.at(0), locale()), numeric = coord.at(1);


	if (validAlphas.find(alpha) == string::npos)
		return false;

	return validNumerics.find(numeric) != string::npos;
}

bool Coord::isValid(Coord coord)
{
	if (coord.alpha < 0 || 7 < coord.alpha)
	{
		return false;
	}

	if (coord.numeric < 0 || 7 < coord.numeric)
	{
		return false;
	}

	return true;
}

Coord Coord::middle(Coord start, Coord end)
{
	return Coord(((start.alpha + end.alpha) / 2),
		((start.numeric + end.numeric) / 2));
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
	board[7][2].owner = TeamType::agent;

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
	board[6][7].owner = TeamType::player;
	
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
			if (board[j][i].owner == TeamType::none)
			{
				cout << "   |";
			}
			else if (board[j][i].owner == TeamType::player)
			{
				if (board[j][i].king)
				{
					cout << " @ |";
				}
				else
				{
					cout << " O |";
				}
			}
			else
			{
				if (board[j][i].king)
				{
					cout << " % |";
				}
				else
				{
					cout << " X |";
				}
			}
		}
		cout << " " << i + 1;
		cout << "\n   ---------------------------------\n";
	}
	cout << "   | A | B | C | D | E | F | G | H |\n\n";
}

bool Board::isValidMove(TeamType team, string move, vector<Coord> &tiles)
{
	bool valid = true;

	// Split move into tile locations
	char* cstr = new char[move.length() + 1];
	//rsize_t cstrMax = sizeof cstr;
	//strcpy_s(cstr, cstrMax, move.c_str());
	strcpy(cstr, move.c_str());

	vector<string> tokens;
	const char* delim = ">";
	char* context;

	//char* tok = strtok_s(cstr, delim, &context);
	char* tok = strtok(cstr, delim);
	while (tok != 0)
	{
		tokens.push_back(string(tok));
		//tok = strtok_s(NULL, delim, &context);
		tok = strtok(NULL, delim);
	}

	// At least two moves
	if (tokens.size() < 2)
	{
		valid = false;
	}

	delete[] cstr;

	tiles.clear();
	if (valid)
	{
		for (int i = 0; i < tokens.size(); ++i)
		{
			// Correct Coord format and on the board
			if (!Coord::isValid(tokens[i]))
			{
				valid = false;
				break;
			}

			tiles.push_back(Coord(tokens[i]));
		}
	}

	// Starting chip belongs to the Team
	if (valid && board[tiles[0].alpha][tiles[0].numeric].owner != team)
	{
		tiles.clear();
		return false;
	}

	if (valid)
	{
		bool wasKinged = false;
		Chip chip = board[tiles.front().alpha][tiles.front().numeric];

		for (int i = 1; i < tiles.size(); ++i)
		{
			Chip endTile = board[tiles[i].alpha][tiles[i].numeric];
			
			// Tile being moved to is an empty tile
			if (endTile.owner != TeamType::none)
			{
				valid = false;
				break;
			}

			int horizontal = tiles[i].alpha - tiles[i - 1].alpha;
			int vertical = tiles[i].numeric - tiles[i - 1].numeric;

			// Correct direction
			if (team == TeamType::player)
			{
				if (!chip.king && !wasKinged && vertical > 0)
				{
					valid = false;
					break;
				}
			}
			else // agent
			{
				if (!chip.king && !wasKinged && vertical < 0)
				{
					valid = false;
					break;
				}
			}

			// Diagonal move
			if (abs(horizontal) == 2)
			{
				// Jump
				if (abs(vertical) != 2)
				{
					valid = false;
					break;
				}
				// Chip jumped was owned by other Team
				Coord middle = Coord::middle(tiles[i - 1], tiles[i]);
				if (team == TeamType::player)
				{
					if (board[middle.alpha][middle.numeric].owner != TeamType::agent)
					{
						valid = false;
						break;
					}
				}
				else // Agent
				{
					if (board[middle.alpha][middle.numeric].owner != TeamType::player)
					{
						valid = false;
						break;
					}
				}
			}
			else if (abs(horizontal) == 1)
			{
				// Not a jump
				if (abs(tiles[i].numeric - tiles[i - 1].numeric) != 1)
				{
					valid = false;
					break;
				}
			}
			else
			{
				valid = false;
				break;
			}

			// Check to see if this move made the Chip a King
			if (tiles[i].numeric == 0)
			{
				wasKinged = true;
			}
		}
	}

	if (!valid)
	{
		tiles.clear();
	}
	return valid;
}