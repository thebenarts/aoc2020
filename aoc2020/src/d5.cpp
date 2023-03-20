#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <span>
#include <algorithm>

//#define TEST_CASE 1;

struct Seat
{
	Seat() = default;
	Seat(std::string_view aData)
	{
		ProcessData(aData);
	}

	bool operator<(const Seat& rhs)
	{
		return mSeatId < rhs.mSeatId;
	}

	inline int GetId() const { return mSeatId; };
private:
	void ProcessData(std::string_view);
	void ResolveRow(std::string_view);
	void ResolveCol(std::string_view);
	void ResolveCharacter(std::span<int> range, const char c);
	
	int mRow;
	int mCol;
	int mSeatId;
};

std::vector<Seat> ReadInput()
{
#ifdef TEST_CASE
	std::ifstream input("input/d5test.txt");
#endif
#ifndef TEST_CASE
	std::ifstream input("input/d5.txt");
#endif

	std::vector<Seat>map;
	std::string line;
	while (std::getline(input, line))
	{
		map.emplace_back(line);
	}

	return map;
}

void Seat::ProcessData(std::string_view aData)
{
	// First 7 (0-6) characters contain the information about the row 0-127
	// F == lower half
	// B == upper half
	std::string_view rowData{ aData.substr(0,7) };
	ResolveRow(rowData);

	// Remaining 3 characters contain the information about the col 0-7
	// L == lower half
	// R == upper half
	std::string_view colData{ aData.substr(7) };
	ResolveCol(colData);

	// ResolveID
	mSeatId = mRow * 8 + mCol;
}

void Seat::ResolveCharacter(std::span<int> aNums, const char c)
{
		if (c == 'L' || c == 'F')
		{
			aNums[1] = aNums[1] - ((aNums[1] - aNums[0]) / 2) - 1;
		}
		else
		{
			aNums[0] = (aNums[1] - aNums[0]) / 2 + aNums[0] + 1;	// We don't want to truncate here
		}
}

void Seat::ResolveRow(std::string_view aData)
{
	int row[2]{ 0,127 };
	for (int i = 0; i < 6; i++)
	{
		ResolveCharacter(row, aData[i]);
	}

	mRow = aData[6] == 'F' ? row[0] : row[1];
}

void Seat::ResolveCol(std::string_view aData)
{
	int col[2]{ 0,7 };
	for (int i = 0; i < 2; i++)
	{
		ResolveCharacter(col, aData[i]);
	}

	mCol = aData[2] == 'L' ? col[0] : col[1];
}

int main()
{
	std::vector<Seat> seats;
	seats = std::move(ReadInput());

	std::sort(seats.begin(), seats.end());
	std::cout << "PART 1 : " << seats.back().GetId() << '\n';

	const int seatsSize{ static_cast<int>(seats.size()) };
	int iter{ 0 };
	Seat low;
	Seat high;
	while (iter + 1 < seatsSize)
	{
		if (seats[iter].GetId() + 2 == seats[iter + 1].GetId())
		{
			low = seats[iter];
			high = seats[iter + 1];
			break;
		}
		++iter;
	}

	std::cout << "PART 2 : " << low.GetId() + 1;
}
