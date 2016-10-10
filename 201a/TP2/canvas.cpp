#include <QPainter>
#include "canvas.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
    {
        start = new QPoint();
        currentPos = new QPoint();
        pen = new QPen();
        drawnLines = new QList<DrawnLine>();
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

    painter.setPen(*pen);
    if (hasMouseTracking())
        painter.drawLine(*start, *currentPos);
//    painter.drawLine(50, 10, 100, 20);
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
    drawnLines->append(*(new DrawnLine(new QLine(*start,*currentPos), pen)));
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
    if (a->text() == "Thin")
        pen->setWidth(1);
}

void Canvas::setColor(QAction *a)
{
    if (a->text() == tr("Black"))
        pen->setColor(*(new QColor(Qt::black)));
    if (a->text() == "Blue")
        pen->setColor(*(new QColor(Qt::blue)));
}

void Canvas::setStyle(QAction *a)
{
    if (a->text() == "Solid")
        pen->setStyle(Qt::SolidLine);
    if (a->text() == "Dash")
        pen->setStyle(Qt::DashLine);
}
