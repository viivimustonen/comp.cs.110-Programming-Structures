/*
#############################################################################
# COMP.CS.110 Programming 2: Autumn 2020                                    #
# Project3: Company                                                         #
# File: company.cpp                                                         #
# Description: Company hierarchy -class datastructure                       #
#       Datastructure is populated with Employee-structs and provides some  #
#       query-functions. This file implements Company class described in    #
#       company.hh file.                                                    #
# Notes:                                                                    #
# Program author: * Name: Viivi Mustonen                                    #
#                 * Student number: H293452                                 #
#                 * UserID: tgvimu                                          #
#                 * E-Mail: viivi.mustonen@tuni.fi                          #
#                                                                           #
#############################################################################
*/

#include "company.hh"

Company::Company()
{

}

Company::~Company()
{
    for(auto iter : workers_){
        iter.second->boss_ = nullptr;
        iter.second->subordinates_.clear();
        delete iter.second;
    }
    workers_.clear();
}

void Company::addNewEmployee(const std::string &id, const std::string &dep,
                             const double &time, std::ostream &output)
{
    if(workers_.find(id) != workers_.end()){
        output << "Error. Employee already added." << std::endl;
    } else{
        //New employee, set all information to empl pointer.
        Employee* empl = new Employee;
        empl->id_ = id;
        empl->department_ = dep;
        empl->time_in_service_ = time;

        //Add newly created employee to workers map
        workers_.insert({id, empl});
    }
}

void Company::printEmployees(std::ostream &output) const
{
    //Print all employees from workers map alphabetically
    for (auto iter : workers_) {
        output << iter.second->id_ << ", " << iter.second->department_
              << ", " << iter.second->time_in_service_ << std::endl;
    }
}

void Company::addRelation(const std::string &subordinate,
                          const std::string &boss, std::ostream &output)
{
    Employee* sub = nullptr;
    Employee* jefe = nullptr;

    if(workers_.find(subordinate) != workers_.end()){
        //Subordinate was found in workers map
        sub = getPointer(subordinate);
    } else{
        printNotFound(subordinate, output);
        return;
    }
    if(workers_.find(boss) != workers_.end()){
        //Boss was found in workers map
        jefe = getPointer(boss);
    } else {
        //No need to print error, if boss is not found. There can be employees
        //without boss
        return;
    }

    // Add relations since subordinate and boss both were found
    sub->boss_ = jefe;
    jefe->subordinates_.push_back(sub);
}

void Company::printBoss(const std::string &id, std::ostream &output) const
{
    Employee* sub = nullptr;
    Employee* jefe = nullptr;

    if(workers_.find(id) != workers_.end()){
        //Subordinate was found in workers map
        sub = getPointer(id);
    } else{
        printNotFound(id, output);
        return;
    }

    //Set subordinates boss to pointer jefe
    jefe = sub->boss_;

    IdSet set;
    if(jefe != nullptr){
        //If boss was found, add to set
        set.insert(jefe->id_);
    }

    //Call printGroup function to print boss
    printGroup(id, "bosses", set, output);
}

void Company::printSubordinates(const std::string &id,
                                std::ostream &output) const
{
    Employee* worker = nullptr;

    if(workers_.find(id) != workers_.end()){
        //Worker was found in workers map
        worker = getPointer(id);
    } else{
        printNotFound(id, output);
        return;
    }

    //Put worker's subordinates from vector to set
    IdSet set = VectorToIdSet(worker->subordinates_);

    //Call printGroup function to print subordinates
    printGroup(id, "subordinates", set, output);
}

void Company::printColleagues(const std::string &id, std::ostream &output) const
{
    Employee* worker = nullptr;
    Employee* jefe = nullptr;

    if(workers_.find(id) != workers_.end()){
        //Worker was found in workers map
        worker = getPointer(id);
    } else{
        printNotFound(id, output);
        return;
    }

    //Set workers boss to pointer jefe
    jefe = worker->boss_;

    IdSet set;
    if(jefe != nullptr){
        //If boss was found, add boss's subordinates from vector to set
        set = VectorToIdSet(jefe->subordinates_);

        //Remove worker from list of colleagues
        set.erase(set.find(worker->id_));
    }

    //Call printGroup function to print worker's colleagues
    printGroup(id, "colleagues", set, output);
}

