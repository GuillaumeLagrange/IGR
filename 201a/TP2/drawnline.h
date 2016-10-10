#ifndef DRAWNLINE_H
#define DRAWNLINE_H

#include<QLine>
#include<QPen>

class DrawnLine
{
public:
    DrawnLine(QLine *l, QPen *p)
    {
        line = l;
//        QPen *pen = new QPen();
        pen = *p;
    }

    QPen  getPen() const {return pen;}
    QLine * getLine() const {return line;}

private:
    QLine *line;
    QPen pen;
};

#endif // DRAWNLINE_H
