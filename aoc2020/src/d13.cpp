#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <tuple>

int ToNumber(std::string_view aData, int& aIter)
{
	int result{};
	for (const char c : aData)
	{
		if ( c == 'x')
		{
			aIter += 2;
			return -1;
		}
		else if (c == ',')
		{
			++aIter;
			break;
		}
		result *= 10;
		result += static_cast<int>(c & 0x0F);
		++aIter;
	}
	return result;
}

int64_t GreatestCommonDivisor_E(int64_t a, int64_t b, int64_t* x, int64_t* y)
{
	if (a == 0)
	{
		*x = 0;
		*y = 1;
		return b;
	}
	int64_t x1{};
	int64_t y1{};
	const int64_t gcd{ GreatestCommonDivisor_E(b % a, a, &x1, &y1) };

	*x = y1 - (b / a) * x1;
	*y = x1;
	return gcd;
}

int64_t ModInverse(int64_t a, int64_t m)
{
	int64_t x{};
	int64_t y{};
	int64_t g{ GreatestCommonDivisor_E(a,m, &x,&y) };
	return (x % m + m) % m;
}

//#define USE_TEST_CASE 1;
int main()
{
#ifdef USE_TEST_CASE 
	std::ifstream input("input/d13test.txt");
#else
	std::ifstream input("input/d13.txt");
#endif
	std::string line;
	std::getline(input, line);
	int iter{ 0 };
	int time{ ToNumber(line,iter) };
	std::getline(input, line);
	iter = 0;
#ifdef PART1
	std::unordered_map<int, int>buses;
	while (iter < line.size())
	{
		std::string busString{ line.substr(iter) };
		int busID{ ToNumber(busString,iter) };
		if (busID != -1)
		{
			int firstValid{ ((time / busID)+1) * busID};
			buses.insert_or_assign(busID, firstValid);
		}
	}

	std::pair<int,int> earliestBus;
	int earliestTime{ INT_MAX };
	for (std::pair<int,int> iter : buses)
	{
		if (iter.second < earliestTime)
		{
			earliestBus = iter;
			earliestTime = iter.second;
		}
	}
	std::cout << "RESULT : " << (earliestBus.second - time) * earliestBus.first;
#else
	// After failing a whole bunch I gave in and googled for clues. Got clued in on ChineseRemainderTheorem being super useful here.
	// https://brilliant.org/wiki/chinese-remainder-theorem/
	// Instead of storing 'x's we store indexes accounting for 'x's in pairs
	std::vector<std::pair<int, int>>buses;	// BusID, index
	int t{};
	while (iter < line.size())
	{
		std::string busString{ line.substr(iter) };
		int busID{ ToNumber(busString,iter) };
		if (busID != -1)
		{
			buses.emplace_back(busID,t);
		}
		++t;
	}

	unsigned long long product{1};
	for (int i = 0; i < buses.size(); i++)
	{
		product *= buses[i].first;
	}

	unsigned long long result{};
	for (int i = 0; i < buses.size(); i++)
	{
		// product[i] = product / busID[i];
		unsigned long long partialProduct = product / buses[i].first;
		result += (buses[i].first - buses[i].second) * ModInverse(partialProduct, buses[i].first) * partialProduct;
	}
	result %= product;
	std::cout << "RESULT : " << result;

#endif
	return 0;
}
