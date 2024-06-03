#pragma once
#include <iostream>
#include <concepts>
#include "Board.h"
#include "Players.h"
#include "DecisionTable.h"
namespace TicTacToe
{
	template <std::derived_from<TicTacToeBoard::IBoard> TBoard>
	class TicTacToeGame
	{
		TicTacToeBoard::IBoard* board;
		const TicTacToePlayers::IPlayer& p1;
		const TicTacToePlayers::IPlayer& p2;

	public:
		TicTacToeGame(const TicTacToePlayers::IPlayer& first, const TicTacToePlayers::IPlayer& second) : p1(first), p2(second) { };

		void Play()
		{
			if (!Decisions::DecisionTable::wasInitialized)
				Decisions::DecisionTable().Init();
			board = new TBoard();
			while (true)
			{
				board->Print();
				TicTacToeBoard::BoardPoint firstPlayerTurn = p1.decideOnNextMove(*board, TicTacToeBoard::Cross);
				board->SetCross(firstPlayerTurn);
				TicTacToeBoard::GameStatus status = board->ReportAndPrintGameStatus();
				if (status != TicTacToeBoard::GameStatus::NotEnded)
					break;
				TicTacToeBoard::BoardPoint secondPlayerTurn = p2.decideOnNextMove(*board, TicTacToeBoard::Zero);
				board->SetZero(secondPlayerTurn);
				status = board->ReportAndPrintGameStatus();
				if (status != TicTacToeBoard::GameStatus::NotEnded)
					break;
			}
			delete board;
		}
	};
}