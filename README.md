# comp.cs.110-Programming-Structures
comp.cs.110 Programming structures -course projects.
The repository contains four practice projects.

## Path
This program generates a Path puzzle. The board is ROWS x COLUMNS (5x4), and every round the player chooses a source point and a target point
(totally four numbers), making the given button to be moved to the target position (if possible). The game checks if the user-given move can be made.
At start all green buttons are on the top row, and all red buttons are on the bottom row. Game will end when all green buttons have been moved to the 
bottom row, and all red buttons to the top row. It is not allowed to move a button over another button. When printing the game board, green button 
is represented by 'G', red button by 'R', empty hole by 'o', and unused position by empty space.

## Education center
This program generates an education center. First, program asks user for an input file. Input file includes information about courses in different
locations. Information includes location, course theme, course name and enrollments for course. Now user can search information about different
courses. User can search all available locations. Or all courses by location and theme (even if they are full). Or all available courses (those that are not
full). The courses are listed alphabetically, first by location, then by theme, and finally by the name of the course. Or user can search courses by
theme from all locations. Or favorite theme from all locations.

## Company
Implements the module Company into the program that first reads company information from the input file, and the permits the user to make searches concerning the relationships in the company.

## Match 3
The purpose of the game is to remove as many fruits as possible from the board. The user can move two adjacent fruits (colored squares).
The transfer is performed by entering the coordinates of the first fruit to be transferred and by selecting the transfer direction from 
the radio button element. The game checks if the transfer is possible. The transfer is impossible if there is no legal coordinate (column and row letter) or direction of movement is out off the board. In addition, "the move" must produce at least three consecutive fruits.
