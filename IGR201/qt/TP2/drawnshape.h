#ifndef DRAWNSHAPE_H
#define DRAWNSHAPE_H

#include <QPainterPath>
#include <QPen>
#include "mode.h"

class DrawnShape
{
public:
    DrawnShape (QRectF *rect, QPen *p, Mode m)
    {
        painterPath = new QPainterPath();
        if (m == rectangle)
        {
            painterPath->addRect(*rect);
            mode = rectangle;
        }
        if (m == ellipse)
        {
            painterPath->addEllipse(*rect);
            mode = ellipse;
        }
        pen = *p;
    }

    DrawnShape(QLineF *l, QPen *p)
    {
        painterPath = new QPainterPath();
        painterPath->moveTo(l->p1());
        painterPath->lineTo(l->p2());
        pen = *p;
    }

    DrawnShape(QPainterPath *path, QPen *p)
    {
        painterPath = path;
        pen = *p;
    }

    QPen getPen() const {return pen;}
    void setPen(QPen p) {pen = p;}
    QPainterPath * getPath() const {return painterPath;}
    void setPath(QPainterPath *path) {*painterPath = *path;}


private:
    QPainterPath * painterPath;
    QPen pen;
    Mode mode;
};

#endif // DRAWNSHAPE_H
