#pragma once

#include "Data.h"

#include <vector>
#include <string>
#include <map>

/**
 * @brief Class implementing data-oriented approach for employee data management
 *
 * This class demonstrates data-oriented design patterns with data organized
 * for optimal cache usage and SIMD operations.
 */
class DataOrientedMethod
{
public:
    //////// METHODS ////////
    //// Data Operations
    void PrepareData(const std::vector<Data::Employee>& data);
    void IncreaseEmployeeSalary(double increase);
    std::vector<size_t> GetEmployeeByIncome(double income) const;
    void PrintEmployeeStats(const std::vector<size_t>& indices, const std::string& printTitle) const;


private:
    //////// STRUCTS ////////

    /**
     * @brief Structure containing numeric employee data
     *
     * Organized for optimal memory layout and SIMD operations
     */
    struct NumericData
    {
        std::vector<int> ids;
        std::vector<int> ages;
        std::vector<double> salaries;
    } numData;

    /**
     * @brief Structure containing textual employee data
     *
     * Separated from numeric data to improve cache efficiency
     */
    struct TextData
    {
        std::vector<std::string> names;
        std::vector<std::string> departments;
    } textData;

    //////// FIELDS ////////
    size_t dataSize = 0;
};
