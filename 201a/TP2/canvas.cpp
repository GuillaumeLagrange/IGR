#include <QPainter>
#include "canvas.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
    {
        start = new QPoint();
        currentPos = new QPoint();
        pen = new QPen();
        drawnShapes = new QList<DrawnShape>();
        mode = line;
        painterPath = new QPainterPath();
    }



void Canvas::paintEvent(QPaintEvent* e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);

    for (DrawnShape shape : *drawnShapes)
    {
        painter.setPen(shape.getPen());
        painter.drawPath(*(shape.getPath()));
    }

    painter.setPen(*pen);
    if (hasMouseTracking())
    {
        switch(mode)
        {
        case line :
            painter.drawLine(*start, *currentPos);
            break;
        case rectangle :
            painter.drawRect(*(new QRect(*start, *currentPos)));
            break;
        case ellipse :
            painter.drawEllipse(*(new QRect(*start, *currentPos)));
            break;
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent * e)
{
    setMouseTracking(true);
    *start = e->pos();
    *currentPos = e->pos();
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent * e)
{
    if (hasMouseTracking())
    {
        *currentPos = e->pos();
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent * e)
{
    setMouseTracking(false);
    switch(mode)
    {
        case line : drawnShapes->append(*(new DrawnShape(new QLine(*start,*currentPos),
                               pen)));
                    break;
        case rectangle : drawnShapes->append(*(new DrawnShape(new QRect(*start, *currentPos),
                                  pen, mode)));
                    break;
        case ellipse : drawnShapes->append(*(new DrawnShape(new QRect(*start, *currentPos),
                                pen, mode)));
                    break;
    }

    update();
}

void Canvas::setWidth(QAction *a)
{
    if (a->text() == tr("Wide"))
    {
        pen->setWidth(5);
    }
    if (a->text() == tr("Thin"))
        pen->setWidth(1);
}

void Canvas::setColor(QAction *a)
{
    if (a->text() == tr("Black"))
        pen->setColor(*(new QColor(Qt::black)));
    if (a->text() == tr("Blue"))
        pen->setColor(*(new QColor(Qt::blue)));
}

void Canvas::setStyle(QAction *a)
{
    if (a->text() == tr("Solid"))
        pen->setStyle(Qt::SolidLine);
    if (a->text() == tr("Dash"))
        pen->setStyle(Qt::DashLine);
}

void Canvas::deleteLine(QAction *a)
{
   if (a->text() == tr("Delete all"))
   {
        drawnShapes = new QList<DrawnShape>();
   }
   if (a->text() == tr("Delete last"))
   {
        drawnShapes->removeLast();
   }
   update();
}

void Canvas::switchMode(QAction *a)
{
    if (a->text() == tr("Line"))
        mode = line;
    if (a->text() == tr("Rectangle"))
        mode = rectangle;
    if (a->text() == tr("Ellipse"))
        mode = ellipse;
}
