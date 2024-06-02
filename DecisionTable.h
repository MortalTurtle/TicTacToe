#pragma once
#include "Bots.h"

namespace TicTacToeBots
{
	const int maxHash = 400031;

	struct DecisionData
	{
		unsigned int timesCrossWon = 0;
		unsigned int timesZeroWon = 0;
		unsigned int timesTie = 0;
		char turnsTillZeroWin = 10;
		char turnsTillCrossWin = 10;
		char turnsTillTie = 10;
		DecisionData() = default;
		DecisionData(char turnsTillZeroWin_, char turnsTillCrossWin_, char turnsTillTie_) :
			turnsTillCrossWin(turnsTillCrossWin_), turnsTillZeroWin(turnsTillZeroWin_), turnsTillTie(turnsTillTie_) {}

		static DecisionData Combine(const DecisionData& first, const DecisionData& second)
		{
			DecisionData sum(std::min(first.turnsTillZeroWin, second.turnsTillZeroWin),
				std::min(first.turnsTillCrossWin, second.turnsTillCrossWin),
				std::min(first.turnsTillTie, second.turnsTillTie));
			sum.timesCrossWon = first.timesCrossWon + second.timesCrossWon;
			sum.timesZeroWon = first.timesZeroWon + second.timesZeroWon;
			sum.timesTie = first.timesTie + second.timesTie;
			return sum;
		}
	};

	struct DecisionNode
	{
		TicTacToe::Figures** board = new TicTacToe::Figures* [3];
		DecisionData data;
		DecisionNode* next = nullptr;
		DecisionNode(const DecisionData& data, TicTacToe::Figures** board_) : data(data)
		{
			for (int i = 0; i < 3;i++)
			{
				board[i] = new TicTacToe::Figures[3];
				for (int j = 0; j < 3;j++)
					board[i][j] = board_[i][j];
			}
		}
	};

	class DecisionList
	{
		DecisionNode* head = nullptr;
		DecisionNode* tail = nullptr;
		bool cmp(TicTacToe::Figures** board1, TicTacToe::Figures** board2)
		{
			for (int i = 0; i < 3;i++)
				for (int j = 0; j < 3;j++)
					if (board1[i][j] != board2[i][j])
						return false;
			return true;
		}
	public:
		void Push(DecisionData data, TicTacToe::Figures** board)
		{
			DecisionNode* next = new DecisionNode(data, board);
			if (tail != nullptr)
				tail->next = next;
			else head = next;
			tail = next;
		}

		DecisionData Find(TicTacToe::Figures** board)
		{
			DecisionNode* currentNode = head;
			while (currentNode != nullptr)
			{
				if (cmp(board, currentNode->board))
					return currentNode->data;
				currentNode = currentNode->next;
			}
			return DecisionData();
		}
	};

	class DecisionTable
	{
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
		static void BuildTree()
		{
			TicTacToe::Figures** board = new TicTacToe::Figures * [3];
			for (int i = 0; i < 3;i++)
				board[i] = new TicTacToe::Figures[3]{ TicTacToe::Empty };
			TraverseTree(board);
			int x = 2;
		}

		DecisionData operator[](TicTacToe::Figures** board)
		{
			return decisionByHash[GetHash(board)].Find(board);
		}
	};

	DecisionList* DecisionTable::decisionByHash = new DecisionList[maxHash];
}