#include <iostream>
#include <cmath>
#include "TicTacToe.h"
#include "Bots.h"

int main()
{
	Decisions::Decisions::BuildTree();
	TicTacToe::IPlayer* p1 = new TicTacToe::HumanPlayer<TicTacToe::HumanConsoleIO>();
	TicTacToe::IPlayer* p2 = new TicTacToeBots::HardBot();
	TicTacToe::TicTacToeGame game = TicTacToe::TicTacToeGame<TicTacToe::BoardConsole>(*p1, *p2);
	game.Play();
	std::cout << "Do you wish to play again? If you do, write Y" << std::endl;
	std::string str;
	std::cin >> str;
	while (str == "Y")
	{
		game.Play();
		std::cout << "Do you wish to play again? If you do, write Y" << std::endl;
		std::cin >> str;
	}
	return 0;
}