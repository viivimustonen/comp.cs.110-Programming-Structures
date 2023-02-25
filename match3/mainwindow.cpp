/*
#############################################################################
# COMP.CS.110 Programming 2: Autumn 2020                                    #
# Project3: Match3                                                          #
# File: mainwindow.cpp                                                      #
# Description: CPP file for Match3 game                                     #
# Program author: * Name: Viivi Mustonen                                    #
#                 * Student number: H293452                                 #
#                 * UserID: tgvimu                                          #
#                 * E-Mail: viivi.mustonen@tuni.fi                          #
#                                                                           #
#############################################################################
*/


#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QDebug>
#include <QPixmap>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles
    scene_ = new QGraphicsScene(this);

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a fruit is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    int seed = time(0); // You can change seed value for testing purposes
    random_eng_.seed(seed);
    distr_ = std::uniform_int_distribution<int>(0, NUMBER_OF_FRUITS - 1);

    // Wiping out the first random number (which is almost always 0)
    distr_(random_eng_);

    //Init board for the game
    init_titles();
    init_board();

    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::perform_move);

    //Timer is stopped until it's time to move fruits on the board
    timer_.stop();

    //Init game time timer and start the game time
    game_time_.setSingleShot(false);
    connect(&game_time_, &QTimer::timeout, this, &MainWindow::gametime_timeout);
    game_time_.start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_titles()
{
    // Displaying column titles, starting from A
    for(int i = 0, letter = 'A'; i < COLUMNS; ++i, ++letter)
    {
        int shift = 5;
        QString letter_string = "";
        letter_string.append(letter);
        QLabel* label = new QLabel(letter_string, this);
        label->setGeometry(LEFT_MARGIN + shift + i * SQUARE_SIDE,
                           TOP_MARGIN - SQUARE_SIDE,
                           SQUARE_SIDE, SQUARE_SIDE);
    }

    // Displaying row titles, starting from A
    for(int i = 0, letter = 'A'; i < ROWS; ++i, ++letter)
    {
        QString letter_string = "";
        letter_string.append(letter);
        QLabel* label = new QLabel(letter_string, this);
        label->setGeometry(LEFT_MARGIN - SQUARE_SIDE,
                           TOP_MARGIN + i * SQUARE_SIDE,
                           SQUARE_SIDE, SQUARE_SIDE);
    }
}

void MainWindow::draw_fruit()
{
    // Vector of fruits
    const std::vector<std::string>
            fruits = {"cherries", "strawberry", "orange", "pear", "apple",
                      "bananas", "tomato", "grapes", "eggplant"};

    // Defining where the images can be found and what kind of images they are
    const std::string PREFIX(":/");
    const std::string SUFFIX(".png");

    // Converting image (png) to a pixmap
    int i = 0; // try different values in 0 <= i < fruits.size()
    std::string filename = PREFIX + fruits.at(i) + SUFFIX;
    QPixmap image(QString::fromStdString(filename));

    // Scaling the pixmap
    image = image.scaled(SQUARE_SIDE, SQUARE_SIDE);

    // Setting the pixmap for a new label
    QLabel* label = new QLabel("test", this);
    label->setGeometry(LEFT_MARGIN + COLUMNS * SQUARE_SIDE,
                       TOP_MARGIN + ROWS * SQUARE_SIDE,
                       SQUARE_SIDE, SQUARE_SIDE);
    label->setPixmap(image);
}

void MainWindow::init_board()
{
    //empty board first. This is needed for reiniting the board.
    board_.clear();

    for(int i = 0; i < COLUMNS; ++i)
    {
        std::vector<Fruit> fruit_vector;
        board_.push_back(fruit_vector);

        for(int j = 0; j < ROWS; ++j)
        {

            // Defining the color and outline of the square
            QBrush brush(Qt::black);
            QPen blackPen(Qt::black);
            blackPen.setWidth(2);

            //Create fruit value with random engine
            int value = distr_(random_eng_);

            //Set fruit color to brush
            if(value == PLUM){
                brush.setColor(Qt::magenta);
            }else if(value == STRAWBERRY){
                brush.setColor(Qt::red);

            }else if(value == APPLE){
                brush.setColor(Qt::green);

            }else if(value == LEMON){
                brush.setColor(Qt::yellow);

            }else if(value == BLUEBERRY){
                brush.setColor(Qt::blue);

            }else if(value == ORANGE){
                brush.setColor(Qt::cyan);
            }

            int x = i * SQUARE_SIDE;
            int y = j * SQUARE_SIDE;

            Fruit f;
            f.value = value;
            f.x = i;
            f.y = j;
            f.rect = scene_->addRect(x, y, SQUARE_SIDE, SQUARE_SIDE,
                                     blackPen, brush);
            board_.at(i).push_back(f);
        }
    }

    //re init board if matches are found after init
    find_match();
    if (matches_.size() > 0){
        scene_->clear();
        init_board();
    }

    //Init game time and start timer (after new game)
    minutes_ = 0;
    seconds_ = 0;
    game_time_.start(1000);
}

