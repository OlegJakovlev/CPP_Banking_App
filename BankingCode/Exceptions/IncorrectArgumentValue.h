#pragma once

#ifndef IncorrectArgumentValue_H
#define IncorrectArgumentValue_H

#include <iostream>
#include <exception>

namespace Exceptions {
	class IncorrectArgumentValue : public std::exception
	{
	public:
		IncorrectArgumentValue();
		IncorrectArgumentValue(std::string message);
		~IncorrectArgumentValue() override;
	};
}

#endif

