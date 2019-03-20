#include "q_step_animator.h"

QStepAnimator::QStepAnimator(QThread *parent) : QThread(parent)
{
    setTerminationEnabled(true);
    
    // seting up the default speed
    this->default_speed = 50;
    this->speed = default_speed;
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

void QStepAnimator::setSpeed(double speed)
{
    this->speed = speed;
}

void QStepAnimator::resetSpeed()
{
    this->speed = default_speed;
}

void QStepAnimator::run()
{
    double dx;
    double dy;
    double dz;
    
    QPoint pos_current = this->pos_from;
    double zoom_current = this->zoom_from;
    
    while (this->halt_thread == false)
    {
        // speed = 0 is instant jump. 
        if (this->speed <= 0.1 && this->speed >= -0.1)
        {
            emit currentAnimationStepCoordinates(this->pos_to, this->zoom_to);
            emit currentAnimationStepZoom(this->zoom_to);
            
            return;
        }
        
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
            
            emit currentAnimationStepCoordinates(pos_current, zoom_current);
            emit currentAnimationStepZoom(zoom_current);
        }
        else
        {
            emit currentAnimationStepCoordinates(this->pos_to, this->zoom_to);
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
