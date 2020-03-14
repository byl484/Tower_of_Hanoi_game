#include "disk.hh"


Disk::Disk(int height, int width, QColor color){
    height_ = height;
    width_ = width;
    color_ = color;
}

void Disk::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem*, QWidget*)
{
    QRectF rect = boundingRect();
    QBrush brush(color_);
    painter->fillRect(rect, brush);
    painter->drawRect(rect);
}

QRectF Disk::boundingRect() const
{
    return QRectF(0,0,width_,height_);
}

int Disk::get_width() const
{
    return width_;
}

int Disk::get_height() const
{
    return height_;
}
