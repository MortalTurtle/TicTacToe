#pragma once
#include "TicTacToe.h"
#include "Bots.h"
#include <iostream>

namespace TicTacToe
{
	class TicTacToeConsole
	{
		static void ChoosePlayerById(int id, TicTacToePlayers::IPlayer*& p)
		{
			if (id == 1)
				p = new TicTacToePlayers::HumanPlayer<TicTacToePlayers::HumanConsoleIO>();
			if (id == 2)
				p = new TicTacToeBots::EasyBot();
			if (id == 3)
				p = new TicTacToeBots::MediumBot();
			if (id == 4)
				p = new TicTacToeBots::HardBot();
		}

		static void ChoosePlayers(TicTacToePlayers::IPlayer*& p1, TicTacToePlayers::IPlayer*& p2)
		{
			std::cout << "Who is gonna play as the cross? \n enter 1 - Human, 2 - easyBot, 3 - medium bot, 4 - hard bot" << std::endl;
			int p1Id, p2Id;
			std::cin >> p1Id;
			while (p1Id < 1 || p1Id > 4)
			{
				std::cout << "Incorrect first player id, please try again" << std::endl;
				std::cin >> p1Id;
			}
			std::cout << "Who is gonna play as the zero? \n enter 1 - Human, 2 - easyBot, 3 - medium bot, 4 - hard bot" << std::endl;
			std::cin >> p2Id;
			while (p2Id < 1 || p2Id > 4)
			{
				std::cout << "Incorrect second player id, please try again" << std::endl;
				std::cin >> p2Id;
			}
			ChoosePlayerById(p1Id, p1);
			ChoosePlayerById(p2Id, p2);
		}

	public:
		static void StartGame()
		{
			TicTacToePlayers::IPlayer* p1;
			TicTacToePlayers::IPlayer* p2;
			ChoosePlayers(p1, p2);
			TicTacToe::TicTacToeGame game = TicTacToe::TicTacToeGame<TicTacToeBoard::BoardConsole>(*p1, *p2);
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
			delete p1;
			delete p2;
		}
	};
}