#pragma once

#include <string>
#include <map>

/**
 * @brief Static helper class for statistics printing
 *
 * Provides utility functions for formatting and displaying
 * employee statistics in a consistent manner.
 */
class StatsHelper
{
public:

	//////// STATIC METHODS ////////
	static void PrintStats(const std::string& title, int employeeCount, double avgAge, double avgSalary, const std::map<std::string, int>& deptCount);

};
