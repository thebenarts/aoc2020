#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <assert.h>

//#define USE_TESTDATA 1;
//#define PART1 1;

int FindNextWhiteSpace(std::string_view aView, int aStart)
{
	while (aStart < aView.size() && aView[aStart] != ' ')
	{
		aStart++;
	}

	return aStart;
}

int ProcessNumber(std::string_view aData)
{
	int number{0};
	for (const char character : aData)
	{
		// Check if it's a number
		if (character < 0x30 || character > 0x3A)
		{
			// Perform conversion right here right now
			if (character == 'i')
			{
				number = static_cast<int>(static_cast<float>(number) * 2.54f);
			}
			break;
		}

		number *= 10;
		number += static_cast<int>(character & 0x0F);
	}

	return number;
}

class Passport
{
public:
	Passport(std::string_view aData)
	{
		ProcessData(aData);
	}

	inline bool IsValid() const {
		bool hasData{ HasData() };
#ifdef PART1
		return hasData;
#endif

		if (hasData)
		{
			return IsValidData();
		}

		return false;
	};
private:

	enum DATA_TYPE : uint8_t
	{
		byr,
		iyr,
		eyr,
		hgt,
		cid,
		hcl,
		ecl,
		pid,
		error,
	};
	DATA_TYPE DeduceType(std::string_view aType);

	void ProcessData(std::string_view aData);
	void ProcessChunk(std::string_view aDataChunk);

	bool HasData() const {
		if (mNumberOfInfo == 8) { return true; }
		else if (mNumberOfInfo == 7 && mCid == INT_MIN) { return true; }
		else return false;
	};

	bool IsValidData() const;

	int mByr{ INT_MIN };		// Birth of year
	int mIyr{ INT_MIN };		// Issue year
	int mEyr{ INT_MIN };		// Expiration year
	int mHgt{ INT_MIN };		// Height
	char mHcl[10]{'\0'};	// Hair color
	char mEcl[5] {'\0'};	// Eye color
	char mPid[12]{ '\0' };	// Passport ID
	int mCid{ INT_MIN };		// Country ID

	int mNumberOfInfo{0};
};

bool Passport::IsValidData() const
{
	if (mByr < 1920 || mByr > 2002)
	{
		return false;
	}

	if (mIyr < 2010 || mIyr > 2020)
	{
		return false;
	}

	if (mEyr < 2020 || mEyr > 2030)
	{
		return false;
	}

	if (mHgt < 150 || mHgt > 193)
	{
		return false;
	}

	auto PerformHairCheck = [this]() {

		if (mHcl[0] != '#')
		{
			return false;
		}

		int i = 1;
		while (i < sizeof(mHcl) && mHcl[i] != '\0')
		{
			if ((mHcl[i] < '0' || mHcl[i] > '9') && (mHcl[i] < 'a' || mHcl[i] > 'f'))
			{
				return false;
			}
			++i;
		}

		if (i != 7)
		{
			return false;
		}

		return true;
	};

	if (!PerformHairCheck())
	{
		return false;
	}

	auto PerformEyeCheck = [this]() {

		auto compareString = [this](std::string_view aString) {

			for (int i = 0; i < sizeof(mEcl); i++)
			{
				if (mEcl[i] == '\0')
				{
					if (i < aString.size())
					{
						return false;
					}
					break;
				}

				if (mEcl[i] != aString[i])
				{
					return false;
				}
			}

			return true;
		};

		if (!compareString("amb") && !compareString("blu") && !compareString("brn") && !compareString("gry") && !compareString("grn") && !compareString("hzl") && !compareString("oth"))
		{
			return false;
		}

		return true;
	};

	if (!PerformEyeCheck())
	{
		return false;
	}

	auto PerformPidCheck = [this]() {
		int i = 0;
		while (i < sizeof(mPid) && mPid[i] != '\0')
		{
			if (mPid[0] < '0' || mPid[i] > '9')
			{
				return false;
			}
			++i;
		}

		return i == 9;
	};

	if (!PerformPidCheck())
	{
		return false;
	}

	return true;
}

