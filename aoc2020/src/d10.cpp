#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <algorithm>

int ToNumber(std::string_view aString)
{
	int number{};
	for (const char& c : aString)
	{
		number *= 10;
		number += static_cast<int>(c & 0x0F);
	}

	return number;
}

std::vector<int> ReadInput()
{
#ifdef USE_TEST_CASE
	std::ifstream input("input/d10test.txt");
#endif
#ifndef USE_TEST_CASE
	std::ifstream input("input/d10.txt");
#endif

	std::vector<int>numbers;
	numbers.reserve(200);
	numbers.emplace_back(0);

	std::string line;
	while (std::getline(input, line))
	{
		numbers.emplace_back(ToNumber(line));
	}

	std::sort(numbers.begin(), numbers.end(), [](const int& a, const int& b) {return a < b; });
	numbers.emplace_back(numbers.back() + 3);

	return numbers;
}

long long RecursiveGetAll(const std::vector<int>& aNumbers, std::vector<long long>& dp, int aIterator)
{
	if (aIterator >= aNumbers.size() - 1)
	{
		return 1;
	}

	if (dp[aIterator] != LONG_MIN)
	{
		return dp[aIterator];
	}

	long long result{};
	for (int j = aIterator + 1; j < aNumbers.size(); j++)
	{
		if (aNumbers[j] - aNumbers[aIterator] > 3)
		{
			break;
		}

		result += RecursiveGetAll(aNumbers,dp, j);
	}
	
	dp[aIterator] = result;
	return result;
}

int main()
{
	std::vector<int>numbers{ std::move(ReadInput()) };
	int freq[3]{0,0,0};

	// -------------------------------------------------------------- PART 1
	for (int i = 1; i < numbers.size(); i++)
	{
		freq[(numbers[i] - numbers[i - 1]) - 1]++;
	}

	std::cout << freq[0] * (freq[2]) << '\n';	
	// ------------------------------------------------------------------

	// ---------------------------------------------------------------- PART 2
	std::vector<long long> dp(numbers.size(), LONG_MIN);
	std::cout << RecursiveGetAll(numbers, dp, 0);
}
