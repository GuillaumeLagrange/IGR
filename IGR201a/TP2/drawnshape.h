#ifndef DRAWNSHAPE_H
#define DRAWNSHAPE_H

#include <QPainterPath>
#include <QPen>
#include "mode.h"

class DrawnShape
{
public:
    DrawnShape (QRect *rect, QPen *p, Mode m)
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

    DrawnShape(QLine *l, QPen *p)
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
    QPainterPath * getPath() const {return painterPath;}


private:
    QPainterPath * painterPath;
    QPen pen;
    Mode mode;
};

#endif // DRAWNSHAPE_H
