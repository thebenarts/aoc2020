#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

//#define TEST_CASE 1;
//#define PART1 1;

int accumulator{ 0 };
int iterator{ 0 };
int swapIter{0};
std::vector<int> swappable;

int add(int aValue)
{
	return aValue;
}

int negate(int aValue)
{
	return -aValue;
}

enum OP_TYPE : uint8_t
{
	ACC,
	JMP,
	NOP
};

struct Operation
{
	Operation(std::string_view aData)
	{
		ProcessData(aData);
	}

	bool operator()()
	{
		if (hasBeenHit)
		{
#ifdef PART1
			std::cout << accumulator;
#endif
			return false;
		}
		hasBeenHit = true;

		if (mType == OP_TYPE::ACC)
		{
			accumulator += mValue;
			++iterator;
		}
		else if (mType == OP_TYPE::JMP)
		{
			if (mValue == 0)
			{
				iterator++;
			}
			else
			{
				iterator += mValue;
			}
		}
		else if (mType == OP_TYPE::NOP)
		{
			iterator++;
		}

		return true;
	};

	void Clear()
	{
		hasBeenHit = false;
	}

	void Swap()
	{
		if (mType == OP_TYPE::JMP)
		{
			mType = OP_TYPE::NOP;
		}
		else if (mType == OP_TYPE::NOP)
		{
			mType = OP_TYPE::JMP;
		}
	}

	void ProcessData(std::string_view aData);

	OP_TYPE mType;
	int mValue;
	bool hasBeenHit{ false };
};

void Operation::ProcessData(std::string_view aData)
{
	// Get the type of operation
	const size_t spaceLoc{ aData.find(' ') };
	std::string_view type{ aData.data(), aData.data() + spaceLoc };
	auto assignType = [this, type]() {
		if (type == "acc")
		{
			mType = OP_TYPE::ACC;
		}
		else if (type == "jmp")
		{
			mType = OP_TYPE::JMP;
		}
		else if (type == "nop")
		{
			mType = OP_TYPE::NOP;
		}
	};
	assignType();

	// Get +/- and the value
	std::string_view numberString{ aData.substr(spaceLoc + 1) };
	auto ToNumber = [this, numberString]() {
		int number{ 0 };

		int (*op)(int);
		if (numberString[0] == '+')
		{
			op = &add;
		}
		else
		{
			op = &negate;
		}

		for (int i = 1; i < numberString.size(); ++i)
		{
			number *= 10;
			number += static_cast<int>(numberString[i] & 0x0F);
		}
		
		mValue = op(number);
	};
	ToNumber();
}

std::vector<Operation> ReadInput()
{
#ifdef TEST_CASE
	std::ifstream input("input/d8test.txt");
#endif
#ifndef TEST_CASE
	std::ifstream input("input/d8.txt");
#endif

	std::vector<Operation> operations;
	std::string line;
	while (std::getline(input, line))
	{
		operations.emplace_back(line);
		// Store NOP and JMP operation iterators
		if (operations.back().mType == OP_TYPE::JMP || operations.back().mType == OP_TYPE::NOP)
		{
			swappable.push_back(operations.size()-1);
		}
	}

	return operations;
}

int main()
{
	std::vector<Operation> operations{ std::move(ReadInput()) };

#ifdef PART1
	while (iterator < operations.size() && iterator >= 0 && operations[iterator]())
	{
	}
#endif

#ifndef PART1
	while (iterator < operations.size())
	{
		// Clear previous results
		for (Operation& op : operations)
		{
			op.Clear();
		}
		accumulator = 0;
		iterator = 0;
		
		if (swapIter > 0)
		{
			operations[swappable[swapIter-1]].Swap();	// Swap back previous operation
		}
		operations[swappable[swapIter]].Swap();
		++swapIter;

		while (iterator < operations.size() && iterator >= 0 && operations[iterator]())
		{
		}
	
	}

	std::cout << accumulator;
#endif

	return 0;
}