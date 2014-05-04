#pragma once

#include <queue>
#include <random>

#include "event.h"

extern std::mt19937 engine;

namespace
{
    std::uintmax_t generate_fault_time_point()
    {
        static std::exponential_distribution<> d(0.001);
        
        return std::round(d(engine));
    }
}

class filter
{
public:
    filter(const std::uintmax_t & timestamp, std::uintmax_t & dirt, std::priority_queue<event> & events) : _timestamp{ timestamp }, 
        _dirt{ dirt }, _events(events)
    {
    }
    
    void do_filter()
    {
        if (_faulty)
        {
            return;
        }
        
        if (!_faulty && !_fault)
        {
            _fault = _timestamp + generate_fault_time_point();
        }
        
        if (!_faulty && _timestamp >= _fault)
        {
            _faulty = true;
            std::cout << "A filter has failed at timestamp " << _timestamp << '\n';
            // TODO: add inserting a repair event into the events queue
            return;
        }
        
        static std::normal_distribution<> dist(100, 50);
        
        _dirt -= std::min<std::intmax_t>(dist(engine), _dirt);
    }
    
private:
    bool _faulty = false;
    const std::uintmax_t & _timestamp;
    std::uintmax_t & _dirt;
    std::priority_queue<event> & _events;
    std::uintmax_t _fault = 0;
};
