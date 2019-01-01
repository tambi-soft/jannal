#ifndef Q_STEP_ANIMATOR_H
#define Q_STEP_ANIMATOR_H

#include <QObject>
#include <QThread>

#include <QPoint>

#include <QtMath>

#include <QDebug>

class QStepAnimator : public QThread
{
    Q_OBJECT
public:
    explicit QStepAnimator(QThread *parent = nullptr);
    
    void setCoordinates(QPoint pos_from, QPoint pos_to);
    void halt();
    
    double speed = 8;
    
protected:
    void run();
    
    QPoint pos_from;
    QPoint pos_to;
    
private:
    bool halt_thread = false;
    
signals:
    void currentAnimationStepCoordinates(QPoint position, double scale);
    
public slots:
};

#endif // Q_STEP_ANIMATOR_H
