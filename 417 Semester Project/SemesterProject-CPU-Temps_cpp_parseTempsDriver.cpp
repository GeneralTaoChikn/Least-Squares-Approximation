#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>
#include <memory>
#include <iterator>
#include <algorithm>
#include <utility>

#include "parseTemps.h"

using namespace std;

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    // Input validation(s)
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " input_file_name" << "\n";
        return 1;
    }

    ifstream input_temps(argv[1]);
    if (!input_temps) {
        cout << "ERROR: " << argv[1] << " could not be opened" << "\n";
        return 2;
    }
    // End Input Validation

    vector<CoreTempReading> readings = parse_raw_temps(input_temps);

    // Output everything to match the Python version
    for (const CoreTempReading& theReading : readings) {
        cout << "(" << theReading.first << ", [";

        const vector<double>& coreTemps = theReading.second;
        for (int i = 0; i < coreTemps.size() - 1; i++) {
            cout << coreTemps[i] << ", ";
        }
        cout << *(coreTemps.end() - 1) << "])" << "\n";
    }

    return 0;
}
