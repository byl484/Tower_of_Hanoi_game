
/* Game_info
 * ------------
 *
 * This class calculates and constant information
 * during the game
 * the values are calculated based on how many
 * disks there are, and some are being changed when
 * setuping the game
 * but they will be constant
 * for the duration of the game
 * */
#ifndef GAME_HH
#define GAME_HH

#include "disk.hh"
#include <math.h>

class Game_info
{
public:
    Game_info(int amount_of_disks);

    //Height is disk height
    int get_height() const;

    //get the width of the next disk when creating them
    //for the game
    int get_current_disk_width() const;

    //get minimum amount of moves to win.
    int get_min_moves() const;

    //calc how tall a disk should be
    //less disks -> taller disk
    void calc_disk_height();

    //current disk width changes based on
    //How many disks there are in total and
    //How many disks there is placed before
    //during the game setup.
    void calc_decrease_width();
    void change_current_disk_width();

private:
    int amount_of_disks_;
    int disk_height_;
    int current_disk_width_;
    int decrease_width_;
    int min_moves_to_win_;

};
#endif // GAME_HH
