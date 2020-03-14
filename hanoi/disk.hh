/* Disk
 * ------------
 * Author:
 * This class stores disk info
 * about its size, and pole location
 * it also contains the methods to draw the disk on
 * the game board
 *
 * */

#ifndef DISK_HH
#define DISK_HH
#include <QGraphicsItem>
#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QRectF>


class Disk : public QGraphicsItem
{
public:
    Disk(int height, int width, QColor color);

    //These 2 functions are used to create graphical disks
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*,
               QWidget* = nullptr) override;
    QRectF boundingRect() const override;

    // self explanatory get methods.
    int get_width()const;
    int get_height() const;

private:
    int height_;
    int width_;
    QColor color_;

};

#endif // DISK_HH
