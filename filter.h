#pragma once

#include <queue>
#include <random>
#include <functional>

#include "event.h"

extern std::mt19937 engine;

namespace
{
    std::uintmax_t generate_fault_time_point()
    {
        static std::exponential_distribution<> d(0.0001);
        
        return std::round(d(engine));
    }
}

class filter
{
public:
    filter(const std::uintmax_t & timestamp, std::uintmax_t & dirt, std::priority_queue<event, std::vector<event>, std::greater<event>> & events) : _timestamp{ timestamp }, 
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
        
        // either it broke or got too dirty
        if ((!_faulty && _timestamp >= _fault) || _filter_dirt >= _filtering_limit)
        {
            _faulty = true;
            std::cout << "At timestamp " << _timestamp << " a filter has failed\n";
            
            static std::normal_distribution<> dist(120, 60);
            
            _events.emplace(_timestamp + dist(engine), [this](std::uintmax_t){ repair(); });
            
            return;
        }
        
        static std::normal_distribution<> dist(100, 25);
        
        std::uintmax_t dirt_filtered = std::min<std::intmax_t>(dist(engine), _dirt);
        _dirt -= dirt_filtered;
        _filter_dirt += dirt_filtered;
    }
    
    void repair()
    {
        _faulty = false;
        _fault = 0;
        _filter_dirt = 0;
        std::cout << "At timestamp " << _timestamp << " a filter has been repaired\n";
    }
    
private:
    static constexpr std::uintmax_t _filtering_limit = 150000;
    
    bool _faulty = false;
    const std::uintmax_t & _timestamp;
    std::uintmax_t & _dirt;
    std::uintmax_t _filter_dirt = 0;
    std::priority_queue<event, std::vector<event>, std::greater<event>> & _events;
    std::uintmax_t _fault = 0;
};
