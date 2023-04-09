#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <span>
#include <ranges>
#include <vector>
#include <assert.h>

struct Instruction;
using OPERATION = void (*)(const Instruction& aInstruction);

// The waypoint starts 10 units east and 1 unit north
#ifndef PART1
int waypointX{10};
int waypointY{1};
#endif

int ToNumber(std::string_view aValue)
{
	int result{};
	for (const char& digit : aValue)
	{
		result *= 10;
		result += static_cast<int>(digit & 0x0F);
	}

	return result;
}

struct Instruction
{
	enum OPERATION_TYPE : int8_t{
		INVALID = -1,
		N = 0,
		S = 1,
		E = 2,
		W = 3,
		L = 4,
		R = 5,
		F = 6,
	};

	Instruction(std::string_view aData)
	{
		assert(aData.size() >= 2);

		// Determine operation	
		const char opChar{ *aData.data()};
		AssignOperation(opChar);

		// Get Value
		std::string_view value{ aData.substr(1) };
		m_value = ToNumber(value);
	}

	void PerformOperation() const
	{
		m_operation(*this);
	}

	void AssignOperation(const char aChar);

	OPERATION m_operation;
	OPERATION_TYPE m_opType;
	int m_value;
};

struct Ship
{
	static Ship& Instance()
	{
		static Ship* instance{ new Ship() };
		return *instance;
	}

	static void Move(const Instruction& aInstruction)
	{
		switch (aInstruction.m_opType)
		{
#ifdef PART1
		case Instruction::N:
			m_vertical += aInstruction.m_value;
			break;
		case Instruction::S:
			m_vertical -= aInstruction.m_value;
			break;
		case Instruction::E:
			m_horizontal += aInstruction.m_value;
			break;
		case Instruction::W:
			m_horizontal -= aInstruction.m_value;
			break;
#else	// PART2
		case Instruction::N:
			waypointY += aInstruction.m_value;
			break;
		case Instruction::S:
			waypointY -= aInstruction.m_value;
			break;
		case Instruction::E:
			waypointX += aInstruction.m_value;
			break;
		case Instruction::W:
			waypointX -= aInstruction.m_value;
			break;
#endif
		case Instruction::F:
			Move_(aInstruction.m_value);
			break;
		}
	}

	static void Turn(const Instruction& aInstruction)
	{
		int n{ aInstruction.m_value / 90 };	// FOR PART2... 
		switch (aInstruction.m_opType)
		{
#ifdef PART1
		case Instruction::L:
			m_direction -= aInstruction.m_value;
			if (m_direction < 0)		// we would have to handle -90 and 90 seperately if we didn't wrap around
			{
				m_direction += 360;
			}
			break;
		case Instruction::R:
			m_direction += aInstruction.m_value;
			break;
#else	// PART2 
		case Instruction::L:
			for (int i = 0; i < n; i++)
			{
				int temp{ waypointX };
				waypointX = -waypointY;
				waypointY = temp;
			}
			break;
		case Instruction::R:
			for (int i = 0; i < n; i++)
			{
				int temp{ waypointX };
				waypointX = waypointY;
				waypointY = -temp;
			}
			break;
#endif
		}

	}

	static int GetManhattanDistance()
	{
		return std::abs(m_horizontal) + std::abs(m_vertical);
	}

	static int m_horizontal;
	static int m_vertical;
	static int m_direction;

private:
	static void Move_(int aValue)
	{
#ifdef PART1
		int dir{ m_direction % 360 };
		dir /= 90;
		// 1 == East
		// 2 == South
		// 3 == West
		switch (dir)
		{
		case 0 :	// 0 == North
			m_vertical += aValue;
			break;
		case 1 :	// 1 == East 
			m_horizontal += aValue;
			break;
		case 2 :	// 0 == South 
			m_vertical -= aValue;
			break;
		case 3 :	// 3 == West 
			m_horizontal -= aValue;
			break;
		}
#else
		for (int i = 0; i < aValue; i++)
		{
			m_horizontal += waypointX;
			m_vertical += waypointY;
		}
#endif
	}
};

int Ship::m_horizontal{ 0 };
int Ship::m_vertical{ 0 };
int Ship::m_direction{ 90 };	

void Instruction::AssignOperation(const char aChar)
{
		switch (aChar)
		{
		case 'N':
			m_opType = OPERATION_TYPE::N;
			m_operation = Ship::Instance().Move;
			break;
		case 'S':
			m_opType = OPERATION_TYPE::S;
			m_operation = Ship::Instance().Move;
			break;
		case 'E':
			m_opType = OPERATION_TYPE::E;
			m_operation = Ship::Instance().Move;
			break;
		case 'W':
			m_opType = OPERATION_TYPE::W;
			m_operation = Ship::Instance().Move;
			break;
		case 'L':
			m_opType = OPERATION_TYPE::L;
			m_operation = Ship::Instance().Turn;
			break;
		case 'R':
			m_opType = OPERATION_TYPE::R;
			m_operation = Ship::Instance().Turn;
			break;
		case 'F':
			m_opType = OPERATION_TYPE::F;
			m_operation = Ship::Instance().Move;
			break;
		default:
			break;
		}
}

//#define USE_TEST_CASE 1;
std::vector<Instruction> ReadInput()
{
#ifdef USE_TEST_CASE 
	std::ifstream input("input/d12test.txt");
#endif
#ifndef USE_TEST_CASE 
	std::ifstream input("input/d12.txt");
#endif

	std::vector<Instruction> data;
	std::string line;
	while (std::getline(input, line))
	{
		data.emplace_back(line);
	}
	
	return data;
}

int main()
{
	std::vector<Instruction> data{ std::move(ReadInput()) };
	for (const Instruction& instruction : data)
	{
		instruction.PerformOperation();
	}

	std::cout << Ship::Instance().GetManhattanDistance();
}