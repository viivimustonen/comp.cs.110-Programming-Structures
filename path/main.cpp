/* Path
 *
 * Desc:
 *  This program generates a Path puzzle. The board is ROWS x COLUMNS (5x4),
 * and every round the player chooses a source point and a target point
 * (totally four numbers), making the given button to be moved to the target
 * position (if possible). The game checks if the user-given move can be made.
 *  At start all green buttons are on the top row, and all red buttons are on
 * the bottom row. Game will end when all green buttons have been moved to the
 * bottom row, and all red buttons to the top row. It is not allowed to move a
 * button over another button.
 *  When printing the game board, green button is represented by 'G', red
 * button by 'R', empty hole by 'o', and unused position by empty space.
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
 * */

#include <iostream>
#include <vector>

using namespace std;

enum Slot_type {RED, GREEN, EMPTY, UNUSED};
const unsigned int ROWS = 5;
const unsigned int COLUMS = 4;
// Number of inputs must be four
const unsigned int NUMBER_OF_INPUTS = 4;
// Colum where is the crossing
const int CROSSING_X = 2;

const string INPUT_TEXT = "Enter start point (x, y) and destination point (x, y), or q to quit: ";
const string INVALID_POINT = "Invalid start/destination point.";
const string CANNOT_MOVE = "There is no path between start point and destination point.";
const string GAME_OVER = "Game over. Congratulations!";
const string MOVES_MADE = " move(s) made.";


 // This fuction prints the board
void print(const vector<vector<Slot_type>>& g)
{
    cout << "===============" << endl;
    cout << "|   | 1 2 3 4 |" << endl;
    cout << "---------------" << endl;
    for(unsigned int i = 0; i < ROWS; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < COLUMS; ++j)
        {
            switch(g.at(j).at(i))
            {
            case GREEN: cout << "G "; break;
            case RED: cout << "R "; break;
            case EMPTY: cout << "o "; break;
            case UNUSED: cout << "  "; break;
            }
        }
        cout << "|" << endl;
    }
    cout << "===============" << endl;
}

// Converts the given numeric string to the corresponding integer
// (by calling stoi).
// If the given string is not numeric, returns 0.
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Inits the board. At start all green buttons are on the top row,
// and all red buttons are on the bottom row.
void init_board(vector<vector<Slot_type>>& board)
{
    vector<Slot_type> column1;
    column1.push_back(GREEN);
    column1.push_back(UNUSED);
    column1.push_back(UNUSED);
    column1.push_back(UNUSED);
    column1.push_back(RED);
    board.push_back(column1);

    vector<Slot_type> column2;
    column2.push_back(GREEN);
    column2.push_back(EMPTY);
    column2.push_back(EMPTY);
    column2.push_back(EMPTY);
    column2.push_back(RED);
    board.push_back(column2);

    vector<Slot_type> column3;
    column3.push_back(GREEN);
    column3.push_back(UNUSED);
    column3.push_back(EMPTY);
    column3.push_back(UNUSED);
    column3.push_back(RED);
    board.push_back(column3);

    vector<Slot_type> column4;
    column4.push_back(GREEN);
    column4.push_back(UNUSED);
    column4.push_back(UNUSED);
    column4.push_back(UNUSED);
    column4.push_back(RED);
    board.push_back(column4);

}

// Checks is point (x,y) on the board. If not returns false.
bool is_point_on_board(unsigned int x, unsigned int y, vector<vector<Slot_type>>& board)
{
    if (x > COLUMS or y > ROWS)
    {
        return false;
    }

    Slot_type symbol = board.at(x-1).at(y-1);

    if(symbol == UNUSED)
    {
        return false;
    }
    return true;
}

// Checks is there a button in the point (x,y).
// Returns false if the point (x,y) is not on the board or if there is not green or red button.
bool is_button(unsigned int x, unsigned int y, vector<vector<Slot_type>>& board)
{
    if (x > COLUMS or y > ROWS)
    {
        return false;
    }

    Slot_type symbol = board.at(x-1).at(y-1);

    if(symbol != GREEN and symbol != RED)
    {
        return false;
    }
    return true;
}