void Company::findSubordinatesDepartment(Employee *&worker,
           std::string& department, std::vector<Employee *> &subordinates) const
{
    for(auto iter : worker->subordinates_){
        //Goes through worker's subordinates
       if (iter->department_ == department){
           //If they have the same department, add subordinate to vector
           subordinates.push_back(iter);
           //Function call's itself recursivly
           findSubordinatesDepartment(iter, department, subordinates);
       }
    }
}

void Company::printDepartment(const std::string &id, std::ostream &output) const
{
    Employee* worker = nullptr;

     if(workers_.find(id) != workers_.end()){
         //Worker was found in workers map
         worker = getPointer(id);
     } else{
         printNotFound(id, output);
         return;
     }
     //Save worker's department
     std::string department = worker->department_;

     //Move up to boss on top of hiearchy in department
     Employee* boss = worker;
     while (boss->boss_ != nullptr and boss->department_ == department){
         boss = boss->boss_;
     }

     std::vector<Employee*> subs;

     //If boss at the top of hierachy belongs to department, add boss to vector
     if(boss->department_ == department){
         subs.push_back(boss);
     }
     //Find all subordinates of "boss" that belong to department
     findSubordinatesDepartment(boss, department, subs);

     IdSet set = VectorToIdSet(subs);

     //Remove worker from list of department colleagues
     set.erase(set.find(worker->id_));

     //Call printGroup function to print worker's deparment colleagues
     printGroup(id, "department colleagues", set, output);
}

void Company::longestServingWorker(Employee* worker, double& longestTime,
                                   Employee*& longest) const
{
    //Goes through worker's subordinates
    for (auto iter : worker->subordinates_){
        double time_in_serv = iter->time_in_service_;

        if (time_in_serv > longestTime){
            //If subordinates time in service is longer than longest time in
            //service, switch longest serving worker to the subordinate and
            //switch longest time in service to subordinates time in service
            longest = iter;
            longestTime = time_in_serv;
        }
        //Function call's itself recursivly
        longestServingWorker(iter, longestTime, longest);
    }
}

void Company::printLongestTimeInLineManagement(const std::string &id,
                                               std::ostream &output) const
{
    Employee* longest = nullptr;
    Employee* self = nullptr;
    double longest_time = NO_TIME;

    if(workers_.find(id) != workers_.end()){
        //Worker was found in workers map, set longest to worker
        longest = getPointer(id);
        self = longest;
        //First the longest time is worker's time in service
        longest_time = longest->time_in_service_;
        //Call longestServingWorker to find out longest serving worker
        longestServingWorker(self, longest_time, longest);
    } else{
        printNotFound(id, output);
        return;
    }

    if(self == longest){
        //If self (= worker) is the longest serving worker
        output << "With the time of " << longest_time <<", " << longest->id_
               << " is the longest-served employee in their line management."
               << std::endl;
    } else{
        output << "With the time of " << longest_time <<", " << longest->id_
               << " is the longest-served employee in " << self->id_
               << "'s line management." << std::endl;
    }
}

void Company::shortestServingWorker(Employee* worker, double& shortestTime,
                                    Employee*& shortest) const
{
    //Goes through worker's subordinates
    for (auto iter : worker->subordinates_){
        double time_in_serv = iter->time_in_service_;

        if (time_in_serv < shortestTime){
            //If subordinates time in service is shorter than shortest time in
            //service, switch shortest serving worker to the subordinate and
            //switch shortest time in service to subordinates time in service
            shortest = iter;
            shortestTime = time_in_serv;
        }
        //Function call's itself recursivly
        shortestServingWorker(iter, shortestTime, shortest);
    }
}

void Company::printShortestTimeInLineManagement(const std::string &id,
                                                std::ostream &output) const
{
    Employee* shortest = nullptr;
    Employee* self = nullptr;
    double shortest_time = NO_TIME;

    if(workers_.find(id) != workers_.end()){
        //Worker was found in workers map, set shortest to worker
        shortest = getPointer(id);
        self = shortest;
        //First the shortest time is worker's time in service
        shortest_time = shortest->time_in_service_;
        //Call shortestServingWorker to find out shortest serving worker
        shortestServingWorker(self, shortest_time, shortest);
    } else{
        printNotFound(id, output);
        return;
    }

    if(self == shortest){
        //If self (= worker) is the shortest serving worker
        output << "With the time of " << shortest_time <<", " << shortest->id_
               << " is the shortest-served employee in their line management."
               << std::endl;
    } else{
        output << "With the time of " << shortest_time <<", " << shortest->id_
               << " is the shortest-served employee in " << self->id_
               << "'s line management." << std::endl;
    }
}

