#include <queue>
#include <functional>
#include <random>

#include "event.h"
#include "filter.h"

constexpr std::uintmax_t timestep = 1;
constexpr std::uintmax_t max_timestamp = 3600;
constexpr std::uintmax_t num_filters = 8;

std::random_device rd;
std::mt19937 engine{ rd() };

std::uintmax_t linear_of_people(std::uintmax_t num_people)
{
    return num_people * 5;
}

std::intmax_t normal_people_distribution(std::uintmax_t, std::uintmax_t amount_of_people)
{
    static std::normal_distribution<> dist(0, 5);
    return std::max<std::intmax_t>(std::round(dist(engine)), -amount_of_people);
}

int main()
{
    std::priority_queue<event> events;
    std::uintmax_t timestamp = 0;
        
    std::uintmax_t water_dirt_level = 0;
    std::uintmax_t amount_of_people = 0;

    std::vector<filter> filters(num_filters, filter{ timestamp, water_dirt_level, events });
    
    std::function<std::uintmax_t(std::uintmax_t)> dirt_function = linear_of_people;
    std::function<std::uintmax_t(std::uintmax_t, std::uintmax_t)> people_function = normal_people_distribution;

    while (timestamp < max_timestamp)
    {
        water_dirt_level += dirt_function(amount_of_people);
        amount_of_people += people_function(timestamp, amount_of_people);
        
        for (filter & f : filters)
        {
            f.do_filter();
        }
        
        std::cout << "At timestamp " << timestamp << ": people = " << amount_of_people << ", dirt = " << water_dirt_level << '\n';
        
        while (events.size() && events.top().timestamp() <= timestamp)
        {
            const event & e = events.top();
            e.handle(timestamp);
            
            events.pop();
        }
        
        timestamp += timestep;
    }
}
