#include <queue>
#include "event.h"

constexpr std::uintmax_t timestep = 2;
constexpr std::uintmax_t max_timestamp = 60;

int main()
{
    std::priority_queue<event> events;
    events.push(event{ 31 });
    
    std::uintmax_t timestamp = 0;
    
    while (timestamp < max_timestamp)
    {
        while (events.size() && events.top().timestamp() <= timestamp)
        {
            const event & e = events.top();
            e.handle(timestamp);
            
            events.pop();
        }
        
        timestamp += timestep;
    }
}
