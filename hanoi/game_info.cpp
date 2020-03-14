#include "game_info.hh"

Game_info::Game_info(int amount_of_disks)
{
    amount_of_disks_ = amount_of_disks;
    calc_disk_height();
    calc_decrease_width();
    current_disk_width_= 120;
    min_moves_to_win_ = pow(2, amount_of_disks_) -1;

}

int Game_info::get_height() const
{
    return disk_height_;
}

int Game_info::get_current_disk_width() const
{
    return current_disk_width_;
}

int Game_info::get_min_moves() const
{
    return min_moves_to_win_;
}

void Game_info::calc_disk_height()
{
    disk_height_ = 180/amount_of_disks_;
}

void Game_info::calc_decrease_width()
{
    decrease_width_ = 100/amount_of_disks_;
}

void Game_info::change_current_disk_width()
{
    current_disk_width_ = current_disk_width_ -decrease_width_;
}
