#include "DataOrientedMethod.h"
#include "StatsHelper.h"

#include <iostream>

/**
* @brief Prepares data structures for DOD processing
* @param data Vector of employee data to prepare
*/
void DataOrientedMethod::PrepareData(const std::vector<Data::Employee>& data)
{
    dataSize = data.size();

    numData.ids.reserve(dataSize);
    numData.ages.reserve(dataSize);
    numData.salaries.reserve(dataSize);
    textData.names.reserve(dataSize);
    textData.departments.reserve(dataSize);

    for (const Data::Employee& emp : data)
    {
        numData.ids.push_back(emp.id);
        numData.ages.push_back(emp.age);
        numData.salaries.push_back(emp.salary);
        textData.names.push_back(emp.name);
        textData.departments.push_back(emp.department);
    }
}

/**
* @brief Increases all employee salaries using SIMD operations
* @param increase Amount to increase salary by
*/
void DataOrientedMethod::IncreaseEmployeeSalary(double increase)
{
    constexpr size_t BATCH_SIZE = 16;

    #pragma omp parallel for
    for (size_t i = 0; i < dataSize; i += BATCH_SIZE)
    {
        #pragma omp simd
        for (size_t j = 0; j < BATCH_SIZE && (i + j) < dataSize; j++)
        {
            numData.salaries[i + j] += increase;
        }
    }
}

/**
* @brief Filters employees based on income using SIMD operations
* @param income Minimum income threshold
* @return Vector of indices of employees above the income threshold
*/
std::vector<size_t> DataOrientedMethod::GetEmployeeByIncome(double income) const
{
    std::vector<size_t> validIndices;
    validIndices.reserve(dataSize / 4);

    constexpr size_t BATCH_SIZE = 16;

    for (size_t i = 0; i < dataSize; i += BATCH_SIZE)
    {
        #pragma omp simd
        for (size_t j = 0; j < BATCH_SIZE && (i + j) < dataSize; j++)
        {
            if (numData.salaries[i + j] > income)
            {
                validIndices.push_back(i + j);
            }
        }
    }

    validIndices.shrink_to_fit();
    return validIndices;
}

/**
* @brief Prints statistical information about a group of employees
* @param indices Vector of indices of employees to analyze
* @param printTitle Title to display in the statistics output
*/
void DataOrientedMethod::PrintEmployeeStats(const std::vector<size_t>& indices, const std::string& printTitle) const
{
    if (indices.empty()) return;

    double totalSalary = 0;
    int totalAge = 0;

    #pragma omp parallel for reduction(+:totalSalary,totalAge)
    for (size_t i = 0; i < indices.size(); i++)
    {
        size_t idx = indices[i];
        totalSalary += numData.salaries[idx];
        totalAge += numData.ages[idx];
    }

    std::map<std::string, int> deptCount;
    for (size_t idx : indices)
    {
        deptCount[textData.departments[idx]]++;
    }

    StatsHelper::PrintStats(printTitle, indices.size(), static_cast<double>(totalAge) / indices.size(), totalSalary / indices.size(), deptCount);
}