Passport::DATA_TYPE Passport::DeduceType(std::string_view aType)
{
	if (aType == "byr")
	{
		return DATA_TYPE::byr;
	}
	else if (aType == "iyr")
	{
		return DATA_TYPE::iyr;
	}
	else if (aType == "eyr")
	{
		return DATA_TYPE::eyr;
	}
	else if (aType == "hgt")
	{
		return DATA_TYPE::hgt;
	}
	else if (aType == "hcl")
	{
		return DATA_TYPE::hcl;
	}
	else if (aType == "ecl")
	{
		return DATA_TYPE::ecl;
	}
	else if (aType == "pid")
	{
		return DATA_TYPE::pid;
	}
	else if (aType == "cid")
	{
		return DATA_TYPE::cid;
	}

	// We should never hit this. If we do we have either messed up the string we passed in or the dataset is wrong.
	assert(false);
	return DATA_TYPE::error;
}

void Passport::ProcessData(std::string_view aData)
{
	// Find chunks to process
	int iter{ 0 };
	while (iter < aData.size())
	{
		int whiteSpace{ FindNextWhiteSpace(aData, iter) };
		ProcessChunk(std::string_view(aData.data() + iter, aData.data() + whiteSpace));
		
		iter = whiteSpace + 1;
	}
}

void Passport::ProcessChunk(std::string_view aDataChunk)
{
	// Due to formatting we have to perform a range check here
	if (aDataChunk.size() < 5)
	{
		return;
	}

	std::string_view typeString{ aDataChunk.substr(0,3) };
	std::string_view data{ aDataChunk.substr(4) };
	DATA_TYPE type{ DeduceType(typeString) };

	auto HandleNullTermination = [this, data](char* member, const size_t memberSize) {
		if (data.size() < memberSize)
		{
			member[data.size()] = '\0';
		}
	};

	switch (type)
	{
	case DATA_TYPE::byr:
		mByr = ProcessNumber(data);
		break;
	case DATA_TYPE::iyr:
		mIyr = ProcessNumber(data);
		break;
	case DATA_TYPE::eyr:
		mEyr = ProcessNumber(data);
		break;
	case DATA_TYPE::hgt:
		mHgt = ProcessNumber(data);
		break;
	case DATA_TYPE::cid:
		mCid = ProcessNumber(data);
		break;
	case DATA_TYPE::hcl:
		memcpy(mHcl, data.data(), data.size());
		HandleNullTermination(mHcl, sizeof(mHcl));
		break;
	case DATA_TYPE::ecl:
		memcpy(mEcl, data.data(), data.size());
		HandleNullTermination(mEcl, sizeof(mEcl));
		break;
	case DATA_TYPE::pid:
		memcpy(mPid, data.data(), data.size());
		HandleNullTermination(mPid, sizeof(mPid));
		break;
	case DATA_TYPE::error:
		assert(false);
		return;
		break;
	}

	++mNumberOfInfo;
}

int main()
{

#ifdef USE_TESTDATA
	std::ifstream input("input/d4test.txt");
#endif
#ifndef USE_TESTDATA
	std::ifstream input("input/d4.txt");
#endif

	std::vector<Passport> passports;
	std::stringstream wordStream;
	std::string line;
	std::string data;
	while (std::getline(input, line))
	{
		// End of current passport data... process now
		if (line.empty())
		{
			// process data
			passports.emplace_back(data);
			//clear data...
			data.clear();
			continue;
		}
		data.append(' ' + line);
	}
	passports.emplace_back(data);	// Read last passport

	int result{0};
	std::vector<Passport>invalidPassports;
	for (const Passport& passport : passports)
	{
		if (passport.IsValid())
		{
			++result;
		}
		else
		{
			invalidPassports.push_back(passport);
		}
	}

	std::cout << result;

}