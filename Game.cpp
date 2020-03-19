#include "stdafx.h"
#include "Game.h"
#include "Board.h"

void Game::play() 
{
	// Initialize seed for 'random' number generator
	srand(time(NULL));

	while (!gameOver)
	{
		Board::Instance().print();

		cout << endl;

		if (!Board::Instance().player.makeMove())
		{
			cout << "Sorry, you lost\nBetter luck next time!\n";
			gameOver = true;
			break;
		}

		cout << endl;

		if (!Board::Instance().agent.makeMove())
		{
			cout << "You Won\nCongratulations!\n";
			gameOver = true;
			break;
		}

		cout << endl;
	}

	cout << "\nThe game is over, the board looks like this\n\n";
	Board::Instance().print();
	cout << endl;
}
