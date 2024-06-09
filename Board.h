#pragma once
#include <iostream>

namespace TicTacToeBoard
{
	struct BoardPoint
	{
		int row, col;
		BoardPoint() = default;
		BoardPoint(int row_, int col_) : row(row_), col(col_) { };
	};

	enum Figures
	{
		Empty,
		Cross,
		Zero
	};

	enum GameStatus
	{
		NotEnded,
		CrossWon,
		ZeroWon,
		Tie
	};

	class IBoard
	{
	public:
		virtual ~IBoard()
		{}
		virtual void SetCross(const BoardPoint& p) abstract;

		virtual void SetZero(const BoardPoint& p) abstract;

		virtual Figures** GetBoard() const abstract;

		virtual GameStatus ReportAndPrintGameStatus() const abstract;

		virtual void Print() const abstract;
	};

	class BoardConsole : public IBoard
	{
		Figures** board = new Figures*[3];
		int emptyCnt = 9;
		GameStatus status = GameStatus::NotEnded;

		void CheckGameStatus()
		{
			GameStatus result = Tie;
			for (int row = 0; row < 3; row++)
				if (board[row][0] == board[row][1] && board[row][1] == board[row][2] &&
					board[row][0] != Empty)
					result = board[row][0] == Cross ? GameStatus::CrossWon : GameStatus::ZeroWon;
			for (int col = 0; col < 3; col++)
				if (board[0][col] == board[1][col] && board[1][col] == board[2][col] &&
					board[0][col] != Empty)
					result = board[0][col] == Cross ? GameStatus::CrossWon : GameStatus::ZeroWon;
			if ((board[1][1] == board[0][0] && board[1][1] == board[2][2] ||
				board[1][1] == board[2][0] && board[1][1] == board[0][2]) &&
				board[1][1] != Empty)
				result = board[1][1] == Cross ? GameStatus::CrossWon : GameStatus::ZeroWon;
			int cnt = 0;
			for (int i = 0; i < 3;i++)
				for (int j = 0; j < 3;j++)
					if (board[i][j] == Empty)
						cnt++;
			if (cnt == 0)
				status = result;
			else status = result == Tie ? GameStatus::NotEnded : result;
		}
	public:
		BoardConsole()
		{
			for (int i = 0; i < 3;i++)
				board[i] = new Figures[3]{ Empty };
		}
		~BoardConsole()
		{
			delete[] board;
		}
		void SetCross(const BoardPoint& p) override
		{
			board[p.row][p.col] = Cross;
			emptyCnt--;
			CheckGameStatus();
		}

		void SetZero(const BoardPoint& p) override
		{
			board[p.row][p.col] = Zero;
			emptyCnt--;
			CheckGameStatus();
		}

		Figures** GetBoard() const override
		{
			return board;
		}
		GameStatus ReportAndPrintGameStatus() const override
		{
			if (status != GameStatus::NotEnded)
			{
				Print();
				if (status == GameStatus::CrossWon)
					std::cout << "Cross won!" << std::endl;
				else if (status == GameStatus::ZeroWon)
					std::cout << "Zero won!" << std::endl;
				else std::cout << "Tie! Friendship won" << std::endl;
			}
			return status;
		}
		void Print() const override
		{
			std::cout << "    0 1 2" << std::endl;
			for (int i = 0; i < 3;i++)
			{
				std::cout << i << " : ";
				for (int j = 0; j < 3;j++)
				{
					std::cout << (board[i][j] == Figures::Zero ? "O" : board[i][j] == Figures::Cross ? "X" : " ");
					if (j != 2)
						std::cout << "|";
					else std::cout << std::endl;
				}
				std::cout << "    -----" << std::endl;
			}
		}
	};
}