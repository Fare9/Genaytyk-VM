#pragma once

#ifndef DISASSEMBLER_EXCEPTION_H
#define DISASSEMBLER_EXCEPTION_H

#include <iostream>

class Disassembler_Exception : public std::exception
{
public:
    Disassembler_Exception(const std::string &msg) : _msg(msg) {}

    virtual const char *what() const noexcept override
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

#endif