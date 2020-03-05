#include "stdafx.h"
#include "Game.h"
#include "Board.h"

void Game::play() 
{
	while (!gameOver)
	{
		Board::Instance().print();

		cout << endl;

		if (!player.makeMove())
		{
			cout << "Sorry, you lost\nBetter luck next time!\n";
			gameOver = true;
		}

		cout << endl;

		if (!agent.makeMove())
		{
			cout << "You Won\nCongratulations!\n";
		}

		cout << endl;
	}

	cout << "The game is over, the board looks like this\n\n";
	Board::Instance().print();
	cout << endl;
}
