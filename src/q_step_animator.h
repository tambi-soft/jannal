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
    void setZoom(double zoom_from, double zoom_to);
    void halt();
    
    double speed = 50;
    
protected:
    void run();
    
    QPoint pos_from;
    QPoint pos_to;
    
    double zoom_from, zoom_to;
    
private:
    bool halt_thread = false;
    
    void stepCoordinates();
    void stepZoom();
    
signals:
    void currentAnimationStepCoordinates(QPoint position);
    void currentAnimationStepZoom(double factor);
    
public slots:
};

#endif // Q_STEP_ANIMATOR_H
