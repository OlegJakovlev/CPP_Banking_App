#include "IncorrectArgumentValue.h"

namespace Exceptions {
	IncorrectArgumentValue::IncorrectArgumentValue() {
		std::cout << "Incorrect value of argument(-s) passed!" << "\n";
	}

	IncorrectArgumentValue::IncorrectArgumentValue(const std::string& message) {
		std::cout << message << "\n";
	}

	IncorrectArgumentValue::~IncorrectArgumentValue() {}
}