void MainWindow::find_match()
{
    //Clear matches before finding them vertically and horizontally
    matches_.clear();

    find_match_vertical();
    find_match_horizontal();
}

void MainWindow::find_match_vertical()
{
    for(int i = 0; i < int (board_.size()); ++i)
    {
        for(int j = 0; j < int (board_[i].size()); ++j)
        {
            if (j < 2){
                //match is not possible until 3rd item
                continue;
            }

            if (board_.at(i).at(j).value == board_.at(i).at(j-1).value and
                    board_.at(i).at(j).value == board_.at(i).at(j-2).value and
                    board_.at(i).at(j).value != FRUIT_DELETED){

                // Match found. Add coordinates to vector
                Fruit pos;
                pos.x = i;
                pos.y = j;
                matches_.push_back(pos);

                //check should "j-1" and "j-2" need to be added too
                Fruit pos2;
                pos2.x = i;
                pos2.y = j-1;
                if (match_not_recorded(pos2) == true) {
                    matches_.push_back(pos2);
                }

                Fruit pos3;
                pos3.x = i;
                pos3.y = j-2;
                if (match_not_recorded(pos3) == true) {
                    matches_.push_back(pos3);
                }
            }
        }
    }
}

void MainWindow::find_match_horizontal()
{
    for(int i = 0; i < int (board_.size()); ++i)
    {
        for(int j = 0; j < int (board_[i].size()); ++j)
        {
            if (i < 2){
                //match is not possible until 3rd item
                continue;
            }

            if (board_.at(i).at(j).value == board_.at(i-1).at(j).value and
                    board_.at(i).at(j).value == board_.at(i-2).at(j).value and
                    board_.at(i).at(j).value != FRUIT_DELETED){

                // Match found. Add coordinates to vector
                Fruit pos;
                pos.x = i;
                pos.y = j;
                matches_.push_back(pos);

                //check should "i-1" and "i-2" need to be added too
                Fruit pos2;
                pos2.x = i-1;
                pos2.y = j;
                if (match_not_recorded(pos2) == true) {
                    matches_.push_back(pos2);
                }

                Fruit pos3;
                pos3.x = i-2;
                pos3.y = j;
                if (match_not_recorded(pos3) == true) {
                    matches_.push_back(pos3);
                }
            }
        }
    }
}

bool MainWindow::get_destination_coordinates(int &x1, int &y1, int &x2, int &y2)
{
    //case no direction selected
    if (ui->leftRadioButton->isChecked() == false and
            ui->rightRadioButton->isChecked() == false and
            ui->upRadioButton->isChecked() == false and
            ui->downRadioButton->isChecked() == false){
        ui->infoTextBrowser->setText("Valitse siirtosuunta");
        return false;
    }

    if (ui->leftRadioButton->isChecked()){
        if (x1 == 0){
            //moving left from left most column is not possible
            ui->infoTextBrowser->setText("Siirto ei ole mahdollinen: vasemman "
                                       "puoleisin sarake ja siirto vasemmalle");
            return false;
        }
        x2 = x1-1;
        y2 = y1;

    } else if (ui->rightRadioButton->isChecked()){
        if (x1 == COLUMNS-1){
            //moving right from right most column is not possible
            ui->infoTextBrowser->setText("Siirto ei ole mahdollinen: oikean "
                                         "puoleisin sarake ja siirto oikealle");
            return false;
        }
        x2 = x1+1;
        y2 = y1;

    } else if(ui->upRadioButton->isChecked()){
        if( y1 == 0){
            //moving up from top row is not possible
            ui->infoTextBrowser->setText("Siirto ei ole mahdollinen: ylin rivi "
                                         "ja siirto ylös");
            return false;
        }
        x2 = x1;
        y2 = y1-1;

    } else if(ui->downRadioButton->isChecked()) {
        if(y1 == ROWS-1){
            //moving down from bottom row is not possible
            ui->infoTextBrowser->setText("Siirto ei ole mahdollinen: alin rivi "
                                         "ja siirto alas");
            return false;
        }
        x2 = x1;
        y2 = y1+1;
    }

    return true;
}

