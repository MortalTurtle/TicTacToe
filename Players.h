#pragma once
#include "Board.h"
#include <iostream>
#include <concepts>

namespace TicTacToe
{
	class IPlayer
	{
	public:
		virtual BoardPoint decideOnNextMove(const IBoard& board, Figures playerFigure) const abstract;
	};

	class IHumanPlayerIO
	{
	public:
		virtual BoardPoint GetHumanTurnPoint() const abstract;
		virtual void ReportIncorrectTurnInput() const abstract;
	};

	class HumanConsoleIO : public IHumanPlayerIO
	{
	public:
		BoardPoint GetHumanTurnPoint() const override 
		{
			BoardPoint p;
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
		BoardPoint decideOnNextMove(const IBoard& board, Figures playerFigure) const override
		{
			BoardPoint p = playerIo->GetHumanTurnPoint();
			while (p.row < 0 || p.row > 2 || p.col < 0 || p.col > 2
				|| board.GetBoard()[p.row][p.col] != Figures::Empty)
			{
				playerIo->ReportIncorrectTurnInput();
				p = playerIo->GetHumanTurnPoint();
			}
			return p;
		}
	};
}
