#include "InputParser.h"

/* Any workaround for non-implemented types?
template <typename T>
T InputParser::parse<T>(std::string toParse) {
	std::cout << "Error" << std::endl;
}
*/

template <>
unsigned InputParser::parse<unsigned>(std::string toParse) {
	try {
		return stoul(toParse);
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
int InputParser::parse<int>(std::string toParse) {
	try {
		return stoi(toParse);
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
float InputParser::parse<float>(std::string toParse) {
	try {
		return stof(toParse);
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
double InputParser::parse<double>(std::string toParse) {
	try {
		return stod(toParse);
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
long InputParser::parse<long>(std::string toParse) {
	try {
		return stol(toParse);
	}
	catch (const std::invalid_argument) {
		throw Exceptions::IncorrectArgumentTypes();
	}
	catch (const std::out_of_range) {
		throw Exceptions::IncorrectArgumentValue();
	}
}

template <>
long long InputParser::parse<std::time_t>(std::string toParse) {
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