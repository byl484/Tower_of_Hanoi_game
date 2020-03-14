/* Mainwindow
 * ------------
 *
 * This class' contructor setups the mainwindow and game board
 * The game functionality is also defined in this class
 * the gui engine is also defined in this class
 * This class takes care of storing and deleting game
 * disks from vectors they are saved in
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <disk.hh>
#include <game_info.hh>

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPushButton>
#include <QMessageBox>
#include <QKeyEvent>
#include <string>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //creates disk graphic
    //takes disk-object, and disk coordinates as
    //param
    void create_disk(Disk *disk, int x, int y);

    //moves disk on the game board
    //specific coordinates that are given as
    //params. Also the disk to be moved if given as
    //param.
    //this is also used
    //when creating disk.
    void move_disk(Disk *disk, int x, int y);

    //Moves disk to specific pole,
    //takes from pole and to pole as param, also takes to poles
    //coordinates as param.
    //This method also calls move_disk method
    void move_from_to_pole(std::vector<Disk*> *from,
                           std::vector<Disk*> *to, int pole_coord);

    //This method disables illegal-move buttons and enables legal buttons
    void set_button_states();

    void check_win_cond();

    //This method setups the game
    void setup_game();

    //This method deletes previous game and setups new game
    void reset_game();

    //Buttons can also be pressed with number keys
    //Takes the pressed key as param
    void keyPressEvent(QKeyEvent *event);


private slots:

    //self explanatory button clicks
    void on_push_button_a_b_clicked();
    void on_push_button_a_c_clicked();
    void on_push_button_b_a_clicked();
    void on_push_button_b_c_clicked();
    void on_push_button_c_a_clicked();
    void on_push_button_c_b_clicked();

    //calls reset_game-method
    void on_reset_button_clicked();

    //calls reset_game-method with new diks amount from spin box
    //doesnt do anything if tries to set game with current
    //amount of disk to avoid user from resetting by accident
    void on_set_disks_button_clicked();

private:
    Ui::MainWindow *ui_;

    QGraphicsScene *scene_;

    Game_info *current_game;
    Disk *disk;
    //Every pole is a vector, which stores disk pointers
    //Every pole vectors pointer is also stored in vector
    std::vector<std::vector<Disk*>*> pole_vec_;
    std::vector<Disk*> pole_left_;
    std::vector<Disk*> pole_mid_;
    std::vector<Disk*> pole_right_;

    //Game-move button's pointers are stored

    std::vector<QPushButton*> button_vec_;

    int amount_of_disks_ = 6;
    int moves_done_ =0;
    int moves_left_;

    //Scene dimensions
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 280;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 680;

    //Pole locations on x-axis:
    const int POLE_LEFT =160;
    const int POLE_MID = 340;
    const int POLE_RIGHT = 520;

    //Pole dimensions:
    //Increase Widths if you want thicker poles or
    //thicker outlines and vice versa.
    //DECREASE TOP, if you want to make poles taller
    //Dont change BOTTOM, unless you want
    //the poles to float in the air.
    const int POLE_WIDTH = 8;
    const int POLE_OUTLINE_WIDTH = 1;
    const int POLE_TOP = 50;
    const int POLE_BOTTOM = 260;


};
#endif // MAINWINDOW_HH
