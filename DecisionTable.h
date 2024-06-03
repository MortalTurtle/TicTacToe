#pragma once
#include "DecisionList.h"

namespace Decisions
{
	class DecisionTable
	{
		static DecisionList* decisionByHash;
		static int GetHash(TicTacToeBoard::Figures** board)
		{
			int hash = 0;
			for (int i = 0; i < 3;i++)
				for (int j = 0;j < 3;j++)
					hash = (hash * 7 + board[i][j]) % maxHash;
			return hash;
		}

		static TicTacToeBoard::GameStatus CheckResult(TicTacToeBoard::Figures** board)
		{
			TicTacToeBoard::GameStatus result = TicTacToeBoard::Tie;
			for (int row = 0; row < 3; row++)
				if (board[row][0] == board[row][1] && board[row][1] == board[row][2] &&
					board[row][0] != TicTacToeBoard::Empty)
					result = board[row][0] == TicTacToeBoard::Cross ? 
					TicTacToeBoard::GameStatus::CrossWon : TicTacToeBoard::GameStatus::ZeroWon;
			for (int col = 0; col < 3; col++)
				if (board[0][col] == board[1][col] && board[1][col] == board[2][col] &&
					board[0][col] != TicTacToeBoard::Empty)
					result = board[0][col] == TicTacToeBoard::Cross ? TicTacToeBoard::GameStatus::CrossWon : TicTacToeBoard::GameStatus::ZeroWon;
			if ((board[1][1] == board[0][0] && board[1][1] == board[2][2] ||
				board[1][1] == board[2][0] && board[1][1] == board[0][2]) &&
				board[1][1] != TicTacToeBoard::Empty)
				result = board[1][1] == TicTacToeBoard::Cross ? TicTacToeBoard::GameStatus::CrossWon : TicTacToeBoard::GameStatus::ZeroWon;
			int cnt = 0;
			for (int i = 0; i < 3;i++)
				for (int j = 0; j < 3;j++)
					if (board[i][j] == TicTacToeBoard::Empty)
						cnt++;
			if (cnt == 0)
				return result;
			return result == TicTacToeBoard::Tie ? TicTacToeBoard::GameStatus::NotEnded : result;
		}

		static void ModifyDecisionByResult(DecisionData& data, TicTacToeBoard::GameStatus result)
		{
			if (result == TicTacToeBoard::CrossWon)
			{
				data.turnsTillCrossWin = 0;
				data.timesCrossWon++;
			}
			if (result == TicTacToeBoard::ZeroWon)
			{
				data.turnsTillZeroWin = 0;
				data.timesZeroWon++;
			}
			if (result == TicTacToeBoard::Tie)
			{
				data.turnsTillTie = 0;
				data.timesTie++;
			}
		}

		static DecisionData TraverseTree(TicTacToeBoard::Figures** board, bool isCrossTurn = true)
		{
			DecisionData minData;
			TicTacToeBoard::GameStatus result = CheckResult(board);
			for (int i = 0; i < 3 && result == TicTacToeBoard::NotEnded;i++)
				for (int j = 0; j < 3;j++)
					if (board[i][j] == TicTacToeBoard::Empty)
					{
						board[i][j] = isCrossTurn ? TicTacToeBoard::Cross : TicTacToeBoard::Zero;
						DecisionData data = TraverseTree(board, !isCrossTurn);
						board[i][j] = TicTacToeBoard::Empty;
						minData = DecisionData::Combine(minData, data);
					}
			ModifyDecisionByResult(minData, result);
			decisionByHash[GetHash(board)].Push(minData, board);
			minData.turnsTillCrossWin++;
			minData.turnsTillZeroWin++;
			minData.turnsTillTie++;
			return minData;
		}

	public:
		static bool wasInitialized;

		static void Init()
		{
			TicTacToeBoard::Figures** board = new TicTacToeBoard::Figures * [3];
			for (int i = 0; i < 3;i++)
				board[i] = new TicTacToeBoard::Figures[3]{ TicTacToeBoard::Empty };
			TraverseTree(board);
			wasInitialized = true;
		}

		DecisionData operator[](TicTacToeBoard::Figures** board)
		{
			return decisionByHash[GetHash(board)].Find(board);
		}
	};

	DecisionList* DecisionTable::decisionByHash = new DecisionList[maxHash];
	bool DecisionTable::wasInitialized = false;
}