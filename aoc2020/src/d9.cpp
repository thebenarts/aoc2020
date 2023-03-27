#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

long long ToNumber(std::string_view aString)
{
	long long num{};
	for (const char c : aString)
	{
		num *= 10;
		num += static_cast<long long>(c & 0x0F);
	}

	return num;
}

std::vector<long long> ReadInput()
{
#ifdef TEST_CASE
	std::ifstream input("input/d9test.txt");
#endif
#ifndef TEST_CASE
	std::ifstream input("input/d9.txt");
#endif

	std::vector<long long> numbers;
	std::string line;
	while (std::getline(input, line))
	{
		numbers.emplace_back(ToNumber(line));
	}

	return numbers;
}

bool Validate(std::unordered_set<long long>& aPreamble, const std::vector<long long>& aNumbers, int aIterator)
{
	bool valid{ false };
	for (long long number : aPreamble)
	{
		long long target{ aNumbers[aIterator] - number };
		if (aPreamble.find(target) != aPreamble.end())
		{
			valid = true;
			break;
		}
	}

	return valid;
}

bool FindWeakness(int aStart, int& aEnd, const std::vector<long long>& aNumbers, long long aTarget)
{
	long long accumulator{};
	while (accumulator < aTarget)
	{
		accumulator += aNumbers[aEnd++];
	}
	
	return accumulator == aTarget;
}

int main()
{
	std::vector<long long>numbers{ std::move(ReadInput()) };

	std::unordered_set<long long> preamble;
	preamble.reserve(25);
	for (int i = 0; i != 25; i++)
	{
		preamble.insert(numbers[i]);
	}

	// PART 1
	long long part1{};
	for (int i = 25; i < numbers.size(); i++)
	{
		if (!Validate(preamble, numbers, i))
		{
			part1 = numbers[i];
			std::cout << "PART 1: " << numbers[i] << std::endl;
			break;
		}
		preamble.erase(numbers[i - 25]);
		preamble.insert(numbers[i]);
	}

	// PART 2
	int start{ 0 };
	int end{ 0 };
	for (int i = 0; i < numbers.size(); i++)
	{
		start = i;
		end = i;

		if (FindWeakness(start, end, numbers, part1))
		{
			long long max{ LONG_MIN };
			long long min{ LONG_MAX };
			for (int k = start; k < end; k++)
			{
				max = std::max(max, numbers[k]);
				min = std::min(min, numbers[k]);
			}

			std::cout << "PART 2: " <<  min + max << std::endl;
			break;
		}
	}
}
