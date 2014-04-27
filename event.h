#pragma once

#include <cstdint>
#include <iostream>

class event
{
public:
    event(std::uintmax_t timestamp) : _timestamp{ timestamp }
    {
    }
    
    void handle(std::uintmax_t current) const
    {
        std::cout << "event to be handled at " << _timestamp << " handled at " << current << '\n';
    }
    
    std::uintmax_t timestamp() const
    {
        return _timestamp;
    }
    
    bool operator<(const event & other) const
    {
        return _timestamp < other._timestamp;
    }
    
private:
    std::uintmax_t _timestamp;
};
