#pragma once

#include <cstdint>
#include <iostream>

class event
{
public:
    template<typename F>
    event(std::uintmax_t timestamp, F && f) : _timestamp{ timestamp }, _callback{ std::forward<F>(f) }
    {
    }
    
    void handle(std::uintmax_t current) const
    {
        _callback(current);
    }
    
    std::uintmax_t timestamp() const
    {
        return _timestamp;
    }
    
    bool operator>(const event & other) const
    {
        return _timestamp > other._timestamp;
    }
    
private:
    std::uintmax_t _timestamp;
    std::function<void (std::uintmax_t)> _callback;
};
