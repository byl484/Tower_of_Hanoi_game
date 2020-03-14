#include "mainwindow.hh"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    // We need a graphics scene in which to draw a rectangle
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates:
    int left_margin = 10; // x coordinate
    int top_margin = 270; // y coordinate
    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui_->graphicsView->setGeometry(left_margin, top_margin,
                                   BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui_->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1,
    // because the circle is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    //Poles will be static for the duration of a game
    //We can create them, before the game starts:

    QPen pole_outline_color(Qt::black);
    pole_outline_color.setWidth(POLE_OUTLINE_WIDTH);

    //Change Qt-color, if you wish to change color of the poles
    //QBrushes are named by the location of the pole.

    QBrush left_color(Qt::red);
    QBrush mid_color(Qt::green);
    QBrush right_color(Qt::blue);

    scene_->addRect(POLE_LEFT, POLE_TOP,
                    POLE_WIDTH, POLE_BOTTOM, pole_outline_color, left_color);
    scene_->addRect(POLE_MID, POLE_TOP,
                    POLE_WIDTH, POLE_BOTTOM, pole_outline_color, mid_color);
    scene_->addRect(POLE_RIGHT, POLE_TOP,
                    POLE_WIDTH, POLE_BOTTOM, pole_outline_color, right_color);

    button_vec_.push_back(ui_->push_button_a_b);
    button_vec_.push_back(ui_->push_button_a_c);
    button_vec_.push_back(ui_->push_button_b_a);
    button_vec_.push_back(ui_->push_button_b_c);
    button_vec_.push_back(ui_->push_button_c_a);
    button_vec_.push_back(ui_->push_button_c_b);

    pole_vec_.push_back(&pole_left_);
    pole_vec_.push_back(&pole_mid_);
    pole_vec_.push_back(&pole_right_);

    //initialize game:
    setup_game();

}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::create_disk(Disk *disk, int x, int y){
    move_disk(disk,x,y);
    scene_->addItem(disk);
}

void MainWindow::move_disk(Disk *disk, int x, int y)
{        disk->setPos(x,y);

}

void MainWindow::move_from_to_pole(std::vector<Disk*> *from,
                                   std::vector<Disk*> *to, int pole_coord)
{
    //update vectors
    to->push_back(from->back());
    from->pop_back();

    int amount_disks_in__next_pole =to->size();
    //Variables:
    int new_y_coord = (BORDER_DOWN - to->back()->get_height()
                       -1-(amount_disks_in__next_pole-1)
                       *to->back()->get_height());
    int new_x_coord = pole_coord -
            to->back()->get_width()/2 + POLE_WIDTH/2;

    //update GUI
    move_disk(to->back(), new_x_coord, new_y_coord);

    //update move counters:
    ++moves_done_;
    --moves_left_;
    ui_->moves_left_num->setText(QString::number(moves_left_));
    ui_->move_counter_num->setText(QString::number(moves_done_));

    //Disable illegal move buttons, enable legal,
    //check winning condition after every move.
    set_button_states();
    check_win_cond();
}

void MainWindow::set_button_states()
{   //this piece of code checks for empty poles
    //and sets right buttons to disabled so
    //a disk cannot be taken away from an empty pole
    for (size_t q=0; q < pole_vec_.size(); ++q){
        if (pole_vec_.at(q)->size() == 0){
            size_t z = 2*q;
            for (;z < 2*q +2; ++z){
                button_vec_.at(z)->setEnabled(false);
            }
        }
    //this piece of code enables buttons if from - pole is not empty.
        else {
            size_t z = 2*q;
            for (;z < 2*q +2; ++z){
                button_vec_.at(z)->setEnabled(true);
            }
        }
    }
    //This check disk widths so
    //wider disk cannot be placed on top of a thinner disk
    //The two for- loops compare every top disk with each other.
    //if from poles top disk is wider than to poles top disk,
    //the button is disabled.
    //The loop doesnt take comparing pole with itself into account
    //since the condition that the pole's top disk is wider than the same disk
    //obiviously never occurs

   for (size_t w = 0; w < pole_vec_.size();++w){
        for (size_t e = 0; e < pole_vec_.size(); ++e){
            //following double if to avoid errors:
            if (pole_vec_.at(w)->size() != 0 and pole_vec_.at(e)->size() != 0){
                if (pole_vec_.at(w)->back()->get_width() >
                        pole_vec_.at(e)->back()->get_width()){
                    // the number which is formed with w as first digit and e
                    //as second digit is a ternary number
                    //(number with 3 as base number)
                    //opposed to decimal where 10 is the base number.
                    //Following number is
                    //the ternary number converted to decimal
                    //and substracted by one since
                    //the vector indexes start at 0 instead of 1.
                    //The button_vec also "skips"
                    //number 4, (Moving the disk from mid pole to mid pole
                    //- button doesnt exist)
                    //So when the number is 4 or higher,
                    //1 is substracted from it to get right button_vec index
                    size_t ternary_to_decimal = 3*w +e -1;
                    if (ternary_to_decimal >= 4){
                        button_vec_.at(ternary_to_decimal -1)->setEnabled(false);
                    }
                    else{
                        button_vec_.at(ternary_to_decimal)->setEnabled(false);
                    }
                }
            }
        }
    }
}

