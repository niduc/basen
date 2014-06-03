#include <queue>
#include <functional>
#include <random>
#include <fstream>

#include "event.h"
#include "filter.h"

constexpr std::uintmax_t timestep = 1;            // half an hour
std::uintmax_t max_timestamp = 2 * 24 * 365;      // a year
std::uintmax_t num_filters = 3;                   // some filters

std::random_device rd;
std::mt19937 engine{ rd() };

std::uintmax_t linear_of_people(std::uintmax_t num_people)
{
    return num_people * 3;
}

std::uintmax_t pool_volume = 25 * 50 * 2;
std::uintmax_t people_limit = 300;
std::uintmax_t ticket_price = 10;

std::uintmax_t open_at = 6;
std::uintmax_t close_at = 22;

std::intmax_t normal_people_distribution(std::uintmax_t timestamp, std::uintmax_t amount_of_people)
{
    timestamp /= 2;
    
    if ((timestamp % 24) < open_at || (timestamp % 24) > close_at)
    {
        return -amount_of_people;
    }
    
    static std::normal_distribution<> dist(0, people_limit / 10);
    
    auto change = std::round(dist(engine));
    if (amount_of_people >= people_limit && change > 0)
    {
        change = 0;
    }
    
    return std::max<std::intmax_t>(change, -amount_of_people);
}

std::uintmax_t repairman_salary = 50;                       // per half an hour
std::uintmax_t filter_cost = 15000;                         // let's make a guess
std::intmax_t money = -filter_cost * num_filters;

void pay_filter_cost()
{
    money -= filter_cost;
}

// either no arguments or: max_timestamp num_filters pool_volume people_limit ticket_price repairman_salary filter_cost open_at close_at
int main(int argc, char ** argv)
{
    if (argc == 10)
    {
        max_timestamp = std::stoll(argv[1]);
        num_filters = std::stoll(argv[2]);
        pool_volume = std::stoll(argv[3]);
        people_limit = std::stoll(argv[4]);
        ticket_price = std::stoll(argv[5]);
        repairman_salary = std::stoll(argv[6]);
        filter_cost = std::stoll(argv[7]);
        open_at = std::stoll(argv[8]);
        close_at = std::stoll(argv[9]);
    }
    
    std::fstream log{ "basen.csv", std::ios::binary | std::ios::out };
    
    std::priority_queue<event, std::vector<event>, std::greater<event>> events;
    std::uintmax_t timestamp = 0;
    std::uintmax_t repairman_available_at = 0;
        
    std::uintmax_t water_dirt_level = 0;
    std::uintmax_t amount_of_people = 0;
    
    std::uintmax_t alarming_level_timestamps = 0;

    std::vector<filter> filters(num_filters, filter{ timestamp, water_dirt_level, events, repairman_available_at });
    
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
        log << timestamp << ',' << amount_of_people << ',' << water_dirt_level << '\n';
        
        while (events.size() && events.top().timestamp() <= timestamp)
        {
            const event & e = events.top();
            e.handle(timestamp);
            
            events.pop();
        }
        
        timestamp += timestep;
        money += amount_of_people;
        
        if (water_dirt_level / pool_volume >= 50)
        {
            ++alarming_level_timestamps;
        }
        
        if (repairman_available_at > timestamp)
        {
            money -= repairman_salary;
        }
    }
    
    std::cout << "Alarming level timestamp count: " << alarming_level_timestamps << '\n';
    std::cout << "Money: " << money << '\n';
    
    log << '\n';
    log << alarming_level_timestamps << ',' << money << '\n';
}