bool MainWindow::validate_input(int &x, int &y)
{
    // get coordinates of first fruit
    QString x_input = ui->xLineEdit->text();
    QString y_input = ui->yLineEdit->text();

    if (x_input == "" or y_input == ""){
        ui->infoTextBrowser->setText("Anna koordinaatit tehdäksesi siirron.");
        return false;
    }

    if (x_input.length() > 1 or y_input.length() > 1) {
        ui->infoTextBrowser->setText("Anna koordinaatti yhtenä kirjaimena.");
        return false;
    }

    //convert to upper case
    x_input = x_input.toUpper();
    std::string str = x_input.toStdString();
    char x_in = str[0];

    //Calculate index of vector from ASCII value
    x = x_in - ASCII_A;

    //valid X ASCII values are between 65 - 90 (A-Z, or max number of columns)
    if (x < 0 or x > COLUMNS or x > ASCII_Z_A){
        ui->infoTextBrowser->setText("X koordinaatti oli laiton");
        return false;
    }

    y_input = y_input.toUpper();
    std::string str_y = y_input.toStdString();
    char y_in = str_y[0];
    y = y_in - ASCII_A;

    //valid X ASCII values are between 65 - 90 (A-Z, or max number of rows)
    if (y < 0 or y > ROWS or y > ASCII_Z_A){
        ui->infoTextBrowser->setText("Y koordinaatti oli laiton");
        return false;
    }

    return true;
}

void MainWindow::perform_move()
{
    find_match();

    if (timer_mode_ == switch_fruit){
        switch_fruits();
        timer_mode_ = remove_match;

    }else if (timer_mode_ == remove_match){
        remove_matches();
        timer_mode_ = drop;

    }else if (timer_mode_ == drop){
        drop_fruits();

        if (extra_drop == false){
           timer_mode_ = remove_match;
           extra_drop = true;
           return;
        }

        //move is over, set game for next move
        timer_mode_ = switch_fruit;
        enable_next_move();
        timer_.stop();
    }
}

void MainWindow::switch_fruits()
{
    // Calculating the displacement that can be either
    // positive or negative, depending on the direction
    qreal delta_x, delta_y;
    if(second_.x - first_.x > 0) {
        delta_x = static_cast<qreal>(SQUARE_SIDE);  // right (positive step)
    } else if (second_.x - first_.x == 0){
        delta_x  = static_cast<qreal>(0);
    } else {
        delta_x = static_cast<qreal>(-1*SQUARE_SIDE); // left (negative step)
    }

    if(second_.y - first_.y > 0) {
        delta_y = static_cast<qreal>(SQUARE_SIDE);  // down (positive step)
    } else if (second_.y - first_.y == 0){
        delta_y  = static_cast<qreal>(0);
    } else {
        delta_y = static_cast<qreal>(-1*SQUARE_SIDE); // up (negative step)
    }

    //move fruits on screen
    first_.rect->moveBy(delta_x, delta_y);
    second_.rect->moveBy((-1*delta_x), (-1*delta_y));
}

void MainWindow::remove_matches()
{
    for (auto iter : matches_){
        board_.at(iter.x).at(iter.y).value = FRUIT_DELETED;

        scene_->removeItem(board_.at(iter.x).at(iter.y).rect);
        delete board_.at(iter.x).at(iter.y).rect;
        board_.at(iter.x).at(iter.y).rect = nullptr;

        //since there was fuits to delete, one more round is needed to check
        //is there need to delete fruits after drop.
        extra_drop = false;
    }

    //update points for player
    points_ += matches_.size() * POINTS_PER_FRUIT;

    //update point on user interface
    QString txt = QString::number(points_);
    ui->pointsLabel->setText(txt);

    matches_.clear();
}

void MainWindow::drop_fruits()
{
    fruits_to_drop_.clear();

    //Mark all items on board, how mane steps htey need to be dropped
    for(int i = 0; i < int (board_.size()); ++i)
    {
        for(int j = 0; j < int (board_[i].size()); ++j)
        {
            if (board_.at(i).at(j).value == FRUIT_DELETED){
                //removed fruit found. Mark all fruits above for drop
                for (int iter = j-1; iter >= 0; --iter ){
                    if (board_.at(i).at(iter).value != FRUIT_DELETED){
                        //mark only not deleted fruits
                        board_.at(i).at(iter).steps_to_drop += 1;
                    }
                }
            }
        }
    }

    //Collect items to be dropped into vector
    for(int i = 0; i < int (board_.size()); ++i)
    {
        for(int j = 0; j < int (board_[i].size()); ++j)
        {
            if (board_.at(i).at(j).steps_to_drop > 0){
                Fruit pos;
                pos.x = i;
                pos.y = j;
                pos.steps_to_drop = board_.at(i).at(j).steps_to_drop;
                fruits_to_drop_.push_back(pos);
            }
        }
    }

    //Process drops from last to first
    for (auto it = fruits_to_drop_.rbegin(); it != fruits_to_drop_.rend(); ++it)
    {
        int x = it->x;
        int y = it->y;
        Fruit from = board_.at(x).at(y);
        Fruit to = board_.at(x).at(y+from.steps_to_drop);
        board_.at(x).at(y+from.steps_to_drop) = board_.at(x).at(y);
        board_.at(x).at(y) = to;


        from.rect->moveBy(0, it->steps_to_drop*SQUARE_SIDE);

        board_.at(x).at(y).x = x;
        board_.at(x).at(y).y = y;
        board_.at(x).at(y+from.steps_to_drop).x = x;
        board_.at(x).at(y+from.steps_to_drop).y = y+from.steps_to_drop;

        board_.at(x).at(y).steps_to_drop = 0;
        board_.at(x).at(y+from.steps_to_drop).steps_to_drop = 0;
    }
}

