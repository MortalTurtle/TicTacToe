#pragma once
#include "Board.h"
#include <iostream>
#include <concepts>

namespace TicTacToePlayers
{
	class IPlayer
	{
	public:
		virtual TicTacToeBoard::BoardPoint decideOnNextMove(const TicTacToeBoard::IBoard& board, TicTacToeBoard::Figures playerFigure) const abstract;
		virtual ~IPlayer()
		{}
	};

	class IHumanPlayerIO
	{
	public:
		virtual TicTacToeBoard::BoardPoint GetHumanTurnPoint() const abstract;
		virtual void ReportIncorrectTurnInput() const abstract;
		virtual ~IHumanPlayerIO()
		{}
	};

	class HumanConsoleIO : public IHumanPlayerIO
	{
	public:
		TicTacToeBoard::BoardPoint GetHumanTurnPoint() const override
		{
			TicTacToeBoard::BoardPoint p;
			std::cin >> p.row >> p.col;
			return p;
		}
		void ReportIncorrectTurnInput() const override
		{
			std::cout << "Something went wrong, maybe your turn is incorrect" << std::endl;
		}
	};

	template <std::derived_from<IHumanPlayerIO> TPlayerIO>
	class HumanPlayer : public IPlayer
	{
		IHumanPlayerIO* playerIo = new TPlayerIO();
	public:
		~HumanPlayer()
		{
			delete playerIo;
		}
		TicTacToeBoard::BoardPoint decideOnNextMove(const TicTacToeBoard::IBoard& board, TicTacToeBoard::Figures playerFigure) const override
		{
			TicTacToeBoard::BoardPoint p = playerIo->GetHumanTurnPoint();
			while (p.row < 0 || p.row > 2 || p.col < 0 || p.col > 2
				|| board.GetBoard()[p.row][p.col] != TicTacToeBoard::Figures::Empty)
			{
				playerIo->ReportIncorrectTurnInput();
				p = playerIo->GetHumanTurnPoint();
			}
			return p;
		}
	};
}
