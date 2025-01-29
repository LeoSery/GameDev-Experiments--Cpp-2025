#include "ObjectOrientedMethod.h"
#include "StatsHelper.h"
#include "Data.h"

#include <iostream>
#include <map>

/**
* @brief Increases the salary of all employees
* @param data Vector of employees to process
* @param increase Amount to increase salary by
* @return Vector of employees with updated salaries
*/
std::vector<Data::Employee> ObjectOrientedMethod::IncreaseEmployeeSalary(std::vector<Data::Employee> data, double increase) const
{
	std::vector<Data::Employee> newData;
    newData.resize(data.size());

    #pragma omp parallel for
    for (size_t i = 0; i < data.size(); i++)
    {
        Data::Employee newEmployee = data[i];
        newEmployee.salary += increase;
        newData[i] = newEmployee;
    }

	return newData;
}

/**
* @brief Filters employees based on income threshold
* @param data Vector of employees to filter
* @param income Minimum income threshold
* @return Vector of employees above the income threshold
*/
std::vector<Data::Employee> ObjectOrientedMethod::GetEmployeeByIncome(std::vector<Data::Employee> data, double income) const
{
	std::vector<Data::Employee> newData;
    newData.reserve(data.size() / 2);

    for (const Data::Employee& employee : data)
    {
        if (employee.salary > income)
        {
            newData.push_back(employee);
        }
    }

	return newData;
}

/**
* @brief Prints statistical information about a group of employees
* @param employees Vector of employees to analyze
* @param printTitle Title to display in the statistics output
*/
void ObjectOrientedMethod::PrintEmployeeStats(const std::vector<Data::Employee>& employees, const std::string& printTitle) const
{
    if (employees.empty())
    {
        return;
    }

    double totalSalary = 0;
    int totalAge = 0;

    #pragma omp parallel for reduction(+:totalSalary,totalAge)
    for (size_t i = 0; i < employees.size(); i++)
    {
        totalSalary += employees[i].salary;
        totalAge += employees[i].age;
    }

    std::map<std::string, int> deptCount;
    for (const Data::Employee& emp : employees)
    {
        deptCount[emp.department]++;
    }

    StatsHelper::PrintStats(printTitle, employees.size(), static_cast<double>(totalAge) / employees.size(), totalSalary / employees.size(), deptCount);
}
