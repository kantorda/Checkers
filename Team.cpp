#include "stdafx.h"
#include "Team.h"
#include "Board.h"

bool Move::operator<(Move& other)
{
	return score < other.score;
}

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
		Board::Instance().board[tiles[i].alpha][tiles[i].numeric].owner = TeamType::none;
	}
}

Player::Player()
{
	isPlayer = true;

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

bool Player::makeMove()
{
	// If no chips left, Player has lost
	if (chips.size() == 0)
	{
		return false;
	}

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
		cout << "Your move: ";
		getline(cin, move);
		Team::removeSpaces(move);
	}

	// Move chip
	Board::Instance().board[tiles.front().alpha][tiles.front().numeric].owner = TeamType::none;
	Board::Instance().board[tiles.back().alpha][tiles.back().numeric].owner = TeamType::player;

	vector<Coord>::iterator chip = find(chips.begin(), chips.end(), tiles.front());
	chip->alpha = tiles.back().alpha;
	chip->numeric = tiles.back().numeric;

	// Capture oponent's chips
	vector<Coord> captures;

	for (int i = 1; i < tiles.size(); ++i)
	{
		if (abs(tiles[i].alpha - tiles[i - 1].alpha) == 2 &&
			abs(tiles[i].numeric - tiles[i - 1].numeric) == 2)
		{
			captures.push_back(Coord::middle(tiles[i - 1], tiles[i]));
		}
	}

	Board::Instance().agent.surrenderChips(captures);

	// Make chip a King if it touched opponent's home row
	for (int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i].numeric == 0)
		{
			Board::Instance().board[tiles.back().alpha][tiles.back().numeric].king = true;
			break;
		}
	}

	return true;
}

