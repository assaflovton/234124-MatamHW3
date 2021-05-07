#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <cstring>

namespace mtm
{

    class Exception : public std::exception
    {
    protected:
        std::string error;

    public:
        Exception(std::string error) : error(error) {}
        const char *what() const noexcept override
        {
            return error.c_str();
        }
    };
    
    class GameException : public Exception
    {
    protected:
        std::string name;

    public:
        GameException(std::string name) : Exception("A game related error has occurred: " + name) {}
    };
    class IllegalArgument : public GameException
    {
    public:
        IllegalArgument() : GameException("IllegalArgument") {}
    };
    class IllegalCell : public GameException
    {
    public:
        IllegalCell() : GameException("IllegalCell") {}
    };
    class CellEmpty : public GameException
    {
    public:
        CellEmpty() : GameException("CellEmpty") {}
    };
    class MoveTooFar : public GameException
    {
    public:
        MoveTooFar() : GameException("MoveTooFar") {}
    };
    class CellOccupied : public GameException
    {
    public:
        CellOccupied() : GameException("CellOccupied") {}
    };
    class OutOfRange : public GameException
    {
    public:
        OutOfRange() : GameException("OutOfRange") {}
    };
    class OutOfAmmo : public GameException
    {
    public:
        OutOfAmmo() : GameException("OutOfAmmo") {}
    };
    class IllegalTarget : public GameException
    {
    public:
        IllegalTarget() : GameException("IllegalTarget") {}
    };

        
      
} // namespace mtm

#endif //EXCEPTIONS_H