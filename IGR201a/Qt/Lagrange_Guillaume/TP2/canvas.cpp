#include <QPainter>
#include "canvas.h"


Canvas::Canvas(QWidget * parent) : QWidget(parent)
    {
        start = new QPointF();
        currentPos = new QPointF();
        pen = new QPen();
        drawnShapes = new QList<DrawnShape>();
        mode = line;
        currentPainterPath = new QPainterPath();
        editing = -1;
        moving = false;
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
                painter.drawRect(*(new QRectF(*start, *currentPos)));
            break;

            case ellipse :
                painter.drawEllipse(*(new QRectF(*start, *currentPos)));
            break;

            case polygon :
                painter.drawPath(*currentPainterPath);
                painter.drawLine(*start, *currentPos);
            break;

            case edit :
                qreal dx = currentPos->x() - start->x();
                qreal dy = currentPos->y() - start->y();
//                QPointF * offset = new QPointF(dx/10000,dy/10000);
                *currentPainterPath = (*drawnShapes)[editing].getPath()->translated(dx,dy);
                painter.drawPath(*currentPainterPath);
//                currentPainterPath = (*drawnShapes)[editing].getPath();
//                for (int i = 0; i < currentPainterPath->length(); i++)
//                {
//                    qreal oldx = currentPainterPath->elementAt(i).x;
//                    qreal oldy = currentPainterPath->elementAt(i).y;
//                    currentPainterPath->setElementPositionAt(i, oldx + dx, oldy + dy);
//                }
            break;
        }
    }
    /* Paint the already drawn shapes */
    for (int i = 0; i < drawnShapes->length(); i++)
    {
        if (i != editing || !moving)
        {
            DrawnShape shape = (*drawnShapes)[i];
            painter.setPen(shape.getPen());
            painter.drawPath(*(shape.getPath()));
        }
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
            QPointF *topLeft = new QPointF(e->x() - 5, e->y() - 5);
            QPointF *bottomRight = new QPointF(e->x() + 5, e->y() + 5);
            QRectF *select = new QRectF(*topLeft, *bottomRight);

            for (int i = 0; i < drawnShapes->size(); i++)
            {
                if ((drawnShapes->at(i)).getPath()->intersects(*select)&&!moving)
                {
//                    int i = drawnShapes->indexOf(shape);
                    editing = i;
                    moving = true;
                }
            }

            if (moving)
            {
                setMouseTracking(true);
                *pen = (*drawnShapes)[editing].getPen();
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
            drawnShapes->append(*(new DrawnShape(new QLineF(*start,*currentPos),
                                                 pen)));
            setMouseTracking(false);
            break;
        case rectangle :
            drawnShapes->append(*(new DrawnShape(new QRectF(*start, *currentPos),
                                                 pen, mode)));
            setMouseTracking(false);
            break;
        case ellipse :
            drawnShapes->append(*(new DrawnShape(new QRectF(*start, *currentPos),
                                                 pen, mode)));
            setMouseTracking(false);
            break;
        case polygon :
            break;

        case edit :
        {
            setMouseTracking(false);
            moving = false;
            (*drawnShapes)[editing].setPath(currentPainterPath);
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

    if (mode == edit && (editing != -1))
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

    if (mode == edit && (editing != -1))
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

    if (mode == edit && editing != -1)
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
    editing = -1;
}
