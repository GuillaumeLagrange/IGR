#ifndef CANVAS_H
#define CANVAS_H

#include <QPainterPath>
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

#include "mode.h"
#include "drawnshape.h"

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
    void deleteLine(QAction *a);
    void switchMode(QAction* a);

protected:
    virtual void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    QPointF * start;
    QPointF * currentPos;
    QPen * pen;
    QList<DrawnShape> * drawnShapes;
    Mode mode;
    QPainterPath *currentPainterPath;
    int editing;
    bool moving;
};

#endif // CANVAS_H