// Chekcs is the point (x,y) empty.
// Returns false if the point is not on board or if point is not empty.
bool is_point_empty(unsigned int x, unsigned int y, vector<vector<Slot_type>>& board)
{
    if (x > COLUMS or y > ROWS)
    {
        return false;
    }

    Slot_type symbol = board.at(x-1).at(y-1);

    if(symbol != EMPTY)
    {
        return false;
    }
    return true;
}

// Function gets cordinates from input, turns it into vector.
// Checks that vector contains four items and that cordinates are integers.
// Returns true if getting cordinates was successful.
bool get_cordinates(string& input, unsigned int& start_x, unsigned int& start_y, unsigned int& end_x, unsigned int& end_y)
{
    vector<string> str_cordinates;
    string piece = "";

    //Split input to vector
    for (int i = 0; i < (int)input.size(); ++i)
    {
        if (input.at(i) != ' ')
        {
            piece.push_back(input.at(i));
        }
        else
        {
            str_cordinates.push_back(piece);
            piece = "";
        }
     }

    if (piece != "")
    {
        str_cordinates.push_back(piece);
    }

    //Check that vector contains four items
    if ((int)str_cordinates.size() != NUMBER_OF_INPUTS)
    {
        return false;
    }

    //Convert string cordinates to int cordinates
    start_x =  stoi_with_check(str_cordinates.at(0));
    start_y =  stoi_with_check(str_cordinates.at(1));
    end_x =  stoi_with_check(str_cordinates.at(2));
    end_y =  stoi_with_check(str_cordinates.at(3));

    if(start_x == 0 or start_y == 0 or end_x == 0 or end_y == 0)
    {
        return false;
    }

    return true;
}

// Validates inputs. If input is not valid returns false.
bool is_input_valid(unsigned int& start_x, unsigned int& start_y, unsigned int& end_x,
                    unsigned int& end_y, vector<vector<Slot_type>>& board)
{
    //Start point must be on board
    if (is_point_on_board(start_x, start_y, board) == false)
    {
        return false;
    }

    //End point must be on board
    if (is_point_on_board(end_x, end_y, board) == false)
    {
        return false;
    }

    //Start point must contain button
    if (is_button(start_x, start_y, board) == false)
    {
        return false;
    }

    // End point must be empty
    if (is_point_empty(end_x, end_y, board) == false)
    {
        return false;
    }

    return true;
}


// Check if the game ended. Game will end when all green buttons have been
// moved to the bottom row, and all red buttons to the top row.
// Returns true if game ended.
bool has_game_ended(vector<vector<Slot_type>>& board)
{
    if(board.at(0).at(0) == RED
            and board.at(1).at(0) == RED
            and board.at(2).at(0) == RED
            and board.at(3).at(0) == RED
            and board.at(0).at(4) == GREEN
            and board.at(1).at(4) == GREEN
            and board.at(2).at(4) == GREEN
            and board.at(3).at(4) == GREEN)
    {
        return true;
    }
    return false;
}

// Tries to find a horizontal path that is available.
// Checks if every step of the way is empty. If point is not empty returns false.
bool is_horizontal_path_available(unsigned int start_x, unsigned int y, unsigned int end_x, vector<vector<Slot_type>>& board)
{
    int delta_x = end_x - start_x;
    if (delta_x < 0)
    {
        //take steps left
        //change sign of delta_x for looping
        delta_x = delta_x * -1;
        for(int i = 1; i <= delta_x; ++i)
        {
            if(is_point_empty(start_x-i, y, board) == false)
            {
                return false;
            }
        }
    }
    else
    {
        //take steps right
        for(int i = 1; i <= delta_x; ++i)
        {
            if(is_point_empty(start_x+i, y, board) == false)
            {
                return false;
            }
        }
    }
    return true;
}