void MainWindow::enable_next_move()
{
    //Enable user interface for next move
    ui->xLineEdit->setText("");
    ui->yLineEdit->setText("");
    ui->movePushButton->setEnabled(true);
    ui->groupBox->setEnabled(true);
    ui->xLineEdit->setEnabled(true);
    ui->yLineEdit->setEnabled(true);
    ui->newgamePushButton->setEnabled(true);

    //Game time continues
    game_time_.start(1000);

}

bool MainWindow::match_not_recorded(MainWindow::Fruit f)
{
    for (auto iter : matches_){
        if (f.x == iter.x and f.y == iter.y){
            //Fruit was found in matches vector
            return false;
        }
    }

    //Fruit was not in matches vector
    return true;
}

void MainWindow::gametime_timeout()
{
    if (seconds_ == 59){
        seconds_ = 0;
        minutes_ += 1;
    } else {
        seconds_ += 1;
    }
    ui->lcdNumberMin->display(minutes_);
    ui->lcdNumberSec->display(seconds_);
}

bool MainWindow::validate_coordinates(int& x, int& y)
{
    if ( x < 0 or x > COLUMNS - 1 or y < 0 or y > ROWS - 1){
        ui->infoTextBrowser->setText("Siirto ei ole mahdollinen: "
                                     "koordinaatit ovat laudan ulkopuolella.");
        return false;
    }

    return true;
}

void MainWindow::on_movePushButton_clicked()
{
    //When move is ongoing, gametime is stopped
    game_time_.stop();

    //disable UI controls when move is processed
    ui->infoTextBrowser->setText("");
    ui->movePushButton->setDisabled(true);
    ui->groupBox->setDisabled(true);
    ui->xLineEdit->setDisabled(true);
    ui->yLineEdit->setDisabled(true);
    ui->newgamePushButton->setDisabled(true);

    //validate user input
    int x1;
    int y1;
    if (validate_input(x1, y1) == false){
        enable_next_move();
        return;
    }

    //make sure x1 and y1 are on board
    if (validate_coordinates (x1, y1) == false) {
        enable_next_move();
        return;
    }

    //validate second coordinates
    int x2;
    int y2;
    if (get_destination_coordinates(x1, y1, x2, y2) == false ){
        enable_next_move();
        return;
    }

    if (validate_coordinates (x2, y2) == false) {
        enable_next_move();
        return;
    }

    first_ = board_.at(x1).at(y1);
    second_ = board_.at(x2).at(y2);

    if (first_.value == FRUIT_DELETED or second_.value == FRUIT_DELETED){
        ui->infoTextBrowser->setText("Siirto ei ole mahdollinen tyhjän "
                                     "ruudun kanssa");
        enable_next_move();
        return;
    }

    //move fruits on board
    board_.at(x1).at(y1) = second_;
    board_.at(x1).at(y1).x = x1;
    board_.at(x1).at(y1).y = y1;

    board_.at(x2).at(y2) = first_;
    board_.at(x2).at(y2).x = x2;
    board_.at(x2).at(y2).y = y2;

    //make sure Match 3 is at board after the move
    find_match();
    if (matches_.size() == 0){

        ui->infoTextBrowser->setText("Siirto ei ole mahdollinen, koska se ei "
                                     "muodosta vähintään kolmea peräkkäistä "
                                     "hedelmää.");
        //There was no match 3, cancel move
        board_.at(x1).at(y1) = first_;
        board_.at(x1).at(y1).x = x1;
        board_.at(x1).at(y1).y = y1;

        board_.at(x2).at(y2) = second_;
        board_.at(x2).at(y2).x = x2;
        board_.at(x2).at(y2).y = y2;

        enable_next_move();
        return;
    }

    //Set status text to user interface
    ui->infoTextBrowser->setText("Siirto on OK.");

    timer_.start(TIMER_INTERVAL);
}

void MainWindow::on_newgamePushButton_clicked()
{
    //user wants to start new game

    points_ = 0;
    QString txt = QString::number(points_);
    ui->pointsLabel->setText(txt);

    matches_.clear();
    fruits_to_drop_.clear();
    init_board();
}
