/****************************************************************************
    Copyright (C) 2012 Adrian Blumer (blumer.adrian@gmail.com)
    Copyright (C) 2012 Pascal Spörri (pascal.spoerri@gmail.com)
    Copyright (C) 2012 Sabina Schellenberg (sabina.schellenberg@gmail.com)

    All Rights Reserved.

    You may use, distribute and modify this code under the terms of the
    MIT license (http://opensource.org/licenses/MIT).
*****************************************************************************/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <exception>

class ExceptionBase : public std::exception
{
protected:
    std::string _message;

public:
    ExceptionBase(const std::string& message)
        : _message(message)
    {}

    ~ExceptionBase() throw(){}

    virtual const char* what() const throw()
    {
        return _message.c_str();
    }
};


class Exception : public ExceptionBase
{
public:
    Exception(const std::string& message) : ExceptionBase(message) {}
};

class TodoException : public ExceptionBase
{
public:
    TodoException(const std::string& message) : ExceptionBase(message) {}
};

#endif // EXCEPTION_H
