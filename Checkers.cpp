// Checkers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "Board.h"

int main()
{
    cout << "\n          Welcome to Checkers\n\n";
    //Game().play();
    Board::Instance().print();
}