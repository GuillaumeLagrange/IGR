#include <QPainter>
#include "canvas.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
    {
        start = new QPoint();
        currentPos = new QPoint();
        pen = new QPen();
        drawnShapes = new QList<DrawnShape>();
        mode = line;
        currentPainterPath = new QPainterPath();
        editing = -1;
        found = false;
    }



void Canvas::paintEvent(QPaintEvent* e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);

    /* Paint the current shapes */
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

            case polygon :
                painter.drawPath(*currentPainterPath);
                painter.drawLine(*start, *currentPos);
            break;

            case edit :
//                qreal dx = currentPos->rx() - start->rx();
//                qreal dy = currentPos->ry() - start->ry();
//                QPoint * offset = new QPoint(*currentPos);
//                (*drawnShapes)[editing].getPath()->translate(*offset);
                (*drawnShapes)[editing].getPath()->setElementPositionAt(*currentPos);
            break;
        }
    }
    /* Paint the already drawn shapes */
    for (DrawnShape shape : *drawnShapes)
    {
        painter.setPen(shape.getPen());
        painter.drawPath(*(shape.getPath()));
    }
}

void Canvas::mousePressEvent(QMouseEvent * e)
{
    switch(mode)
    {
        case polygon :
        {
            switch (e->button())
            {
                case Qt::LeftButton:
                if (!hasMouseTracking())
                {
                    *start = e->pos();
                    *currentPos = e->pos();
                    currentPainterPath->moveTo(*start);
                    currentPainterPath->lineTo(*start);
                    setMouseTracking(true);
                }
                else
                {
                    currentPainterPath->lineTo(*currentPos);
                    currentPainterPath->moveTo(*currentPos);
                    *start=*currentPos;
                }
                break;

                default :
                if (!currentPainterPath->isEmpty())
                {
                    currentPainterPath->lineTo(*currentPos);
                    currentPainterPath->connectPath(*currentPainterPath);
                    drawnShapes->append(*(new DrawnShape(currentPainterPath, pen)));
                    currentPainterPath = new QPainterPath();
                    setMouseTracking(false);
                }
                break;
                update();
            }
            break;
        }

        case edit :
        {
            /* We are looking of the shape thats has been clicked on */
            /* Then we put it in currentShape */
            *start = e->pos();
            *currentPos = e->pos();
            QPoint *topLeft = new QPoint(e->x() - 5, e->y() - 5);
            QPoint *bottomRight = new QPoint(e->x() + 5, e->y() + 5);
            QRect *select = new QRect(*topLeft, *bottomRight);

            for (int i = 0; i < drawnShapes->size(); i++)
            {
                if ((drawnShapes->at(i)).getPath()->intersects(*select)&&!found)
                {
//                    int i = drawnShapes->indexOf(shape);
                    editing = i;
                    found = true;
                }
            }

            if (found)
            {
                setMouseTracking(true);

            }


            break;
        }

        default :
            setMouseTracking(true);
            *start = e->pos();
            *currentPos = e->pos();
            update();
        break;
    }
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
    switch(mode)
    {
        case line :
            drawnShapes->append(*(new DrawnShape(new QLine(*start,*currentPos),
                                                 pen)));
            setMouseTracking(false);
            break;
        case rectangle :
            drawnShapes->append(*(new DrawnShape(new QRect(*start, *currentPos),
                                                 pen, mode)));
            setMouseTracking(false);
            break;
        case ellipse :
            drawnShapes->append(*(new DrawnShape(new QRect(*start, *currentPos),
                                                 pen, mode)));
            setMouseTracking(false);
            break;
        case polygon :
        {
            setMouseTracking(false);
            editing = -1;
            found = false;
            qDebug("kikoo");
            break;
        }
    }

    update();
}

void Canvas::setWidth(QAction *a)
{
    if (a->text() == tr("Wide"))
        pen->setWidth(5);
    if (a->text() == tr("Thin"))
        pen->setWidth(1);

    if (mode == edit && found)
    {
       (*drawnShapes)[editing].setPen(*pen);
        update();
    }
}

void Canvas::setColor(QAction *a)
{
    if (a->text() == tr("Black"))
        pen->setColor(*(new QColor(Qt::black)));
    if (a->text() == tr("Blue"))
        pen->setColor(*(new QColor(Qt::blue)));

    if (mode == edit && found)
    {
       (*drawnShapes)[editing].setPen(*pen);
        update();
    }
}

void Canvas::setStyle(QAction *a)
{
    if (a->text() == tr("Solid"))
        pen->setStyle(Qt::SolidLine);
    if (a->text() == tr("Dash"))
        pen->setStyle(Qt::DashLine);

    if (mode == edit && found)
    {
       (*drawnShapes)[editing].setPen(*pen);
        update();
    }
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
    if (a->text() == tr("Polygon"))
        mode = polygon;
    if (a->text() == tr("Edit"))
        mode = edit;
}