Agent::Agent()
{
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

bool Agent::makeMove()
{
	// If no chips left, Agent has lost
	if (chips.size() == 0)
	{
		return false;
	}

	// Calculate moves for all chips
	moves.clear();
	for (int i = 0; i < chips.size(); ++i)
	{
		calculateMoves(chips[i]);
	}

	if (moves.size() == 0)
	{
		return false;
	}

	// Sort for best move(s)
	sort(moves.begin(), moves.end());

	// if more than one move is tied with highest score
	// pick randomly
	vector<Move> bestMoves;
	for (int i = 0; i < moves.size(); ++i)
	{
		if (moves[i].score == moves[0].score)
		{
			bestMoves.push_back(moves[i]);
		}
		else
		{
			break;  // Already sorted, so first instance not matching we can exit
		}
	}

	int indx = rand() % bestMoves.size();

	Move move = bestMoves[indx];

	// Move chip
	Board::Instance().board[move.tiles.front().alpha][move.tiles.front().numeric].owner = TeamType::none;
	Board::Instance().board[move.tiles.back().alpha][move.tiles.back().numeric].owner = TeamType::agent;

	vector<Coord>::iterator chip = find(chips.begin(), chips.end(), move.tiles.front());
	chip->alpha = move.tiles.back().alpha;
	chip->numeric = move.tiles.back().numeric;

	// Capture oponent's chips
	vector<Coord> captures;

	for (int i = 1; i < move.tiles.size(); ++i)
	{
		if (abs(move.tiles[i].alpha - move.tiles[i - 1].alpha) == 2 &&
			abs(move.tiles[i].numeric - move.tiles[i - 1].numeric == 2))
		{
			captures.push_back(Coord::middle(move.tiles[i - 1], move.tiles[i]));
		}
	}

	Board::Instance().player.surrenderChips(captures);

	// Make chip a King if it touched opponent's home row
	for (int i = 0; i < move.tiles.size(); ++i)
	{
		if (move.tiles[i].numeric == 7)
		{
			Board::Instance().board[move.tiles.back().alpha][move.tiles.back().numeric].king = true;
			break;
		}
	}

	cout << "I made this move: ";
	for (int i = 0; i < move.tiles.size(); ++i)
	{
		cout << move.tiles[i].toString();
		if (i != move.tiles.size() - 1)
		{
			cout << " > ";
		}
	}

	cout << endl;

	return true;
}

void Agent::calculateMoves(Coord chip)
{
	vector<Move> potentials;
	Coord target;

	// Non-king chip
	// Down Left
	target.alpha = chip.alpha - 1;
	target.numeric = chip.numeric + 1;
	if (Coord::isValid(target))
	{
		// Single Move
		if (Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
		{
			Move move;
			move.tiles.push_back(chip);
			move.tiles.push_back(target);

			// check for threat in new position
			move.score -= calculateThreat(target);

			// Incentivise protecting the Home Row
			if (chip.numeric == 0)
			{
				--move.score;
			}

			potentials.push_back(move);
		}
		else if (Board::Instance().board[target.alpha][target.numeric].owner == TeamType::player)
		{
			// check for jump
			target.alpha -= 1;
			target.numeric += 1;
			if (Coord::isValid(target) &&
				Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
			{
				Move move;
				move.tiles.push_back(chip);
				move.tiles.push_back(target);
				++move.score; // For the capture

				// check for threat in new position
				move.score -= calculateThreat(target);

				// Incentivise protecting the Home Row
				if (chip.numeric == 0)
				{
					--move.score;
				}

				// Recurse
				vector<vector<Chip>> localBoard(Board::Instance().board);
				localBoard[target.alpha + 1][target.numeric - 1].owner = TeamType::none;
				vector<Move> localMoves(1, move);

				// Set Chip as King in localBoard if target was oponent's home row
				// Only do this in the localBoard, true board will change if the chosen move
				// Makes a new King
				if (target.numeric == 7)
				{
					localBoard[target.alpha][target.numeric].king = true;
				}

				if (!potentials.empty())
				{
					calculateGain(localBoard, potentials);
					potentials.insert(potentials.end(), localMoves.begin(), localMoves.end());
				}
			}
		}
	}
	// Down Right
	target.alpha = chip.alpha + 1;
	target.numeric = chip.numeric + 1;
	if (Coord::isValid(target))
	{
		// Single Move
		if (Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
		{
			Move move;
			move.tiles.push_back(chip);
			move.tiles.push_back(target);

			// check for threat in new position
			move.score -= calculateThreat(target);

			// Incentivise protecting the Home Row
			if (chip.numeric == 0)
			{
				--move.score;
			}

			potentials.push_back(move);
		}
		else if (Board::Instance().board[target.alpha][target.numeric].owner == TeamType::player)
		{
			// check for jump
			target.alpha += 1;
			target.numeric += 1;
			if (Coord::isValid(target) &&
				Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
			{
				Move move;
				move.tiles.push_back(chip);
				move.tiles.push_back(target);
				++move.score; // For the capture

				// check for threat in new position
				move.score -= calculateThreat(target);

				// Incentivise protecting the Home Row
				if (chip.numeric == 0)
				{
					--move.score;
				}

				// Recurse
				vector<vector<Chip>> localBoard(Board::Instance().board);
				localBoard[target.alpha - 1][target.numeric - 1].owner = TeamType::none;
				vector<Move> localMoves(1, move);

				// Set Chip as King in localBoard if target was oponent's home row
				// Only do this in the localBoard, true board will change if the chosen move
				// Makes a new King
				if (target.numeric == 7)
				{
					localBoard[target.alpha][target.numeric].king = true;
				}

				if (!potentials.empty())
				{
					calculateGain(localBoard, potentials);
					potentials.insert(potentials.end(), localMoves.begin(), localMoves.end());
				}
			}
		}
	}

	// King chip
	if (Board::Instance().board[chip.alpha][chip.numeric].king)
	{
		// Up Left
		target.alpha = chip.alpha - 1;
		target.numeric = chip.numeric - 1;
		if (Coord::isValid(target))
		{
			// Single Move
			if (Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
			{
				Move move;
				move.tiles.push_back(chip);
				move.tiles.push_back(target);

				// check for threat in new position
				move.score -= calculateThreat(target);

				// Incentivise protecting the Home Row
				if (chip.numeric == 0)
				{
					--move.score;
				}

				potentials.push_back(move);
			}
			else if (Board::Instance().board[target.alpha][target.numeric].owner == TeamType::player)
			{
				// check for jump
				target.alpha -= 1;
				target.numeric -= 1;
				if (Coord::isValid(target) &&
					Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
				{
					Move move;
					move.tiles.push_back(chip);
					move.tiles.push_back(target);
					++move.score; // For the capture

					// check for threat in new position
					move.score -= calculateThreat(target);

					// Incentivise protecting the Home Row
					if (chip.numeric == 0)
					{
						--move.score;
					}

					// Recurse
					vector<vector<Chip>> localBoard(Board::Instance().board);
					localBoard[target.alpha + 1][target.numeric + 1].owner = TeamType::none;
					vector<Move> localMoves(1, move);
					if (!potentials.empty())
					{
						calculateGain(localBoard, potentials);
						potentials.insert(potentials.end(), localMoves.begin(), localMoves.end());
					}
				}
			}
		}
		// Up Right
		target.alpha = chip.alpha + 1;
		target.numeric = chip.numeric - 1;
		if (Coord::isValid(target))
		{
			// Single Move
			if (Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
			{
				Move move;
				move.tiles.push_back(chip);
				move.tiles.push_back(target);

				// check for threat in new position
				move.score -= calculateThreat(target);

				// Incentivise protecting the Home Row
				if (chip.numeric == 0)
				{
					--move.score;
				}

				potentials.push_back(move);
			}
			else if (Board::Instance().board[target.alpha][target.numeric].owner == TeamType::player)
			{
				// check for jump
				target.alpha += 1;
				target.numeric -= 1;
				if (Coord::isValid(target) &&
					Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
				{
					Move move;
					move.tiles.push_back(chip);
					move.tiles.push_back(target);
					++move.score; // For the capture

					// check for threat in new position
					move.score -= calculateThreat(target);

					// Incentivise protecting the Home Row
					if (chip.numeric == 0)
					{
						--move.score;
					}

					// Recurse
					vector<vector<Chip>> localBoard(Board::Instance().board);
					localBoard[target.alpha - 1][target.numeric + 1].owner = TeamType::none;
					vector<Move> localMoves(1, move);
					if (!potentials.empty())
					{
						calculateGain(localBoard, potentials);
						potentials.insert(potentials.end(), localMoves.begin(), localMoves.end());
					}
				}
			}
		}
	}

	if (!potentials.empty())
	{
		moves.insert(moves.begin(), potentials.begin(), potentials.end());
	}
}

// TODO Use this same function to incentivise moving a peice out of danger, ontop of the original intent of
//		stopping the AI from taking a risky move
int Agent::calculateThreat(Coord chip)
{
	int threat = 0;
	Coord downLeft(chip.alpha - 1, chip.numeric + 1);
	if (Coord::isValid(downLeft))
	{
		// Direct Threat
		if (Board::Instance().board[downLeft.alpha][downLeft.numeric].owner == TeamType::player)
		{
			// If space for player to jump, direct threat
			Coord landing(chip.alpha + 1, chip.numeric - 1);
			if (Coord::isValid(landing) && 
				Board::Instance().board[landing.alpha][landing.numeric].owner == TeamType::none)
			{
				++threat;
			}
		}
		// Jump Threat
		else if (Board::Instance().board[downLeft.alpha][downLeft.numeric].owner == TeamType::none)
		{
			//TODO: Jump Threat
		}
	}

	Coord downRight(chip.alpha + 1, chip.numeric + 1);
	if (Coord::isValid(downRight))
	{
		// Direct Threat
		if (Board::Instance().board[downRight.alpha][downRight.numeric].owner == TeamType::player)
		{
			// If space for player to jump, direct threat
			Coord landing(chip.alpha - 1, chip.numeric - 1);
			if (Coord::isValid(landing) &&
				Board::Instance().board[landing.alpha][landing.numeric].owner == TeamType::none)
			{
				++threat;
			}
		}
		// Jump Threat
		else if (Board::Instance().board[downRight.alpha][downRight.numeric].owner == TeamType::none)
		{
			//TODO: Jump Threat
		}
	}

	Coord upLeft(chip.alpha - 1, chip.numeric - 1);
	if (Coord::isValid(upLeft))
	{
		// Direct Threat
		// Only threat if Chip is Player King
		if (Board::Instance().board[upLeft.alpha][upLeft.numeric].owner == TeamType::player &&
			Board::Instance().board[upLeft.alpha][upLeft.numeric].king)
		{
			// If space for player to jump, direct threat
			Coord landing(chip.alpha + 1, chip.numeric + 1);
			if (Coord::isValid(landing) &&
				Board::Instance().board[landing.alpha][landing.numeric].owner == TeamType::none)
			{
				++threat;
			}
		}
		// Jump Threat
		else if (Board::Instance().board[upLeft.alpha][upLeft.numeric].owner == TeamType::none)
		{
			//TODO: Jump Threat
		}
	}

	Coord upRight(chip.alpha + 1, chip.numeric - 1);
	if (Coord::isValid(upRight))
	{
		// Direct Threat
		// Only threat if Chip is Player King
		if (Board::Instance().board[upRight.alpha][upRight.numeric].owner == TeamType::player &&
			Board::Instance().board[upRight.alpha][upRight.numeric].king)
		{
			// If space for player to jump, direct threat
			Coord landing(chip.alpha + 1, chip.numeric - 1);
			if (Coord::isValid(landing) &&
				Board::Instance().board[landing.alpha][landing.numeric].owner == TeamType::none)
			{
				++threat;
			}
		}
		// Jump Threat
		else if (Board::Instance().board[upRight.alpha][upRight.numeric].owner == TeamType::none)
		{
			//TODO: Jump Threat
		}
	}

	return threat;
}

void Agent::calculateGain(vector<vector<Chip>> board, vector<Move> &localMoves)
{
	Coord piece(localMoves.back().tiles.back());
	Coord target;

	// Down Left
	target.alpha = piece.alpha - 1;
	target.numeric = piece.numeric + 1;
	if (Coord::isValid(target) &&
		board[target.alpha][target.numeric].owner == TeamType::player)
	{
		// check for jump
		target.alpha -= 1;
		target.numeric += 1;
		if (Coord::isValid(target) &&
			Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
		{
			Move move(localMoves.back());
			move.tiles.push_back(target);
			++move.score;

			// If target is oponent's home row, make the Chip a King
			// And grant an extra point
			if (target.numeric == 7)
			{
				board[target.alpha][target.numeric].king = true;
				++move.score;
			}

			// Recurse
			board[target.alpha + 1][target.numeric - 1].owner = TeamType::none;
			localMoves.push_back(move);
			calculateGain(board, localMoves);
		}
	}

	// Down Right
	target.alpha = piece.alpha + 1;
	target.numeric = piece.numeric + 1;
	if (Coord::isValid(target) &&
		board[target.alpha][target.numeric].owner == TeamType::player)
	{
		// check for jump
		target.alpha += 1;
		target.numeric += 1;
		if (Coord::isValid(target) &&
			Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
		{
			Move move(moves.back());
			move.tiles.push_back(target);
			++move.score;

			// If target is oponent's home row, make the Chip a King
			// And grant an extra point
			if (target.numeric == 7)
			{
				board[target.alpha][target.numeric].king = true;
				++move.score;
			}

			// Recurse
			board[target.alpha - 1][target.numeric - 1].owner = TeamType::none;
			localMoves.push_back(move);
			calculateGain(board, localMoves);
		}
	}

	if (board[piece.alpha][piece.numeric].king)
	{
		// Up Left
		target.alpha = piece.alpha - 1;
		target.numeric = piece.numeric - 1;
		if (Coord::isValid(target) &&
			board[target.alpha][target.numeric].owner == TeamType::player)
		{
			// check for jump
			target.alpha -= 1;
			target.numeric -= 1;
			if (Coord::isValid(target) &&
				Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
			{
				Move move(localMoves.back());
				move.tiles.push_back(target);
				++move.score;

				// Recurse
				board[target.alpha + 1][target.numeric + 1].owner = TeamType::none;
				localMoves.push_back(move);
				calculateGain(board, localMoves);
			}
		}

		// Up Right
		target.alpha = piece.alpha + 1;
		target.numeric = piece.numeric - 1;
		if (Coord::isValid(target) &&
			board[target.alpha][target.numeric].owner == TeamType::player)
		{
			// check for jump
			target.alpha += 1;
			target.numeric -= 1;
			if (Coord::isValid(target) &&
				Board::Instance().board[target.alpha][target.numeric].owner == TeamType::none)
			{
				Move move(localMoves.back());
				move.tiles.push_back(target);
				++move.score;

				// Recurse
				board[target.alpha - 1][target.numeric + 1].owner = TeamType::none;
				localMoves.push_back(move);
				calculateGain(board, localMoves);
			}
		}
	}
}