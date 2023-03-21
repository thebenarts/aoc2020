#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <span>
#include <vector>

struct Group
{
	Group() = default;
	Group(std::span<std::string> aData)
	{
		ProcessData(aData);
	}

	inline unsigned int GetYes() const {
		return static_cast<unsigned int> (mYes);
	};
	
private:
	void ProcessData(std::span<std::string> aData);
	int mYes{};
};

void Group::ProcessData(std::span<std::string> aData)
{
#ifndef PART1
	std::vector<unsigned int> bits;
	bits.reserve(aData.size());
	bool yesAll{ true };
#endif
	for (const std::string& person : aData)
	{
		unsigned int bitField{};
		for (const char c : person)
		{
			int bitShift = static_cast<int>(c - 'a');
			bitField |= 1 << bitShift;
		}
#ifndef PART1
		bits.emplace_back(bitField);
#endif
	}

	for (int i = 0; i != 26; i++)
	{
#ifdef PART1
		mYes += (bitField & (1 << i)) ? 1 : 0;
#endif
#ifndef PART1
		for (unsigned int bit : bits)
		{
			yesAll &= (bit & (1 << i)) ? true : false;
		}
		mYes += yesAll ? 1 : 0;
		yesAll = true;
#endif
	}
}

//#define TEST_CASE 1;
std::vector<Group> ReadInput()
{
#ifdef TEST_CASE
	std::ifstream input("input/d6test.txt");
#endif
#ifndef TEST_CASE
	std::ifstream input("input/d6.txt");
#endif
	
	std::vector<Group> groups;
	std::string line;
	std::vector<std::string> data;
	while (std::getline(input, line))
	{
		// Add new group data
		if (line.empty())
		{
			groups.emplace_back(data);
			data.clear();
			continue;
		}
		data.push_back(line);
	}
	// Add last group
	groups.emplace_back(data);

	return groups;
}

int main()
{
	std::vector<Group> groups{ std::move(ReadInput()) };
	unsigned int yesCount{};
	for (const Group group : groups)
	{
		yesCount += group.GetYes();
	}

	std::cout << yesCount;
}
