#include <QPainter>
#include "canvas.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
    {
        start = new QPoint();
        currentPos = new QPoint();
        pen = new QPen();
        drawnLines = new QList<DrawnLine>();
        drawnShapes = new QList<DrawnShape>();
        mode = line;
        painterPath = new QPainterPath();
    }



void Canvas::paintEvent(QPaintEvent* e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);

    for (DrawnLine line : *drawnLines)
    {
        painter.setPen(line.getPen());
        painter.drawLine(*(line.getLine()));
    }

    for (DrawnShape shape : *drawnShapes)
    {
        painter.setPen(shape.getPen());
        painter.drawPath(*(shape.getPath()));
    }

    painter.setPen(*pen);
    if (hasMouseTracking())
    {
        if (mode ==  line)
        painter.drawLine(*start, *currentPos);
        if (mode == rectangle)
        painter.drawRect(*(new QRect(*start, *currentPos)));
        if (mode == ellipse)
        painter.drawEllipse(*(new QRect(*start, *currentPos)));
    }
}

void Canvas::mousePressEvent(QMouseEvent * e)
{
//    qDebug("MOUSE PRESSED");
    setMouseTracking(true);
    *start = e->pos();
    *currentPos = e->pos();
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent * e)
{
    if (hasMouseTracking())
    {
//        qDebug("MOUSE MOVED");
        *currentPos = e->pos();
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent * e)
{
//    qDebug("MOUSE RELEASED");
    setMouseTracking(false);
    switch(mode)
    {
        case line : drawnLines->append(*(new DrawnLine(new QLine(*start,*currentPos),
                               pen)));
                    lastDrawn = line;
        case rectangle : drawnShapes->append(*(new DrawnShape(new QRect(*start, *currentPos),
                                  pen, mode)));
                    lastDrawn = rectangle;
        case ellipse : drawnShapes->append(*(new DrawnShape(new QRect(*start, *currentPos),
                                pen, mode)));
                    lastDrawn = ellipse;
    }

    update();
}

void Canvas::setWidth(QAction *a)
{
//    qDebug("Received widthGroup signal");
    if (a->text() == tr("Wide"))
    {
        pen->setWidth(5);
//        qDebug("WIDE SET");
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
        drawnLines = new QList<DrawnLine>();
        drawnShapes = new QList<DrawnShape>();
   }
   if (a->text() == tr("Delete last"))
   {
        if (lastDrawn == line)
            drawnLines->removeLast();
        else
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
