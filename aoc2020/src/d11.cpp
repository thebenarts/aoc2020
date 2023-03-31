#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

//#define USE_TEST_CASE 1;

std::vector<std::vector<char>> ReadInput()
{
#ifdef USE_TEST_CASE
	std::ifstream input("input/d11test.txt");
#endif
#ifndef USE_TEST_CASE
	std::ifstream input("input/d11.txt");
#endif

	std::vector<std::vector<char>> result;
	std::string line;

	while (std::getline(input, line))
	{
		static const size_t lineSize{ line.size() };

		std::vector<char> vectorLine;
		for (const char& c : line)
		{
			vectorLine.emplace_back(c);
		}
		result.push_back(std::move(vectorLine));
	}

	return result;
}

int moves[8][2]{
	{0,-1},		// left
	{0, 1},		// right
	{1,0},		// down
	{-1, 0},	// up
	{-1,-1},	// top-left
	{-1,1},		// top-right
	{1,-1},		// bottom-left
	{1,1}		// bottom-right
};

enum MOVE_TYPE : int
{
	LEFT = 0,
	RIGHT = 1,
	DOWN = 2,
	UP = 3,
	TOP_LEFT = 4,
	TOP_RIGHT = 5,
	BOTTOM_LEFT = 6,
	BOTTOM_RIGHT = 7,
};

bool CheckBounds(int row, int col, int rowSize, int colSize)
{
	if (row < 0 || row >= rowSize || col < 0 || col >= colSize)
	{
		return false;
	}

	return true;
}

char FindFirstSeat(const std::vector<std::vector<char>>& seats, int row, int col, MOVE_TYPE movType)
{
	static const int rowSize{ static_cast<int>(seats.size()) };
	static const int colSize{ static_cast<int>(seats[0].size()) };
	if (!CheckBounds(row, col, rowSize, colSize))
	{
		return '.';
	}

	if (seats[row][col] != '.')
	{
		return seats[row][col];
	}

	return FindFirstSeat(seats, row + moves[movType][0], col + moves[movType][1], movType);
}

//#define PART1 1;
std::vector<std::vector<char>> ApplyRules(std::vector<std::vector<char>>& seats)
{
	/* All decisions are based on the number of occupied seats adjacent to a given seat 
	(one of the eight positions immediately up, down, left, right, or diagonal from the seat). 
	The following rules are applied to every seat SIMULTANEOUSLY:*/
	std::vector<std::vector<char>> copy{ seats };
	const int rowSize{ static_cast<int>(seats.size()) };
	const int colSize{ static_cast<int>(seats[0].size()) };

	// - If a seat is empty (L) and there are no occupied seats adjacent to it, the seat becomes occupied.
	auto CheckNotOccupied = [rowSize, colSize, &copy](int row, int col) {
		for (int i = 0; i != 8; i++)
		{
#ifdef PART1
			int newRow{ row + moves[i][0] };
			int newCol{ col + moves[i][1] };
			if (CheckBounds(newRow, newCol, rowSize, colSize))
			{
				if (copy[newRow][newCol] == '#')
				{
					return false;
				}
			}
#endif
#ifndef PART1
			if ('#' == FindFirstSeat(copy, row + moves[i][0], col + moves[i][1], static_cast<MOVE_TYPE>(i)))
			{
				return false;
			}
#endif
		}

		return true;
	};

	// - If a seat is occupied (#) and four or more seats adjacent to it are also occupied, the seat becomes empty.
	auto CheckOccupied = [rowSize, colSize, &copy](int row, int col){
		int numberOfAdjecentOccupied{ 0 };
		for (int i = 0; i != 8; i++)
		{
#ifdef PART1
			int newRow{ row + moves[i][0] };
			int newCol{ col + moves[i][1] };
			if (CheckBounds(newRow, newCol, rowSize, colSize))
			{
				if (copy[newRow][newCol] == '#')
				{
					numberOfAdjecentOccupied++;
				}
			}
#endif
#ifndef PART1
			if ('#' == FindFirstSeat(copy, row + moves[i][0], col + moves[i][1], static_cast<MOVE_TYPE>(i)))
			{
				numberOfAdjecentOccupied++;
			}
#endif
		}

#ifdef PART1
		return numberOfAdjecentOccupied >= 4 ? true : false;
#endif 
#ifndef PART1
		return numberOfAdjecentOccupied >= 5 ? true : false;
#endif PART1
	};

	for (int row = 0; row < seats.size(); row++)
	{
		for (int col = 0; col < seats[0].size(); col++)
		{
			if (seats[row][col] == 'L')
			{
				if(CheckNotOccupied(row, col))
				{
					seats[row][col] = '#';
				}
			}
			else if (seats[row][col] == '#')
			{
				if (CheckOccupied(row, col))
				{
					seats[row][col] = 'L';
				}
			}
			// - Otherwise, the seat's state does not change.
		}
	}

	return copy;
}

int CountOccupied(const std::vector<std::vector<char>>& seats)
{
	int occupied{ 0 };
	for (const std::vector<char>& line : seats)
	{
		for (const char& c : line)
		{
			if (c == '#')
			{
				occupied++;
			}
		}
	}

	return occupied;
}

int main()
{

	std::vector<std::vector<char>> seats{ std::move(ReadInput()) };
	std::vector<std::vector<char>> copy;
	
	while (seats != copy)
	{
		copy = std::move(ApplyRules(seats));
	}

	std::cout << CountOccupied(seats);

}