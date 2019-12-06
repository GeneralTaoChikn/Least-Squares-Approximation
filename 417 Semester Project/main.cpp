#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>
#include <memory>
#include <iterator>
#include <algorithm>
#include <utility>

#include "matrix.h"

using namespace std;

//------------------------------------------------------------------------------
//////int main(int argc, char** argv)
int main()
{
//////    // Input validation
//////    if (argc < 2) {
//////        cout << "Usage: " << argv[0] << " input_file_name" << "\n";
//////        return 1;
//////    }
//////
//////    ifstream input_temps(argv[1]);
//////    if (!input_temps) {
//////        cout << "ERROR: " << argv[1] << " could not be opened" << "\n";
//////        return 2;
//////    }
//////    // End Input Validation

    ifstream input_temps("Test2 - Copy.txt");

//    ifstream input_temps("Test1.txt");

    //Read in here
    Matrix matrix(input_temps);

    //try to display
//    string FileName = argv[1];
    string FileName = "Test1.txt";
    matrix.display(FileName);


    return 0;

}
