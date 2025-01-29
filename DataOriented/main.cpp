#include "ObjectOrientedMethod.h"
#include "DataOrientedMethod.h"
#include "Data.h"

#include <iomanip>
#include <chrono>

int main()
{
    ////////////// Init //////////////
    Data dataGenerator;
    ObjectOrientedMethod OOP;
    DataOrientedMethod DOD;

	int dataSize = 10000;
    std::vector<Data::Employee> baseData = dataGenerator.createEmployeeData(dataSize);
    printf("Generating Data for %d employees\n", dataSize);

    ////////////// Object Oriented Method //////////////
    printf("\n----------------------------------------------");
    printf("\nStarting Object oriented...\n");
    printf("----------------------------------------------");
    auto startOOP = std::chrono::high_resolution_clock::now();

    std::vector<Data::Employee> OOP_EmployeeOver50k = OOP.GetEmployeeByIncome(baseData, 50000);
    std::vector<Data::Employee> OOP_EmployeeWithRaise = OOP.IncreaseEmployeeSalary(baseData, 10000);
    std::vector<Data::Employee> OOP_NewEmployeeOver50k = OOP.GetEmployeeByIncome(OOP_EmployeeWithRaise, 50000);

    auto endOOP = std::chrono::high_resolution_clock::now();
    auto durationOOP = std::chrono::duration_cast<std::chrono::microseconds>(endOOP - startOOP);

    OOP.PrintEmployeeStats(OOP_EmployeeOver50k, "OOP data:");
    OOP.PrintEmployeeStats(OOP_NewEmployeeOver50k, "OOP after processing:");
    printf("----------------------------------------------\n");

    ////////////// Data Oriented Method //////////////
    printf("\n----------------------------------------------");
    printf("\nStarting Data oriented...\n");
    printf("----------------------------------------------");
    DOD.PrepareData(baseData);

    auto startDOP = std::chrono::high_resolution_clock::now();

    std::vector<size_t> DOD_EmployeeOver50k = DOD.GetEmployeeByIncome(50000);
    DOD.IncreaseEmployeeSalary(10000);
    std::vector<size_t> DOD_NewEmployeeOver50k = DOD.GetEmployeeByIncome(50000);

    auto endDOP = std::chrono::high_resolution_clock::now();
    auto durationDOP = std::chrono::duration_cast<std::chrono::microseconds>(endDOP - startDOP);

    DOD.PrintEmployeeStats(DOD_EmployeeOver50k, "DOD data:");
    DOD.PrintEmployeeStats(DOD_NewEmployeeOver50k, "DOD after processing:");
    printf("----------------------------------------------\n");

    double secondsOOP = durationOOP.count() / 1000000.0;
    double secondsDOD = durationDOP.count() / 1000000.0;

    printf("\nBenchmark Results:\n");
    printf("OOP time: %.6fs (%lld microseconds)\n", secondsOOP, durationOOP.count());
    printf("DOD time: %.6fs (%lld microseconds)\n", secondsDOD, durationDOP.count());

    return 0;
}
