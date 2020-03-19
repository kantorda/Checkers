// Checkers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "Board.h"
#include "Game.h"

int main()
{
    cout << "\nWelcome to Checkers\n\n";
    cout << "To enter a move, type in the following format: '[Starting Coordinates] > [Ending Coordinates]'\n";
    cout << "Example: 'C6 > D5\n";
    cout << "To quit, enter 'quit' or 'q'\n";
    cout << "Are you ready to begin?\n";
    string resp = "";
    while (resp != "Y" && resp != "Yes" && 
            resp != "y" && resp != "yes")
    {
        getline(cin, resp);
        if (resp == "q" || resp == "Q" ||
            resp == "quit" || resp == "Quit")
        {
            return 0;
        }
    }
    cout << "\n\n";
    Game().play();
}