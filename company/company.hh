/*
#############################################################################
# COMP.CS.110 Programming 2: Autumn 2020                                    #
# Project3: Company                                                         #
# File: company.hh                                                          #
# Description: Company hierarchy -class datastructure                       #
#       Datastructure is populated with Employee-structs and provides some  #
#       query-functions.                                                    #
# Notes: * This is a part of an exercise program                            #
#        * Student's aren't allowed to alter public interface!              #
#        * All changes to private side are allowed.                         #
# Program author: * Name: Viivi Mustonen                                    #
#                 * Student number: H293452                                 #
#                 * UserID: tgvimu                                          #
#                 * E-Mail: viivi.mustonen@tuni.fi                          #
#                                                                           #
#############################################################################
*/
#ifndef COMPANY_HH
#define COMPANY_HH

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <map>


//Named constants to improve readability in other modules.
const std::string EMPTY = "";
const double NO_TIME = -1.0;

//Struct for an employee.
struct Employee
{
    std::string id_ = EMPTY;
    std::string department_ = EMPTY;
    double time_in_service_ = NO_TIME;
    Employee* boss_ = nullptr;
    std::vector<Employee*> subordinates_;
};


using IdSet = std::set<std::string>;

/*  Class: Company
 *  Description: Datastructure for Employee-structs
 */
class Company
{
public:
    Company();
    ~Company();

    /* Description: Adds a new Employee to the datastructure.
     * Parameters:
     *  Param1: Employee's ID string
     *  Param2: Employee's department
     *  Param3: Employee's time in service
     *  Param4: Output-stream for error-printing
     * Errormessages:
     *  If employees's ID is already in datastructure:
     *      "Error. Employee already added."
     */
    void addNewEmployee(const std::string& id, const std::string& dep,
                        const double& time, std::ostream &output);

    /* Description: Prints all stored Employees: ID, department and time in
     * service
     * Parameters:
     *  Param1: Output-stream for printing
     * Errormessages:
     *  None.
     */
    void printEmployees(std::ostream &output) const;


    /*
     * All the following functions have the same error messages:
     *  If ID wasn't found in datastructure:
     *      "Error. <ID> not found."
     *  If the printing list is empty:
     *      "Error. <ID> has no <group's name>."
     * -------------------------------------------------------------------
     */

    /* Description: Adds new boss-subordinate relation
     * Parameters:
     *  Param1: ID of the subordinate
     *  Param2: ID of the boss
     *  Param3: Output-stream for error-printing
     */
    void addRelation(const std::string& subordinate,
                     const std::string& boss,
                     std::ostream& output);

    /* Description: Prints the direct boss of the employee.
     * Parameters:
     *  Param1: ID of the employee
     *  Param2: Output-stream for printing
     */
    void printBoss(const std::string& id,
                   std::ostream& output) const;

    /* Description: Prints direct subordinates for the employee.
     * Parameters:
     *  Param1: ID of the employee
     *  Param2: Output-stream for printing
     */
    void printSubordinates(const std::string& id,
                           std::ostream& output) const;

    /* Description: Prints the colleagues for the employee.
     *  (Employees who shares their direct boss)
     * Parameters:
     *  Param1: ID of the employee
     *  Param2: Output-stream for printing
     */
    void printColleagues(const std::string& id,
                         std::ostream& output) const;

    /* Description: Prints all-level colleagues for the employee.
     *  (Employees who share their department and belong to the same hierarchy)
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Output-stream for printing
     */
    void printDepartment(const std::string& id,
                         std::ostream& output) const;

    /* Description: Prints the employee with the longest time in service
     *  in the ID's line management.
     * Parameters:
     *  Param1: ID of the employee
     *  Param2: Output-stream for printing
     */
    void printLongestTimeInLineManagement(const std::string& id,
                                          std::ostream& output) const;

    /* Description: Prints the employee with the shortest time in service
     *  in the ID's line management.
     * Parameters:
     *  Param1: ID of the employee
     *  Param2: Output-stream for printing
     */
    void printShortestTimeInLineManagement(const std::string& id,
                                           std::ostream& output) const;


    /* The following functions have additional errormessage:
     *  If Param2's value is less than 1:
     *      "Error. Level can't be less than 1."
     * ---------------------------------------------------------------
     */

    /* Description: Prints the amount and names of bosses in given
     *  distance from the employee.
     * Parameters:
     *  Param1: ID of the employee
     *  Param2: Maximum distance from the employee. (n-1 times "hierarchy")
     *  Param3: Output-stream for printing
     */
    void printBossesN(const std::string& id, const int n,
                      std::ostream& output) const;

    /* Description: Prints the amount and names of subordinates in given
     *  distance from the employee.
     * Parameters:
     *  Param1: ID of the employee
     *  Param2: Maximum distance from the employee. (n-1 times "hierarchy")
     *  Param3: Output-stream for printing
     */
    void printSubordinatesN(const std::string& id, const int n,
                            std::ostream& output) const;

private:

    /* The following functions are meant to make project easier.
     * You can implement them if you want and/or create your own.
     * Anyway it would be a good idea to implement more functions
     * to make things easier and to avoid "copy-paste-coding"
     */

    //Return a pointer for ID.
    Employee* getPointer(const std::string& id) const;

    //Printing errors.
    void printNotFound(const std::string& id,
                       std::ostream& output) const;

    //Turns a vector of employees to a set of IDs.
    IdSet VectorToIdSet(const std::vector<Employee*> &container) const;

    //Prints the the data in a container.
    void printGroup(const std::string& id, const std::string& group,
                    const IdSet& container, std::ostream& output) const;

    /* Description: Goes through worker's subordinates and if worker and
     * subordinate have the same department, saves subordinate to vector.
     * Parameters:
     *  Param1: employee pointer
     *  Param2: department
     *  Param3: vector of pointers to empolyee's subordinates
     */
    void findSubordinatesDepartment(Employee *&worker, std::string& department,
                                   std::vector<Employee *> &subordinates) const;

    /* Description: Finds longest serving worker from employee's subordinates.
     * If subordinates time in service is longer than employee's switch longest
     * serving worker to subordinate. Then go through subordinate's subordinates.
     * Parameters:
     *  Param1: employee pointer
     *  Param2: longest time service
     *  Param3: longest serving employee pointer
     */
    void longestServingWorker (Employee* worker, double& longestTime,
                               Employee*& longest) const;

    /* Description: Finds shortest serving worker from employee's subordinates.
     * If subordinates time in service is shorter than employee's switch
     * shortest serving worker to subordinate. Then goes through subordinate's
     * subordinates.
     * Parameters:
     *  Param1: employee pointer
     *  Param2: shortest time service
     *  Param3: shortest serving employee pointer
     */
    void shortestServingWorker (Employee* worker, double& shortestTime,
                                Employee*& shortest) const;

    /* Description: Goes through employee's bosses and adds them to a vector.
     * Then goes through employee's boss's boss etc.
     * Parameters:
     *  Param1: employee pointer
     *  Param2: depth n
     *  Param3: vector of boss pointers
     */
    void findBosses(Employee*& worker, const int& n,
                    std::vector<Employee*>& bosses) const;

    /* Description: Goes through employee's subordinates and adds them to a
     * vector. Then goes through employee's subordinate's subordinates etc.
     * Parameters:
     *  Param1: employee pointer
     *  Param2: depth n
     *  Param3: vector of subordinate pointers
     */
    void findSubordinates(Employee*& worker, const int& n,
                          std::vector<Employee*>& subordinates) const;


    //Map containing pointer to all employees in the company
    std::map<std::string, Employee*> workers_;

};

#endif // COMPANY_HH
