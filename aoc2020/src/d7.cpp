// I am sorry in advance. I should have used regex... and the code could be optimized much better.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <span>
#include <functional>

struct Bag
{
	Bag() = default;
	Bag(std::string_view aString, int num) : mId(aString), mNumber(num){}

	bool operator==(const Bag& other)const
	{
		return mId == other.mId;
	}

	std::string mId;
	int mNumber;
};

namespace std {
	template<>
	struct hash<Bag> {
		auto operator()(const Bag& bag) const -> size_t {
			return hash<string>{}(bag.mId);
		}
	};
}

int ToNumber(std::string_view aString)
{
	int number{};
	for (const char c : aString)
	{
		if (c < '0' || c > '9')
		{
			continue;
		}

		number *= 10;
		number += static_cast<int>(c & 0x0F);
	}

	return number;
}

std::string_view ProcessString(std::string_view aData, size_t& end, int& number)
{
	static const char numbersAndWhiteSpace[]{ '0','1','2','3','4','5','6','7','8','9',' '};
	size_t startPos{ aData.find_first_not_of(numbersAndWhiteSpace) };

	std::string_view numberString{ aData.substr(0,startPos) };
	number = ToNumber(numberString);
	
	size_t endPos{ aData.find_first_of(',')};
	if (endPos == std::string::npos) // If this is the last bag we won't find ',' instead we have to look for '.'
	{
		endPos = aData.find_first_of('.');
		end = std::string::npos;
	}
	else
	{
		end += endPos + 1;
	}

	std::string_view newView{ aData.data() + startPos,aData.data() + endPos };
	endPos = newView.find_last_of(' ');
	return std::string_view{ aData.data() + startPos, aData.data() + startPos + endPos };
}

#define TEST_CASE 1;
void ReadInput(std::unordered_map<std::string, std::unordered_set<Bag>>& aMap)
{
#ifdef TEST_CASE
	std::ifstream input("input/d7test.txt");
#endif
#ifndef TEST_CASE
	std::ifstream input("input/d7.txt");
#endif

	std::string line;
	while (std::getline(input, line))
	{
		size_t lastChar = line.find("bags");
		std::string_view bag{ line.data(),line.data() + lastChar - 1 };
		std::string_view rest{ line.data() + lastChar + 13 };
	
		std::unordered_set<Bag>set;
		size_t pos{};
		int number{};
		while (pos != std::string::npos)
		{
			std::string_view otherBag = rest.substr(pos);
			
			std::string otherBagString{ ProcessString(otherBag,pos,number) };
			// Handle case where the bag doesn't contain other bags.
			if (otherBagString == "no other")
			{
				continue;
			}
			set.insert(Bag{ otherBagString,number });
		}

		aMap.insert_or_assign(std::string{ bag }, set);
	}
}

bool RecursiveSearch(std::unordered_map<std::string, std::unordered_set<Bag>>& aMap, const std::string& aString, const Bag& aTarget)
{
	auto currentSet{ aMap[aString] };
	if (currentSet.empty())	// Exit statemenet
	{
		return false;
	}

	// If it doesn't directly contain target we will recurse
	if (currentSet.find(aTarget) == currentSet.end())
	{
		bool truth{ false };
		for (const Bag& bag : currentSet)
		{
			truth |= RecursiveSearch(aMap, bag.mId,aTarget);
		}

		return truth;
	}

	return true;
}

int Part1(std::unordered_map<std::string, std::unordered_set<Bag>>& aData)
{
	const Bag target{ "shiny gold",0 };
	int result{ 0 };
	for (auto iter : aData)
	{
		result += RecursiveSearch(aData, iter.first,target);
	}
	return result;
}

int RecursiveSearch(std::unordered_map<std::string, std::unordered_set<Bag>>& aData, const Bag& aBag)
{
	std::unordered_map < const std::string,std::unordered_set<Bag>>::iterator iter = aData.find(aBag.mId);
	if (iter->second.empty())
	{
		return 1;
	}

	int count{0};
	for (const Bag& bag : iter->second)
	{
		int num{ RecursiveSearch(aData, bag) };
		if (num == 1)	// If this is the last bag we don't need to add the current bag number to it.
		{
			count += bag.mNumber * num;
		}
		else
		{
			count += bag.mNumber + bag.mNumber * num;
		}
	}

	return count;
}

int Part2(std::unordered_map<std::string, std::unordered_set<Bag>>& aData)
{
	Bag targetBag{ "shiny gold",0 };
	return RecursiveSearch(aData, targetBag);
}

int main()
{
	std::unordered_map<std::string, std::unordered_set<Bag>> dataMap;
	ReadInput(dataMap);
	std::cout << Part1(dataMap) << '\n';
	std::cout << Part2(dataMap);
}