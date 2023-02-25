/*
#############################################################################
# COMP.CS.110 Programming 2: Autumn 2020                                    #
# Project3: Match3                                                          #
# File: mainwindow.hh                                                       #
# Description: HH file for Match3 game                                      #
# Program author: * Name: Viivi Mustonen                                    #
#                 * Student number: H293452                                 #
#                 * UserID: tgvimu                                          #
#                 * E-Mail: viivi.mustonen@tuni.fi                          #
#                                                                           #
#############################################################################
*/


#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <vector>
#include <deque>
#include <random>
#include <QTimer>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //Contstructor for main window
    MainWindow(QWidget *parent = nullptr);

    //Destructor for main window
    ~MainWindow();

private slots:

    //Slot for handling push of move button
    void on_movePushButton_clicked();

    //Slot for handling push of new game button
    void on_newgamePushButton_clicked();

private:
    Ui::MainWindow *ui;

    // Scene for the game grid
    QGraphicsScene* scene_;

    // Margins for the drawing area (the graphicsView object)
    // You can change the values as you wish
    const int TOP_MARGIN = 50;
    const int LEFT_MARGIN = 100;

    // Size of a square containing a fruit
    const int SQUARE_SIDE = 50; // give your own value here
    // Number of vertical cells (places for fruits)
    const int ROWS = 10; // give your own value here
    // Number of horizontal cells (places for fruits)
    const int COLUMNS = 8; // give your own value here

    // Constants describing scene coordinates
    const int BORDER_UP = 0;
    const int BORDER_DOWN = SQUARE_SIDE * ROWS;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = SQUARE_SIDE * COLUMNS;

    //TIme interval for delay between operations
    const int TIMER_INTERVAL = 800;

    //Const values for changing input char to index of vector
    const int ASCII_A = 65;
    const int ASCII_Z_A = 25;

    //Const value for removed fruit
    const int FRUIT_DELETED = -1;

    //Number of points user earns from each removed fruit
    const unsigned int POINTS_PER_FRUIT = 10;

    //Game time timer
    QTimer game_time_;

    //variables for store game time
    int minutes_ = 0;
    int seconds_ = 0;

    //Timer for delay between operations
    QTimer timer_;

    // Constants for different fruits and the number of them
     enum Fruit_kind {PLUM,
                     STRAWBERRY,
                     APPLE,
                     LEMON,
                     BLUEBERRY,
                     ORANGE,
                     NUMBER_OF_FRUITS};

    //Game mode indicates what happens after timer_ timetout. Here are options
    //for game mode
    enum Game_mode {switch_fruit, remove_match, drop};

    //Struct to hold fruit data
    struct Fruit
    {
        int x;
        int y;
        int value;
        int steps_to_drop = 0;
        QGraphicsRectItem* rect;
    };

    //Game board is saved to this variable
    std::vector<std::vector<Fruit>> board_;

    //Points earned by user
    unsigned int points_ = 0;

    //Variables for flipping two fruits
    Fruit first_;
    Fruit second_;

    //Game mode indicates what happens after timer_ timetout.
    Game_mode timer_mode_ = switch_fruit;

    //This variable is used to recheck matches after fruits are dropped.
    //With this variable, game can handle removing match that occurs after
    //dropping fruits of privous match.
    bool extra_drop = false;

    //For randomly selecting fruits for the grid
    std::default_random_engine random_eng_;
    std::uniform_int_distribution<int> distr_;

    //Matches are stored to this variable
    std::vector<Fruit> matches_;

    //Fruits need to be dropped are saved to this variable
    std::vector<Fruit> fruits_to_drop_;

    // Writes the titles for the grid rows and columns
    void init_titles();

    // Draws a single fruit near the right bottom corner of the grid
    // At the moment, this function is not called in the template,
    // but try to do so, if you want to use real fruits instead of rectangles.
    void draw_fruit();

    //Initializes the boards with fruits. In case there is match 3 situation
    //on board, init is repeated until match 3 is avoided and game can start.
    void init_board();

    //Clears previous matches and then finds matches vertically
    //and horizontally.
    void find_match();

    //This function finds matches vertically
    void find_match_vertical();

    //This function finds matches horizontally
    void find_match_horizontal();

    //Checks are param x and param y on board
    //param x, x-coordinate
    //param y, y-coordinate
    //Returns True, if coordinates are on board. Otherwise displays error
    //message and returns false.
    bool validate_coordinates(int& x, int& y);

    //Finds out coordinates of second fruit to be moved.
    //param x1, x-cordinate of first fruit
    //param y1, y-cordinate of first fruit
    //param x2, x-cordinate of second fruit filled by this function
    //param y2, y-cordinate of second fruit filled by this function
    //returns true if coordinates x2 and y2 are on board.
    //Otherwice displays error message and returns false
    bool get_destination_coordinates (int& x1, int& y1, int& x2, int& y2);

    //Validates that input entered by user can be tranfered to coordinates of
    //first fruit to be moved.
    //param x, x-coordinate filled by this function
    //param y, y-coordinate filled by this function
    //returns true, if inputs are valid. Otherwice displays error message and
    //returns false.
    bool validate_input(int& x, int& y);

    //this functions handles moving fruits on board. Function is responsible for
    //switching tw fruits, removing match 3 fruits and dropping fruits
    //down after remove
    void perform_move();

    //Switches positions of two fruits
    void switch_fruits();

    //Removes fruits that create Match 3 situation
    void remove_matches();

    //Drops fruits down after Match 3 fruits are removed from board
    void drop_fruits();

    //Sets user interface cnotrals back to enabled after the move
    void enable_next_move();

    //Checks is fruit already added to matches_ variable to avoid
    //dublicate entries in matches_
    //param f, Fruit for getting coordinates to be checked from matches_
    //returns true if f was not found from matches_
    bool match_not_recorded(Fruit f);

    // Function handling timeout for game time timer.
    void gametime_timeout();
};
#endif // MAINWINDOW_HH