// Tries to find a vertical path that is available.
// Checks if every step of the way is empty. If point is not empty returns false.
bool is_vertical_path_available(unsigned int x, unsigned int start_y, unsigned int end_y, vector<vector<Slot_type>>& board)
{
    int delta_y = end_y - start_y;
    if (delta_y < 0)
    {
        //take steps up
        //change sign of delta_y for looping
        delta_y = delta_y * -1;
        for(int i = 1; i <= delta_y; ++i)
        {
            if(is_point_empty(x, start_y-i, board) == false)
            {
                return false;
            }
        }
    }
    else
    {
        //take steps down
        for(int i = 1; i <= delta_y; ++i)
        {
            if(is_point_empty(x, start_y+i, board) == false)
            {
                return false;
            }
        }
    }
    return true;
}


// Function checks is there path in the board available from point start_x, start_y to end_x, end_y.
// If path is available, return true.
// Otherwise, return false.
bool is_path_available(unsigned int& start_x, unsigned int& start_y, unsigned int& end_x,
                       unsigned int& end_y, vector<vector<Slot_type>>& board)
{
    // check is vertical movement needed
    if (end_y - start_y == 0)
    {
        //End point is on the same row
        if(is_horizontal_path_available(start_x, start_y, end_x, board) == false)
        {
            return false;
        }
    }
    else
    {
        //End point isin't on the same row
        //Move horizontally to crossing,

        if(is_horizontal_path_available(start_x, start_y, CROSSING_X, board) == false)
        {
            return false;
        }

        //move vertically to end row
        if(is_vertical_path_available(CROSSING_X, start_y, end_y, board) == false)
        {
            return false;
        }

        //move horizontally to end point (if needed)
        if(end_x - CROSSING_X != 0)
        {
            if(is_horizontal_path_available(CROSSING_X, end_y, end_x, board) == false)
            {
                return false;
            }
        }
    }

    //path was found
    return true;
}

// Function moves button from start_x, start_y to end_x, end_y.
void move(unsigned int& start_x, unsigned int& start_y, unsigned int& end_x,
          unsigned int& end_y, vector<vector<Slot_type>>& board)
{
    //Save button color to variable
    Slot_type symbol = board.at(start_x-1).at(start_y-1);

    //Set value to start point
    board.at(start_x-1).at(start_y-1) = EMPTY;

    //Set value to end point
    board.at(end_x-1).at(end_y-1) = symbol;
}


// This is the main function. Main first inits the board by calling init_board and prints it.
// Then main asks user to input cordinates or quit the game. If input is q the game stops and
// main prints how many moves. Else main validates input cordinates and tries to move button on board.
// If the input cordinates are valid and there is a path between start and end point, button
// moves and calls print function that prints the board. Finally main checks if the game ended.
int main()
{
    //create boards and set buttons to start positions
    vector<vector<Slot_type>> board;
    init_board(board);
    print(board);

    //Init move counter
    int moves = 0;

    //Ask command from user until game ends or user quits
    while(true)
    {
        string input;
        cout << INPUT_TEXT;
        getline(cin, input);

        //Check if user want's to quit
        if (input == "q")
        {
            cout << moves << MOVES_MADE << endl;
            break;
        }

        unsigned int start_x = 0;
        unsigned int start_y = 0;
        unsigned int end_x = 0;
        unsigned int end_y = 0;

        //Convert input to vector containing ints
        if(get_cordinates(input, start_x, start_y, end_x, end_y) == false)
        {
            cout << INVALID_POINT << endl;
            continue;
        }

        //Check is users input valid
        if (is_input_valid(start_x, start_y, end_x, end_y, board) == false)
        {
            cout << INVALID_POINT << endl;
            continue;
        }

        //Check is there path between start and end point
        if (is_path_available(start_x, start_y, end_x, end_y, board) == false)
        {
            cout << CANNOT_MOVE << endl;
            continue;
        }

        //Count move
        ++moves;

        //Move button on board
        move(start_x, start_y, end_x, end_y, board);

        //Print board after move
        print(board);

        //Check has game ended
        if (has_game_ended(board))
        {
            cout << GAME_OVER << endl;
            cout << moves << MOVES_MADE << endl;
            break;
        }
    }
    return EXIT_SUCCESS;
}
