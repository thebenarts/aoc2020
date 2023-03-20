#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <functional>

const std::pair<int, int> GetRange(const std::string& aString)
{
	int i = 0;
	int num1{};
	while (i < aString.size() && aString[i] != '-')
	{
		num1 *= 10;
		num1 += static_cast<int>(aString[i] & 0x0F);
		++i;
	}
	i++;

	int num2{};
	while (i < aString.size())
	{
		num2 *= 10;
		num2 += static_cast<int>(aString[i] & 0x0F);
		++i;
	}

	return { num1,num2 };
}

int CountOccurences(const std::string& aString, const char delimeter)
{
	int occurence{};
	for (int i = 0; i < aString.size(); ++i)
	{
		if (aString[i] == delimeter)
		{
			++occurence;
		}
	}

	return occurence;
}

bool CheckIndexes(const std::string& aString, const std::pair<int, int>aIndexes, const char delimeter)
{
	bool correctPassword{ false };
	// indexing starts from 1
	if (aIndexes.first - 1 < aString.size())
	{
		if (aString[aIndexes.first - 1] == delimeter)
		{
			correctPassword = true;
		}
	}

	if (aIndexes.second - 1 < aString.size())
	{
		if (aString[aIndexes.second - 1] == delimeter)
		{
			correctPassword = !correctPassword;
		}
	}

	return correctPassword;
}

int main()
{
	std::ifstream input("input/d2.txt");
	std::string line;
	std::stringstream wordStream;
	std::string word;
	int validPasswords{};
	while (std::getline(input, line))
	{
		wordStream.str(line);

		// get the range
		wordStream >> word;
		const std::pair<int, int> range{ GetRange(word) };

		//get the character
		char requiredCharacter{};
		wordStream >> requiredCharacter;

		// get the password
		wordStream >> word;	// skip ':'
		wordStream >> word;

		std::function<int()> CountOccurence = [&word, &requiredCharacter, &range]() -> int{
			int occurence{};
			for (int i = 0; i < word.size(); ++i)
			{
				if (word[i] == requiredCharacter)
				{
					++occurence;
				}
			}

			return occurence;
		};

		//int occurence{ CountOccurence() };
		//if (occurence >= range.first && occurence <= range.second)
		//{
		//	++validPasswords;
		//}
		if (CheckIndexes(word, range, requiredCharacter))
		{
			++validPasswords;
		}

		wordStream.clear();
	}

	std::cout << validPasswords;
}