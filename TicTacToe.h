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
		IBoard* board;
		const IPlayer& p1;
		const IPlayer& p2;

	public:
		TicTacToeGame(const IPlayer& first, const IPlayer& second) : p1(first), p2(second) { };

		void Play()
		{
			if (!Decisions::DecisionTable::wasInitialized)
				Decisions::DecisionTable().Init();
			board = new TBoard();
			while (true)
			{
				board->Print();
				BoardPoint firstPlayerTurn = p1.decideOnNextMove(*board, Cross);
				board->SetCross(firstPlayerTurn);
				GameStatus status = board->ReportAndPrintGameStatus();
				if (status != GameStatus::NotEnded)
					break;
				BoardPoint secondPlayerTurn = p2.decideOnNextMove(*board, Zero);
				board->SetZero(secondPlayerTurn);
				status = board->ReportAndPrintGameStatus();
				if (status != GameStatus::NotEnded)
					break;
			}
			delete board;
		}
	};
}