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

void pay_filter_cost();

class filter
{
public:
    filter(const std::uintmax_t & timestamp, std::uintmax_t & dirt, std::priority_queue<event, std::vector<event>, std::greater<event>> & events,
        std::uintmax_t & repairman) : _timestamp{ timestamp }, _dirt{ dirt }, _events(events), _repairman{ repairman }
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
            auto repair_time = dist(engine);
            if (_repairman > _timestamp)
            {
                repair_time += _repairman;
            }
            
            else
            {
                repair_time += _timestamp;
            }
            
            if (_timestamp >= _fault) 
            { 
                pay_filter_cost(); 
            } 
            
            _events.emplace(repair_time, [this](std::uintmax_t){ repair(); });
            _repairman = repair_time;
            
            return;
        }
        
        static std::normal_distribution<> dist(50, 25);
        
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
    std::uintmax_t & _repairman;
};
