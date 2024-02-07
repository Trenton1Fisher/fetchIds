/*
Skeleton code for storage and buffer management
*/

#include <string>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes.h"
using namespace std;

int main(int argc, char *const argv[])
{
    int userChoice = 1;
    int managerIdChoice;

    // Create the EmployeeRelation file from Employee.csv
    StorageBufferManager manager("EmployeeRelation.dat");
    manager.createFromFile("Employee.csv");
    // Loop to lookup IDs until user is ready to quit
    while (userChoice == 1)
    {
        cout << "Enter Manager id you would like to search: ";
        scanf("%d", &managerIdChoice);
        // do searching
        manager.findRecordById(managerIdChoice);
        cout << "Would like to search again or end the program (0 = End Program, 1 = Continue): ";
        cin >> userChoice;
    }

    return 0;
}
