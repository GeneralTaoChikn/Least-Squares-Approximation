#include <iostream>

#include "matrix.h"

using namespace std;

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    // Input validation
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


    //Read in here
    Matrix matrix(input_temps);

    //Set Argument as a String
    string FileName = argv[1];

    //Pass in as a String
    matrix.display(FileName);


    return 0;

}