void MainWindow::check_win_cond()
{   size_t game_win_pole_size = amount_of_disks_;
    if (pole_mid_.size() == game_win_pole_size or
            pole_right_.size() == game_win_pole_size){
        for (size_t i=0; i < button_vec_.size(); ++i){
            button_vec_.at(i)->setEnabled(false);
        }
        QMessageBox::about(this, "You Win!", "Game Over");
    }
}

void MainWindow::setup_game()
{
    Game_info current_game = Game_info(amount_of_disks_);
    moves_done_ = 0;

    //setup counters.
    moves_left_ = current_game.get_min_moves();
    ui_->move_counter_num->setText(QString::number(moves_done_));
    ui_->moves_left_num->setText(QString::number(moves_left_));
    ui_->min_amount_num->setText(QString::number(moves_left_));

    //Variables for better clarity:
    int running_numb = 0;
    int amount_left = amount_of_disks_;
    int disk_height = current_game.get_height();


    for (;amount_left>0; --amount_left){

        //These 2 are just variables for clarity:
        int disk_y_coord = (BORDER_DOWN-current_game.get_height())
                -1-running_numb * current_game.get_height();
        int width_of_the_current_disk = current_game.get_current_disk_width();

        disk = new Disk(disk_height,
                        width_of_the_current_disk, Qt::red);
        pole_left_.push_back(disk);
        create_disk(disk, POLE_LEFT -
                    disk->get_width() / 2 + POLE_WIDTH / 2, disk_y_coord);
        current_game.change_current_disk_width();
        ++running_numb;
    }
    set_button_states();
}

void MainWindow::reset_game()
{
    //first delete old game, then call for new game setup
    for (size_t i=0; i <pole_vec_.size(); ++i){
        for (size_t u=0; u < pole_vec_.at(i)->size(); ++u){
            delete pole_vec_.at(i)->at(u);
            pole_vec_.at(i)->at(u) = nullptr;
        }
        pole_vec_.at(i)->clear();
    }
    setup_game();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_1 and button_vec_.at(0)->isEnabled()){
        move_from_to_pole(&pole_left_, &pole_mid_, POLE_MID);
    }
    else if (event->key() == Qt::Key_2 and button_vec_.at(1)->isEnabled()){
        move_from_to_pole(&pole_left_, &pole_right_, POLE_RIGHT);
    }
    else if (event->key() == Qt::Key_3 and button_vec_.at(2)->isEnabled()){
        move_from_to_pole(&pole_mid_, &pole_left_, POLE_LEFT);
    }
    else if (event->key() == Qt::Key_4 and button_vec_.at(3)->isEnabled()){
        move_from_to_pole(&pole_mid_, &pole_right_, POLE_RIGHT);
    }
    else if (event->key() == Qt::Key_5 and button_vec_.at(4)->isEnabled()){
        move_from_to_pole(&pole_right_, &pole_left_, POLE_LEFT);
    }
    else if (event->key() == Qt::Key_6 and button_vec_.at(5)->isEnabled()){
        move_from_to_pole(&pole_right_, &pole_mid_, POLE_MID);
    }
}

void MainWindow::on_push_button_a_b_clicked()
{
    move_from_to_pole(&pole_left_, &pole_mid_, POLE_MID);
}

void MainWindow::on_push_button_a_c_clicked()
{
    move_from_to_pole(&pole_left_, &pole_right_, POLE_RIGHT);
}

void MainWindow::on_push_button_b_a_clicked()
{
    move_from_to_pole(&pole_mid_, &pole_left_, POLE_LEFT);
}

void MainWindow::on_push_button_b_c_clicked()
{
    move_from_to_pole(&pole_mid_, &pole_right_, POLE_RIGHT);
}

void MainWindow::on_push_button_c_a_clicked()
{
    move_from_to_pole(&pole_right_, &pole_left_, POLE_LEFT);
}

void MainWindow::on_push_button_c_b_clicked()
{
    move_from_to_pole(&pole_right_, &pole_mid_, POLE_MID);
}

void MainWindow::on_reset_button_clicked()
{
    reset_game();
}

void MainWindow::on_set_disks_button_clicked()
{   if (amount_of_disks_ == ui_->spin_box_disks->value()){
        return;
    }
    amount_of_disks_ = ui_->spin_box_disks->value();
    reset_game();
}
