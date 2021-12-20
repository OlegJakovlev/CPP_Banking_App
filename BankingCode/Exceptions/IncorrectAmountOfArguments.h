#pragma once

#ifndef IncorrectAmountOfArguments_H
#define IncorrectAmountOfArguments_H

#include <iostream>
#include <exception>

namespace Exceptions {
    class IncorrectAmountOfArguments : public std::exception
    {
    public:
        IncorrectAmountOfArguments();
        ~IncorrectAmountOfArguments() override;
    };
}


#endif
