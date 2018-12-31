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

void QStepAnimator::run()
{
    double dx = pos_to.x() - pos_from.x();
    double dy = pos_to.y() - pos_from.y();
    
    QPoint pos_current = pos_from;
    
    while (this->halt_thread == false)
    {
        usleep(2000);
        //pos_current.setX(pos_current.x() + 5);
        //pos_current.setY(pos_current.y() + 5);
        
        dx = pos_to.x() - pos_current.x();
        dy = pos_to.y() - pos_current.y();
        
        double dist = qSqrt(dx * dx + dy * dy);
        if (dist >= this->speed)
        {
            double vel_x = (dx / dist) * speed;
            double vel_y = (dy / dist) * speed;
            
            pos_current.setX(int(pos_current.x() + vel_x));
            pos_current.setY(int(pos_current.y() + vel_y));
        }
        else
        {
            return;
        }
        
        emit currentAnimationStepCoordinates(pos_current);
        
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
