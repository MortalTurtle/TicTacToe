#pragma once
#include "Board.h"
#include "DecisionTable.h"

namespace TicTacToeBots
{
	class EasyBot : public TicTacToe::IPlayer
	{
	public:
		TicTacToe::BoardPoint decideOnNextMove(const TicTacToe::IBoard& board, TicTacToe::Figures playerFigure) const override
		{
			TicTacToe::Figures** boardSimple = board.GetBoard();
			TicTacToe::BoardPoint worstMove(-1, 0);
			int worstTurns = 0;

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (boardSimple[i][j] == TicTacToe::Empty)
					{
						boardSimple[i][j] = playerFigure;
						Decisions::DecisionData data = Decisions::DecisionTable()[boardSimple];
						boardSimple[i][j] = TicTacToe::Empty;
						int turns = 0;
						if (playerFigure == TicTacToe::Cross) turns = data.turnsTillCrossWin;
						else turns = data.turnsTillZeroWin;

						if (worstMove.row == -1 || turns > worstTurns)
						{
							worstMove.row = i;
							worstMove.col = j;
							worstTurns = turns;
						}
					}
			return worstMove;
		}
	};

	class MediumBot : public TicTacToe::IPlayer
	{
	public:
		TicTacToe::BoardPoint decideOnNextMove(const TicTacToe::IBoard& board, TicTacToe::Figures playerFigure) const override
		{
			TicTacToe::Figures** boardSimple = board.GetBoard();
			int emptyCount = 0;
			int emptyIndices[9][2];

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (boardSimple[i][j] == TicTacToe::Empty)
					{
						emptyIndices[emptyCount][0] = i;
						emptyIndices[emptyCount][1] = j;
						emptyCount++;
					}

			int randomIndex = (int)((double)rand() / RAND_MAX * emptyCount);
			return TicTacToe::BoardPoint(emptyIndices[randomIndex][0], emptyIndices[randomIndex][1]);
		}
	};

	class HardBot : public TicTacToe::IPlayer
	{
	public:
		TicTacToe::BoardPoint decideOnNextMove(const TicTacToe::IBoard& board, TicTacToe::Figures playerFigure) const override
		{
			Decisions::DecisionData minData;
			TicTacToe::Figures** boardSimple = board.GetBoard();
			TicTacToe::BoardPoint bestP(-1, 0);
			for (int i = 0; i < 3;i++)
				for (int j = 0; j < 3;j++)
					if (boardSimple[i][j] == TicTacToe::Empty)
					{
						boardSimple[i][j] = playerFigure;
						Decisions::DecisionData data = Decisions::DecisionTable()[boardSimple];

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