#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <map>

/**
 * @brief Class responsible for data generation and management
 *
 * This class provides functionality to generate test data for both
 * object-oriented and data-oriented implementations.
 */
class Data
{
public:

    //////// STRUCTS ////////

    /**
     * @brief Structure representing an employee's data
     *
     * Contains all relevant information about an employee including
     * personal information and professional details.
     */
    struct Employee
    {
        int id;
        std::string name;
        int age;
        std::string department;
        double salary;
    };

    //////// METHODS ////////

    /**
     * @brief Creates a dataset of random employee data
     * @param dataSize Number of employees to generate
     * @return Vector containing the generated employee data
     */
    std::vector<Employee> createEmployeeData(const size_t dataSize)
    {
        std::vector<Employee> employees;
        employees.reserve(dataSize);

        std::mt19937 rng(std::time(nullptr));

        std::vector<std::string> firstNames = {
            "John", "Mary", "Peter", "Sophie", "Thomas", "Julia", "Nicholas", "Emma",
            "Luke", "Leah", "Anthony", "Clara", "Hugo", "Chloe", "Louis", "Camille",
            "Gabriel", "Sarah", "Alexander", "Laura", "Max", "Julia", "Paul", "Louise",
            "Arthur", "Alice", "Victor", "Monica", "Jules", "Eva", "Nathan", "Iris",
            "Adam", "Charlotte", "Raphael", "Zoe", "Theodore", "Lina", "Samuel", "Anna"
        };

        std::vector<std::string> lastNames = {
            "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis",
            "Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas",
            "Taylor", "Moore", "Jackson", "Martin", "Lee", "Perez", "Thompson", "White",
            "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson", "Walker", "Young",
            "Allen", "King", "Wright", "Scott", "Torres", "Nguyen", "Hill", "Adams"
        };

        std::vector<std::string> departments = {
            "Human Resources", "IT", "Marketing", "Sales", "Finance",
            "Management", "Research and Development", "Production", "Quality",
            "Customer Service", "Communications", "Legal", "Accounting", "Logistics"
        };

        std::uniform_int_distribution<> age_dist(22, 65);
        std::uniform_int_distribution<> firstname_dist(0, firstNames.size() - 1);
        std::uniform_int_distribution<> lastname_dist(0, lastNames.size() - 1);
        std::uniform_int_distribution<> dept_dist(0, departments.size() - 1);
        std::normal_distribution<> salary_dist(50000, 15000);

        for (int i = 0; i < dataSize; ++i)
        {
            Employee emp;
            emp.id = 1001 + i;
            emp.name = firstNames[firstname_dist(rng)] + " " + lastNames[lastname_dist(rng)];
            emp.age = age_dist(rng);
            emp.department = departments[dept_dist(rng)];

            double salary = salary_dist(rng);
            emp.salary = std::max(35000.0, std::min(150000.0, salary));

            employees.push_back(emp);
        }

        return employees;
    }
};
