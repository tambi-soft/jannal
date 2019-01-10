#ifndef Q_BEAMER_PROXY_H
#define Q_BEAMER_PROXY_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QDebug>

#include "q_canvas_widget.h"

class QBeamerProxy : public QWidget
{
    Q_OBJECT
public:
    explicit QBeamerProxy(QWidget *parent = nullptr);
    
    void initPresentation(QString filepath);
    void runPresentation();
    
    //QCanvasWidget *canvas;
    QString filepath;
    
private:
    QVBoxLayout *layout;
    
signals:
    
public slots:
};

#endif // Q_BEAMER_PROXY_H
