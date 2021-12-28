#pragma once

#ifndef InputParser_H
#define InputParser_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "./Exceptions/IncorrectArgumentTypes.h"
#include "./Exceptions/IncorrectArgumentValue.h"

static class InputParser
{
public:
	template <typename T>
	static T parse(const std::string& toParse);
};

#endif

