#include "StatsHelper.h"

/**
* @brief Prints formatted statistical information about employees
* @param title Title for the statistics block
* @param employeeCount Total number of employees
* @param avgAge Average employee age
* @param avgSalary Average employee salary
* @param deptCount Map of department counts
*/
void StatsHelper::PrintStats(const std::string& title, int employeeCount, double avgAge, double avgSalary, const std::map<std::string, int>& deptCount)
{
    printf("\n%s\n", title.c_str());
    printf("Total employees: %d\n", employeeCount);
    printf("Average age: %.0f years\n", avgAge);
    printf("Average salary: %.2f$\n", avgSalary);
}