void Company::findBosses(Employee *&worker, const int &n,
                         std::vector<Employee *> &bosses) const
{
    Employee* jefe = worker->boss_;
    if(n > 0 and jefe != nullptr){
        //If n (level) is bigger than zero and boss isn't nullpointer, add boss
        //(jefe) to bosses vector
        bosses.push_back(jefe);
        //Function call's itself recursivly
        findBosses(jefe, n-1, bosses);
    }
}

void Company::printBossesN(const std::string &id, const int n,
                           std::ostream &output) const
{
    std::vector<Employee*> bosses;
    Employee* sub = nullptr;

    if(n < 1){
        output << "Error. Level can't be less than 1." << std::endl;
        return;
    }

    if(workers_.find(id) != workers_.end()){
        //Worker was found in workers map
        sub = getPointer(id);

        if(sub->boss_ == nullptr){
            output << id << " has no bosses." << std::endl;
            return;
        }

        //Worker has at least one boss, find other bosses
        findBosses(sub, n, bosses);

    } else{
        printNotFound(id, output);
        return;
    }

    IdSet set = VectorToIdSet(bosses);

    //Print worker's bosses alphabetically
    output << id << " has " << set.size() << " bosses:" << std::endl;
    for(auto iter : set){
        output << iter << std::endl;
    }
}

void Company::findSubordinates(Employee *&worker, const int &n,
                               std::vector<Employee *> &subordinates) const
{
    if(n > 0 and worker->subordinates_.size() > 0){
        //If n (level) is bigger than zero and worker has at least one
        //subordinate
        for(auto iter : worker->subordinates_){
            //Goes through worker's subordinates and adds them to vector
            subordinates.push_back(iter);
            //Function call's itself recursivly
            findSubordinates(iter, n-1, subordinates);
        }
    }
}

void Company::printSubordinatesN(const std::string &id, const int n,
                                 std::ostream &output) const
{
    std::vector<Employee*> subordinates;
    Employee* worker = nullptr;

    if(n < 1){
        output << "Error. Level can't be less than 1." << std::endl;
        return;
    }

    if(workers_.find(id) != workers_.end()){
        //Worker was found in workers map
        worker = getPointer(id);

        if(worker->subordinates_.size() == 0){
            output << id << " has no subordinates." << std::endl;
            return;
        }

        //Worker has at least one subordinate, find other subordinates
        findSubordinates(worker, n, subordinates);

    } else{
        printNotFound(id, output);
        return;
    }

    IdSet set = VectorToIdSet(subordinates);

    //Print worker's subordinates alphabetically
    output << id << " has " << set.size() << " subordinates:" << std::endl;
    for(auto iter : set){
        output << iter << std::endl;
    }
}

Employee *Company::getPointer(const std::string &id) const
{
    if(workers_.find(id) != workers_.end()){
        //Worker was found in workers map, return pointer to worker
        return workers_.at(id);
    } else{
        return nullptr;
    }
}

void Company::printNotFound(const std::string &id, std::ostream &output) const
{
    if(workers_.find(id) == workers_.end()){
        //Worker was not found in workers map
        output << "Error. " << id << " not found." << std::endl;
    }
}

IdSet Company::VectorToIdSet(const std::vector<Employee *> &container) const
{
    IdSet set;

    for (auto iter : container) {
        //Goes through vector and adds contents to set
        set.insert(iter->id_);
    }

    return set;
}

void Company::printGroup(const std::string &id, const std::string &group,
                         const IdSet &container, std::ostream &output) const
{
    if(container.size() == 0){
        output << id <<" has no " << group << "." << std::endl;
    } else {
        //Container wasn't empty, print containers contents
        output << id << " has " << container.size() << " " << group
               << ":" << std::endl;

        for (auto iter : container) {
            //Print contents alphabetically
            output << getPointer(iter)->id_ << std::endl;
        }
    }
}
