#pragma once
#include "Board.h"
#include "DecisionTable.h"

namespace TicTacToeBots
{
	class EasyBot : public TicTacToePlayers::IPlayer
	{
	public:
		TicTacToeBoard::BoardPoint decideOnNextMove(const TicTacToeBoard::IBoard& board, TicTacToeBoard::Figures playerFigure) const override
		{
			TicTacToeBoard::Figures** boardSimple = board.GetBoard();
			TicTacToeBoard::BoardPoint worstMove(-1, 0);
			int worstTurns = 0;

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (boardSimple[i][j] == TicTacToeBoard::Empty)
					{
						boardSimple[i][j] = playerFigure;
						Decisions::DecisionData data = Decisions::DecisionTable()[boardSimple];
						boardSimple[i][j] = TicTacToeBoard::Empty;
						int turns = 0;
						if (playerFigure == TicTacToeBoard::Cross) turns = data.turnsTillCrossWin;
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

	class MediumBot : public TicTacToePlayers::IPlayer
	{
	public:
		TicTacToeBoard::BoardPoint decideOnNextMove(const TicTacToeBoard::IBoard& board, TicTacToeBoard::Figures playerFigure) const override
		{
			TicTacToeBoard::Figures** boardSimple = board.GetBoard();
			int emptyCount = 0;
			int emptyIndices[9][2];

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (boardSimple[i][j] == TicTacToeBoard::Empty)
					{
						emptyIndices[emptyCount][0] = i;
						emptyIndices[emptyCount][1] = j;
						emptyCount++;
					}

			int randomIndex = (int)((double)rand() / RAND_MAX * emptyCount);
			return TicTacToeBoard::BoardPoint(emptyIndices[randomIndex][0], emptyIndices[randomIndex][1]);
		}
	};

	class HardBot : public TicTacToePlayers::IPlayer
	{
	public:
		TicTacToeBoard::BoardPoint decideOnNextMove(const TicTacToeBoard::IBoard& board, TicTacToeBoard::Figures playerFigure) const override
		{
			Decisions::DecisionData minData;
			TicTacToeBoard::Figures** boardSimple = board.GetBoard();
			TicTacToeBoard::BoardPoint bestP(-1, 0);
			for (int i = 0; i < 3;i++)
				for (int j = 0; j < 3;j++)
					if (boardSimple[i][j] == TicTacToeBoard::Empty)
					{
						boardSimple[i][j] = playerFigure;
						Decisions::DecisionData data = Decisions::DecisionTable()[boardSimple];

						bool shouldChangeDecisionIfCross = bestP.row == -1 || (playerFigure == TicTacToeBoard::Cross &&
							(minData.turnsTillCrossWin > data.turnsTillCrossWin && data.turnsTillZeroWin > data.turnsTillCrossWin ||
								data.turnsTillZeroWin >= minData.turnsTillZeroWin));

						bool shouldChangeDecisionIfZero = (playerFigure == TicTacToeBoard::Zero &&
							(minData.turnsTillZeroWin > data.turnsTillZeroWin && data.turnsTillCrossWin > data.turnsTillZeroWin ||
								data.turnsTillCrossWin >= minData.turnsTillCrossWin));
						if (shouldChangeDecisionIfCross || shouldChangeDecisionIfZero ||
							playerFigure == TicTacToeBoard::Cross && data.turnsTillCrossWin == minData.turnsTillCrossWin &&
							data.timesCrossWon > minData.timesCrossWon ||
							playerFigure == TicTacToeBoard::Zero && data.turnsTillZeroWin == minData.turnsTillZeroWin &&
							data.timesZeroWon > minData.timesZeroWon)
						{
							minData = data;
							bestP.row = i;
							bestP.col = j;
						}
						boardSimple[i][j] = TicTacToeBoard::Empty;
					}
			return bestP;
		}
	};
}