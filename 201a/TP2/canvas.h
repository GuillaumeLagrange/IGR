#ifndef CANVAS_H
#define CANVAS_H

#include <QVector>
#include <QLine>
#include <QPoint>
#include <QPen>
#include <QColor>
#include <QMouseEvent>
#include <Qt>
#include <QActionGroup>
#include <QWidget>
#include <QList>

#include"drawnline.h"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget* parent= 0);
    QPen * getPen() {return pen;}

public slots:
    void setColor(QAction * a);
    void setWidth(QAction * a);
    void setStyle(QAction * a);

protected:
    virtual void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    QPoint * start;
    QPoint * currentPos;
    QPen * pen;
    QList<DrawnLine> * drawnLines;
};

#endif // CANVAS_H
