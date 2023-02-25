/* Education center
 *
 * Desc:
 *  This program generates an education center. First, program asks user for an
 * input file. Input file includes information about courses in different
 * locations. Information includes location, course theme, course name and
 * enrollments for course. Now user can search information about different
 * courses.
 *  User can search all available locations. Or all courses by location and
 * theme (even if they are full). Or all available courses (those that are not
 * full). The courses are listed alphabetically, first by location, then by
 * theme, and finally by the name of the course. Or user can search courses by
 * theme from all locations. Or favorite theme from all locations.
 *
 * Program author
 * Name: Viivi Mustonen
 * Student number: H293452
 * UserID: tgvimu
 * E-Mail: viivi.mustonen@tuni.fi
 * Feedback language (fi/en): fi
 *
 * Notes about the program and it's implementation:
 *
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

const char SEPARATOR = ';';
const int COURSE_FULL = 50;
const int PARTS_IN_FILE = 4;
const int EMPTY = 0;
const std::string ERROR_COMMAND = "Error: error in command ";
const std::string ERROR_LOCATION = "Error: unknown location name";
const std::string ERROR_THEME = "Error: unknown theme";
const std::string UNKNOWN_COMMAND = "Error: Unknown command: ";

struct Course {
    std::string name;
    std::string theme;
    int enrollments;
};

//This function splits wanted line to parts and puts them to vector.
//Parameters are line to be splitted, separator and information about
//empty parts.
std::vector< std::string > split(std::string line, char separator,
                                 bool ignore_empty = false)
{
    std::vector<std::string> parts;
    std::string piece = "";
    for (int i = 0; i < (int)line.size(); ++i){
        if (line.at(i) != separator){

            piece.push_back(line.at(i));
        } else{
            if (piece == "" and ignore_empty == true){
                continue;
            }
            parts.push_back(piece);
            piece = "";
        }
    }

    if (piece != ""){
        parts.push_back(piece);
    } else if (piece == "" and ignore_empty == false){
        parts.push_back(piece);
    }
    return parts;
}


//This function reads the input file and puts lines to lines vector.
bool read_file(std::vector<std::string>& lines)
{
    std::string input_file_name = "";
    std::cout << "Input file: ";
    getline(std::cin, input_file_name);

    std::ifstream input_file(input_file_name);
    if ( not input_file ) {
        return false;

    } else {
        std::string line;

        while ( getline(input_file, line) ) {
            lines.push_back(line);
        }
        input_file.close();
    }
    return true;
}


//This function processes input file line by line and puts splitted line to
//Course struct. Courses are added to courses map by location. If the location
//is new -> new entry to map (location and vector of Course structs). If
//location is already in map, check is course already in location or is it new.
//If course already in location, update it. And if course is new, add course to
//vector.
bool process_line(std::map<std::string, std::vector<Course>>& courses,
                  std::vector<std::string>& lines)
{
    for(size_t i = 0; i < lines.size(); ++i){
        std::string line = lines.at(i);
        std::vector<std::string> parts = split(line, SEPARATOR, true);

        if(parts.size() != PARTS_IN_FILE){
            return false;
        }

        std::string location = parts[0];
        std::string course_name = parts[2];
        std::string course_theme = parts[1];
        std::string s_enrollments = parts[3];
        int enrollments = 0;

        if(s_enrollments == "full"){
            enrollments = COURSE_FULL;
        } else{
            enrollments = stoi(s_enrollments);
        }
        //create course struct
        Course course ={course_name, course_theme, enrollments};

        if(courses.find(location) == courses.end()){
            //new location, add new entry to map
            std::vector<Course> course_vector;
            course_vector.push_back(course);
            courses.insert({location, course_vector});

        }else{
            // location was found in map.
            //check is course already in the location
            bool course_found = false;

            for(int i=0; i < int(courses.at(location).size()); ++i){
                if(courses.at(location)[i].name == course_name
                        and courses.at(location)[i].theme == course_theme){
                    //update course
                    courses.at(location)[i].enrollments = enrollments;
                    course_found = true;
                }
            }

            if (course_found == false) {
                // course was not found, push_back course to vector
                courses.at(location).push_back(course);
            }
        }
    }
    return true;
}


//This function removes unwanted quotation marks. Quotation marks surround one
//theme. Function finds quotation marks and removes them (around one word or
//around several words). Then function replaces words with quotation marks to
//words without quotation marks (in parts vector).
void remove_quotation_marks(std::vector<std::string>& parts, int index)
{
    int start_index = -1;
    int end_index = -1;
    std::string parameter = "";

    for(int i = 0; i < int (parts.size()); ++i){
        if( i < index){
            //skip to index
            continue;

        } else{
            std::string item = parts.at(i);

            if ( item.at(0) == '"'){
                //sets start_index to first word of the theme
                start_index = i;
            }
            if ( item.at(item.length()-1) == '"'){
                //sets end_index to last word of the theme
                end_index = i;
            }
            if (start_index == end_index and start_index > -1){
                //quotation marks are around one word. Remowe quotation marks.
                parameter = item.substr(1, item.length()-2);
                parts.at(i) = parameter;
                return;
            }
            if (parameter.length() > EMPTY){
                //There is already one word in parameter. Add space.
                parameter.append(" ");
            }
            parameter.append(item);
        }
    }

    if (start_index > -1 and end_index > start_index){
        //remove quotation marks from parameter
        parameter = parameter.substr(1, parameter.length()-2);

        //Remove unwanted items (items with quotationmarks) from parts vector
        for (int j = end_index; j >= start_index; --j){
            parts.pop_back();
        }

        //Add paratemter (item wittout quotation marks) to parts vector
        parts.push_back(parameter);
    }
}


//This function prints all the locations in map alphabetically.
void locations( const std::map<std::string, std::vector<Course>>& courses)
{
    for(auto iter: courses){
        std::cout << iter.first << std::endl;;
    }
}


//Finds out what is the favorite theme (or themes) and counts how many
//enrollments are in this theme (or themes). Check if there is multible
//favorite themes.
void favorite_theme(std::map<std::string, std::vector<Course>>& courses)
{
    std::map <std::string, int> enrolments_of_theme;

    std::map<std::string, std::vector<Course>>::iterator iter;
    iter = courses.begin();
    while ( iter != courses.end() ) {

        //loop through all locations
        std::vector<Course> course_list = iter->second;
        std::vector<Course>::iterator v_iter;
        v_iter = course_list.begin();

        while (v_iter != course_list.end()) {
            //loop through all courses of one locations

            if(enrolments_of_theme.find(v_iter->theme) !=
                    enrolments_of_theme.end()){
                //theme was already in map, increase enrollments
                enrolments_of_theme.at(v_iter->theme) += v_iter->enrollments;

            } else{
                //add new theme to map
                enrolments_of_theme.insert({v_iter->theme,
                                            v_iter->enrollments});
            }
            ++v_iter;
        }
        ++iter;
    }

    //Find out themes with max number of participants
    int current_max = 0;
    std::vector<std::string> favorite_themes;

    std::map <std::string, int>::iterator theme_iter;
    theme_iter = enrolments_of_theme.begin();

    while (theme_iter != enrolments_of_theme.end()) {

        if(theme_iter->second > current_max){
            //If new theme enrollments is bigger than current_max, remove
            //old favorite and add new theme to favorite_themes vector.
            favorite_themes.clear();
            current_max = theme_iter->second;
            favorite_themes.push_back(theme_iter->first);

        } else if(theme_iter->second == current_max){
            //If new theme enrollments is same current_max, add new theme
            //to favorite_themes vector.
            favorite_themes.push_back(theme_iter->first);
        }
        ++theme_iter;
    }

    //Print results
    if(int(favorite_themes.size()) == EMPTY){
        std::cout << "No enrollments" << std::endl;
    } else{
        std::cout<< current_max << " enrollments in themes" << std::endl;
        for(int i = 0; i < int(favorite_themes.size()); ++i){
            std::cout << "--- " << favorite_themes.at(i) << std::endl;
        }
    }
}


//This function checks if theme (parameter) belongs to courses map (parameter).
//If true, prints the courses belonging to the given theme from all
//localities in alphabetical order. If the theme is not in courses map
//returns false.
bool courses_in_theme(std::string theme, std::map<std::string,
                      std::vector<Course>>& courses)
{
    std::set<std::string> courses_of_theme;
    std::map<std::string, std::vector<Course>>::iterator iter;
    iter = courses.begin();
    while ( iter != courses.end() ) {

        //Loop through all locations
        std::vector<Course> course_list = iter->second;
        std::vector<Course>::iterator v_iter;
        v_iter = course_list.begin();

        while (v_iter != course_list.end()) {
            //Loop through all courses of one locations.
            //Add course name to set, if theme matches.
            if(v_iter->theme == theme){
                courses_of_theme.insert(v_iter->name);
            }
            ++v_iter;
        }
        ++iter;
    }

    //Check error
    if(int(courses_of_theme.size()) == EMPTY){
        return false;
    } else{
        //Print results
        std::set<std::string>::iterator set_iter;
        set_iter = courses_of_theme.begin();
        while(set_iter != courses_of_theme.end()){
           std::cout << *set_iter << std::endl;
            ++set_iter;
        }
    }
    return true;
}


//This function checks if theme (parameter) belongs to courses map (parameter).
//If true, function prints, in alphabetical order by course name, all courses in
//that locality that can be found in that locality and in that theme, as well
//as the number of participants. If the theme is not in courses map
//returns false.
bool courses_command(std::string location, std::string theme,
                     std::map<std::string, std::vector<Course>>& courses)
{
    std::map <std::string, int> course_enrollments;

    std::vector<Course> course_vector = courses.at(location);
    std::vector<Course>::iterator v_iter;
    v_iter = course_vector.begin();

    while (v_iter != course_vector.end()) {
        //loop through all courses of one locations

        if(v_iter->theme == theme){
            //If theme matches to te parameter theme, add course name and number
            //of enrollments to course_enrollments map.
            course_enrollments.insert({v_iter->name, v_iter->enrollments});
        }
        ++v_iter;
    }

    //Check error
    if(int(course_enrollments.size()) == EMPTY){
        return false;

    } else{
        //Print results
        std::map<std::string, int>::iterator map_iter;
        map_iter = course_enrollments.begin();
        while(map_iter != course_enrollments.end()){
            //Loop through course_enrollments map (alphabetically).
            if(map_iter->second == COURSE_FULL){
                //If course is full replace COURSE_FULL with text. Print course
                //name and that course is full.
                std::cout << map_iter->first << " --- " << "full" << std::endl;

            } else{
                //Print course name and number of enrollments.
                std::cout << map_iter->first << " --- " << map_iter->second
                          << " enrollments" << std::endl;
            }
            ++map_iter;
        }
    }
    return true;
}


//This function print out of all courses in all localities those that can still
//be enrolled, ie those that are not full. The location, theme and name are
//printed from the courses. In the list to be printed, the courses are listed
//alphabetically, first by location, then by theme, and finally by the name of
//the course.
void available(std::map<std::string, std::vector<Course>>& courses)
{
    std::set<std::string> available_courses;
    std::map<std::string, std::vector<Course>>::iterator iter;
    iter = courses.begin();
    while ( iter != courses.end() ) {

        //Loop through all locations.
        std::vector<Course> course_list = iter->second;
        std::vector<Course>::iterator v_iter;
        v_iter = course_list.begin();

        while (v_iter != course_list.end()) {
            //Loop through all courses of one locations.
            //Add course data to set, if course is not full.
            //Add the necessary punctuation.
            if(v_iter->enrollments < COURSE_FULL){
                std::string course_data = "";
                course_data.append(iter->first);
                course_data.append(" : ");
                course_data.append(v_iter->theme);
                course_data.append(" : ");
                course_data.append(v_iter->name);

                available_courses.insert(course_data);
            }
            ++v_iter;
        }
        ++iter;
    }

    //Print results,
    std::set<std::string>::iterator set_iter;
    set_iter = available_courses.begin();
    while(set_iter != available_courses.end()){
       std::cout << *set_iter << std::endl;
        ++set_iter;
    }
}


//This is the main function. read_file and prosess_line functions are called
//here. Main also contains user interface. The interface prompts the user for
//commands and calls the required functions.
int main() {

    std::map<std::string, std::vector<Course>> courses;
    std::vector<std::string> lines;

    if(read_file(lines) == false){
        std::cout << "Error: the input file cannot be opened" << std::endl;
        return EXIT_FAILURE;
    }

    if(process_line(courses, lines) == false){
        std::cout << "Error: empty field" << std::endl;
        return EXIT_FAILURE;
    }

    while(true){

        std::string line;
        std::cout << "> ";
        getline(std::cin, line);
        std::vector<std::string> parts = split(line, ' ', true);

        std::string command = parts.at(0);

        if(command == "locations"){
            if(parts.size() != 1){
                std::cout << ERROR_COMMAND << command << std::endl;
                continue;
            }
            //Print locations
            locations(courses);

        } else if(command == "courses"){
            remove_quotation_marks(parts, 2);
            if(parts.size() != 3){
                std::cout << ERROR_COMMAND << command << std::endl;
                continue;
            }
            std::string location = parts.at(1);
            std::string theme = parts.at(2);

            //Check is location in courses map
            if(courses.find(location) == courses.end()){
                std::cout << ERROR_LOCATION << std::endl;
                continue;
            }
            if(courses_command(location, theme, courses) == false){
                // error unknown theme
                std::cout << ERROR_THEME << std::endl;
                continue;
            }

        } else if(command == "available"){
            if(parts.size() != 1){
                std::cout << ERROR_COMMAND << command
                          << std::endl;
                continue;
            }
            //Print available courses
            available(courses);

        } else if(command == "courses_in_theme"){
            remove_quotation_marks(parts, 1);
            if(parts.size() != 2){
                std::cout << ERROR_COMMAND << command << std::endl;
                continue;
            }
            std::string theme = parts.at(1);

            if(courses_in_theme(theme, courses) == false){
                // error unknown theme
                std::cout << ERROR_THEME << std::endl;
                continue;
            }

        } else if(command == "favorite_theme"){
            if(parts.size() != 1){
                std::cout << ERROR_COMMAND << command << std::endl;
                continue;
            }
            //Print favorite themes
            favorite_theme(courses);

        } else if(command == "quit"){
           return EXIT_SUCCESS;

        } else {
            std::cout << UNKNOWN_COMMAND << command << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
