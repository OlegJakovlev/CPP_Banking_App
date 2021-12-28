#include "IncorrectArgumentTypes.h"

namespace Exceptions {
	IncorrectArgumentTypes::IncorrectArgumentTypes() {
		std::cout << "Incorrect type of argument(-s) passed!" << "\n";
	}

	IncorrectArgumentTypes::IncorrectArgumentTypes(const std::string message) {
		std::cout << message << "\n";
	}

	IncorrectArgumentTypes::~IncorrectArgumentTypes() {}
}

