#pragma once

#include "Data.h"

#include <vector>
#include <string>

/**
 * @brief Class implementing object-oriented approach for employee data management
 *
 * This class demonstrates traditional object-oriented programming patterns
 * for handling employee data operations.
 */
class ObjectOrientedMethod
{
public:

    //////// METHODS ////////
    //// Data Operations
	std::vector<Data::Employee> IncreaseEmployeeSalary(std::vector<Data::Employee> data, double increase) const;
	std::vector<Data::Employee> GetEmployeeByIncome(std::vector<Data::Employee> data, double income) const;
	void PrintEmployeeStats(const std::vector<Data::Employee>& employees, const std::string& printTitle) const;

};
