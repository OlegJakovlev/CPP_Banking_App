#include "InputParser.h"

template <>
unsigned InputParser::parse<unsigned>(const std::string toParse) {
	try {
		// Convert and record number of converted symbols
		std::size_t amountOfConvertedSymbols;
		const auto result = std::stoul(toParse, &amountOfConvertedSymbols);

		// Check if input is corrupted
		if (amountOfConvertedSymbols != toParse.size()) throw Exceptions::IncorrectArgumentValue();

		return result;
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
int InputParser::parse<int>(const std::string toParse) {
	try {
		// Convert and record number of converted symbols
		std::size_t amountOfConvertedSymbols;
		const auto result = std::stoi(toParse, &amountOfConvertedSymbols);

		// Check if input is corrupted
		if (amountOfConvertedSymbols != toParse.size()) throw Exceptions::IncorrectArgumentValue();

		return result;
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
float InputParser::parse<float>(const std::string toParse) {
	try {
		// Convert and record number of converted symbols
		std::size_t amountOfConvertedSymbols;
		const auto result = std::stof(toParse, &amountOfConvertedSymbols);

		// Check if input is corrupted
		if (amountOfConvertedSymbols != toParse.size()) throw Exceptions::IncorrectArgumentValue();

		return result;
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
double InputParser::parse<double>(const std::string toParse) {
	try {
		// Convert and record number of converted symbols
		std::size_t amountOfConvertedSymbols;
		const auto result = std::stod(toParse, &amountOfConvertedSymbols);
		
		// Check if input is corrupted
		if (amountOfConvertedSymbols != toParse.size()) throw Exceptions::IncorrectArgumentValue();

		return result;
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
long InputParser::parse<long>(const std::string toParse) {
	try {
		// Convert and record number of converted symbols
		std::size_t amountOfConvertedSymbols;
		const auto result = std::stol(toParse, &amountOfConvertedSymbols);

		// Check if input is corrupted
		if (amountOfConvertedSymbols != toParse.size()) throw Exceptions::IncorrectArgumentValue();
		
		return result;
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
long long InputParser::parse<std::time_t>(const std::string toParse) {
	// Initialize tmp variables
	std::tm time = { 0 };
	std::istringstream date(toParse);

	// Convert string to UNIX timestamp
	date >> std::get_time(&time, "%H:%M:%S %d/%m/%Y");
	std::time_t time_stamp = mktime(&time);

	// Check if convertation failed
	if (time_stamp == -1) throw Exceptions::IncorrectArgumentValue("Please check date format! It should be H:M:S day/month/year. Example: 15:12:01 08/12/2021");

	return time_stamp;
}