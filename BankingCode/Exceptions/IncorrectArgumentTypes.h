#pragma once

#ifndef IncorrectArgumentTypes_H
#define IncorrectArgumentTypes_H

#include <iostream>
#include <exception>

namespace Exceptions {
    class IncorrectArgumentTypes : public std::exception
    {
    public:
        IncorrectArgumentTypes();
        IncorrectArgumentTypes(const std::string message);
        ~IncorrectArgumentTypes() override;
    };
}
#endif
