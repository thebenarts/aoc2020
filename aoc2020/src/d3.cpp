#include <iostream>	
#include <string>
#include <fstream>
#include <vector>

//#define TESTDATA 1;
void ReadInput(std::vector<std::vector<int>>& aGrid)
{
#ifdef TESTDATA
	std::ifstream input("input/d3test.txt");
#endif
#ifndef TESTDATA
	std::ifstream input("input/d3.txt");
#endif
	std::string line;

	// read input
	while (std::getline(input, line))
	{
		static const int colSize{ static_cast<int>(line.size()) };
		std::vector<int> row;
		row.reserve(colSize);
		
		for (const char c : line)
		{
			if (c == '.')
			{
				row.emplace_back(0);
			}
			else
			{
				row.emplace_back(1);
			}
		}

		aGrid.emplace_back(std::move(row));
	}
};

struct Vec2D
{
	int row{};
	int col{};
};

int main()
{
	std::vector<std::vector<int>> grid;
	ReadInput(grid);

	// movement
	Vec2D movements[5]{ {1,1},{1,3},{1,5},{1,7},{2,1} };
	int routeResults[5];
	memset(routeResults, 0, sizeof(int) * 5);

	Vec2D bounds{ grid.size(),grid[0].size() };


	for (int i = 0; i != sizeof(routeResults)/sizeof(int);i++)
	{
		const Vec2D& movement = movements[i];
		int x = movement.col;
		int y = movement.row;

		Vec2D pos;

		auto Move = [&pos, bounds, x, y]()-> bool {

			if (pos.row + y >= bounds.row)
			{
				return false;
			}

			pos.row += y;
			pos.col = (pos.col + x) % bounds.col;

			return true;
		};

		while (Move())
		{
			if (grid[pos.row][pos.col])
			{
				++routeResults[i];
			}
		}
	}

	for (int i = 0; i != 5; i++)
		std::cout << "trees encountered on slope " << i << ": " << routeResults[i] << '\n';

	unsigned int occurences{static_cast<unsigned int>(routeResults[0])};
	for (int i = 1; i < sizeof(routeResults)/sizeof(int); i++)
	{
		occurences *= static_cast<unsigned int>(routeResults[i]);
	}

	std::cout << occurences << '\n';
}
