#include "IncorrectArgumentValue.h"

namespace Exceptions {
	IncorrectArgumentValue::IncorrectArgumentValue() {
		std::cout << "Incorrect value of argument(-s) passed!" << std::endl;
	}

	IncorrectArgumentValue::IncorrectArgumentValue(const std::string& message) {
		std::cout << message << std::endl;
	}

	IncorrectArgumentValue::~IncorrectArgumentValue() {}
}