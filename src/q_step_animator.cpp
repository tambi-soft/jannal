#include "q_step_animator.h"

QStepAnimator::QStepAnimator(QThread *parent) : QThread(parent)
{
    setTerminationEnabled(true);
}

void QStepAnimator::setCoordinates(QPoint pos_from, QPoint pos_to)
{
    this->halt_thread = false;
    this->pos_from = pos_from;
    this->pos_to = pos_to;
}

void QStepAnimator::setZoom(double zoom_from, double zoom_to)
{
    this->zoom_from = zoom_from;
    this->zoom_to = zoom_to;
}

void QStepAnimator::run()
{
    double dx;
    double dy;
    double dz;
    
    QPoint pos_current = this->pos_from;
    double zoom_current = this->zoom_from;
    qDebug() << pos_from;
    
    while (this->halt_thread == false)
    {
        usleep(16666);
        //msleep(17); // for ~60 updates per second
        
        // calculate coordinates with phythagoras
        dx = pos_to.x() - pos_current.x();
        dy = pos_to.y() - pos_current.y();
        dz = this->zoom_to - zoom_current;
        
        //double dist = qSqrt(dx * dx + dy * dy);
        double dist = qSqrt(dx*dx + dy*dy + dz*dz);
        if (dist >= this->speed)
        {
            double vel_x = (dx / dist) * this->speed;
            double vel_y = (dy / dist) * this->speed;
            double vel_z = (dz / dist) * this->speed;
            
            pos_current.setX(int(pos_current.x() + vel_x));
            pos_current.setY(int(pos_current.y() + vel_y));
            zoom_current = zoom_current + vel_z;
            
            //qDebug() << pos_current;
            
            emit currentAnimationStepCoordinates(pos_current);
            emit currentAnimationStepZoom(zoom_current);
        }
        else
        {
            emit currentAnimationStepCoordinates(this->pos_to);
            emit currentAnimationStepZoom(this->zoom_to);
            
            return;
        }
        
        if (pos_current == pos_to)
        {
            this->halt_thread = true;
        }
    }
}

void QStepAnimator::halt()
{
    this->halt_thread = true;
}
