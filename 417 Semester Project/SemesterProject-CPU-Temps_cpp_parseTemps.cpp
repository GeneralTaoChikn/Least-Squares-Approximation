#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <utility>

#include "parseTemps.h"

//------------------------------------------------------------------------------
std::vector<CoreTempReading> parse_raw_temps(std::istream& original_temps,
                                             int step_size)
{
    std::vector<CoreTempReading> allTheReadings;

    // Input Parsing Variables
    int step = 0;
    std::string line;

    while (getline(original_temps, line)) {
        std::istringstream input(line);

        std::vector<double> next_temperature_set;
        std::transform(std::istream_iterator<std::string>(input),
                       std::istream_iterator<std::string>(),
                       std::back_inserter(next_temperature_set),
                       [](const std::string& raw_reading) -> double {
                           return stod(raw_reading);
                       });

        allTheReadings.emplace_back(step, next_temperature_set);
        step += step_size;
    }

    return allTheReadings;
}

