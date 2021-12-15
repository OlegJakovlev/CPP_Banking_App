#include "IncorrectArgumentTypes.h"

namespace Exceptions {
	IncorrectArgumentTypes::IncorrectArgumentTypes() {
		std::cout << "Incorrect type of argument(-s) passed!" << std::endl;
	}

	IncorrectArgumentTypes::IncorrectArgumentTypes(std::string message) {
		std::cout << message << std::endl;
	}

	IncorrectArgumentTypes::~IncorrectArgumentTypes() {}
}

