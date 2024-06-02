#pragma once
#include "Board.h"

namespace Decisions
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
		TicTacToe::Figures** board = new TicTacToe::Figures * [3];
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
}