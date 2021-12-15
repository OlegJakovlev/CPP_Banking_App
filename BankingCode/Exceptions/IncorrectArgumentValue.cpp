#include "IncorrectArgumentValue.h"

namespace Exceptions {
	IncorrectArgumentValue::IncorrectArgumentValue() {
		std::cout << "Incorrect type of argument(-s) passed!" << std::endl;
	}

	IncorrectArgumentValue::IncorrectArgumentValue(std::string message) {
		std::cout << message << std::endl;
	}

	IncorrectArgumentValue::~IncorrectArgumentValue() {}
}