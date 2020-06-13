#pragma once

#ifndef GENAYTYK_EXCEPTION_H
#define GENAYTYK_EXCEPTION_H

#include <iostream>

class Genaytyk_Exception : public std::exception
{
public:
    Genaytyk_Exception(const std::string &msg) : _msg(msg) {}

    virtual const char *what() const noexcept override
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

#endif // !GENAYTYK_EXCEPTION_H