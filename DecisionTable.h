#pragma once
#include "Bots.h"
#include "DecisionList.h"
namespace Decisions
{
	class DecisionTable
	{
		static bool WasInitialized;
		static DecisionList* decisionByHash;
		static int GetHash(TicTacToe::Figures** board)
		{
			int hash = 0;
			for (int i = 0; i < 3;i++)
				for (int j = 0;j < 3;j++)
					hash = (hash * 7 + board[i][j]) % maxHash;
			return hash;
		}

		static TicTacToe::GameStatus CheckResult(TicTacToe::Figures** board)
		{
			TicTacToe::GameStatus result = TicTacToe::Tie;
			for (int row = 0; row < 3; row++)
				if (board[row][0] == board[row][1] && board[row][1] == board[row][2] &&
					board[row][0] != TicTacToe::Empty)
					result = board[row][0] == TicTacToe::Cross ? TicTacToe::GameStatus::CrossWon : TicTacToe::GameStatus::ZeroWon;
			for (int col = 0; col < 3; col++)
				if (board[0][col] == board[1][col] && board[1][col] == board[2][col] &&
					board[0][col] != TicTacToe::Empty)
					result = board[0][col] == TicTacToe::Cross ? TicTacToe::GameStatus::CrossWon : TicTacToe::GameStatus::ZeroWon;
			if ((board[1][1] == board[0][0] && board[1][1] == board[2][2] ||
				board[1][1] == board[2][0] && board[1][1] == board[0][2]) &&
				board[1][1] != TicTacToe::Empty)
				result = board[1][1] == TicTacToe::Cross ? TicTacToe::GameStatus::CrossWon : TicTacToe::GameStatus::ZeroWon;
			int cnt = 0;
			for (int i = 0; i < 3;i++)
				for (int j = 0; j < 3;j++)
					if (board[i][j] == TicTacToe::Empty)
						cnt++;
			if (cnt == 0)
				return result;
			return result == TicTacToe::Tie ? TicTacToe::GameStatus::NotEnded : result;
		}

		static void ModifyDecisionByResult(DecisionData& data, TicTacToe::GameStatus result)
		{
			if (result == TicTacToe::CrossWon)
			{
				data.turnsTillCrossWin = 0;
				data.timesCrossWon++;
			}
			if (result == TicTacToe::ZeroWon)
			{
				data.turnsTillZeroWin = 0;
				data.timesZeroWon++;
			}
			if (result == TicTacToe::Tie)
			{
				data.turnsTillTie = 0;
				data.timesTie++;
			}
		}

		static DecisionData TraverseTree(TicTacToe::Figures** board, bool isCrossTurn = true)
		{
			DecisionData minData;
			TicTacToe::GameStatus result = CheckResult(board);
			for (int i = 0; i < 3 && result == TicTacToe::NotEnded;i++)
				for (int j = 0; j < 3;j++)
					if (board[i][j] == TicTacToe::Empty)
					{
						board[i][j] = isCrossTurn ? TicTacToe::Cross : TicTacToe::Zero;
						DecisionData data = TraverseTree(board, !isCrossTurn);
						board[i][j] = TicTacToe::Empty;
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
		static void Init()
		{
			TicTacToe::Figures** board = new TicTacToe::Figures * [3];
			for (int i = 0; i < 3;i++)
				board[i] = new TicTacToe::Figures[3]{ TicTacToe::Empty };
			TraverseTree(board);
			WasInitialized = true;
		}

		DecisionData operator[](TicTacToe::Figures** board)
		{
			return decisionByHash[GetHash(board)].Find(board);
		}
	};

	DecisionList* DecisionTable::decisionByHash = new DecisionList[maxHash];
	bool DecisionTable::WasInitialized = false;
}