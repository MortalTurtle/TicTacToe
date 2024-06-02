#pragma once
#include "Board.h"
#include "DecisionTree.h"

namespace TicTacToeBots
{
	class EasyBot : public TicTacToe::IPlayer
	{
	public:
		TicTacToe::BoardPoint decideOnNextMove(const TicTacToe::IBoard& board, TicTacToe::Figures playerFigure) const override
		{
			//...
		}
	};

	class MediumBot : public TicTacToe::IPlayer
	{
	public:
		TicTacToe::BoardPoint decideOnNextMove(const TicTacToe::IBoard& board, TicTacToe::Figures playerFigure) const override
		{
			//...
		}
	};

	class HardBot : public TicTacToe::IPlayer
	{
	public:
		TicTacToe::BoardPoint decideOnNextMove(const TicTacToe::IBoard& board, TicTacToe::Figures playerFigure) const override
		{
			DecisionData minData;
			TicTacToe::Figures** boardSimple = board.GetBoard();
			TicTacToe::BoardPoint bestP(-1, 0);
			for (int i = 0; i < 3;i++)
				for (int j = 0; j < 3;j++)
					if (boardSimple[i][j] == TicTacToe::Empty)
					{
						boardSimple[i][j] = playerFigure;
						DecisionData data = DecisionTree()[boardSimple];

						bool shouldChangeDecisionIfCross = bestP.row == -1 || (playerFigure == TicTacToe::Cross &&
							(minData.turnsTillCrossWin > data.turnsTillCrossWin && data.turnsTillZeroWin > data.turnsTillCrossWin ||
								data.turnsTillZeroWin >= minData.turnsTillZeroWin));

						bool shouldChangeDecisionIfZero = (playerFigure == TicTacToe::Zero &&
							(minData.turnsTillZeroWin > data.turnsTillZeroWin && data.turnsTillCrossWin > data.turnsTillZeroWin ||
								data.turnsTillCrossWin >= minData.turnsTillCrossWin));
						if (shouldChangeDecisionIfCross || shouldChangeDecisionIfZero ||
							playerFigure == TicTacToe::Cross && data.turnsTillCrossWin == minData.turnsTillCrossWin &&
							data.timesCrossWon > minData.timesCrossWon ||
							playerFigure == TicTacToe::Zero && data.turnsTillZeroWin == minData.turnsTillZeroWin &&
							data.timesZeroWon > minData.timesZeroWon)
						{
							minData = data;
							bestP.row = i;
							bestP.col = j;
						}
						boardSimple[i][j] = TicTacToe::Empty;
					}
			return bestP;
		}
	};
}