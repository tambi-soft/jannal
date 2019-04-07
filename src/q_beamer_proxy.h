#ifndef Q_BEAMER_PROXY_H
#define Q_BEAMER_PROXY_H

#include <QCoreApplication>
#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QDebug>

#include "q_canvas_widget.h"
#include "config.h"

class QBeamerProxy : public QWidget
{
    Q_OBJECT
public:
    explicit QBeamerProxy(QString filepath, QWidget *parent = nullptr);
    ~QBeamerProxy();
    
    void initPresentation();
    void reloadPresentation();
    void runPresentation();
    void moveEditorToPosititon(QPoint position, double zoom);
    void presentationStepBackward();
    void presentationStepForward();
    
    //QCanvasWidget *canvas;
    QString filepath;
    
    QCanvasWidget *canvas_edit;
    
private:
    QVBoxLayout *layout;
    Config *config;
    QCanvasWidget *presentation_canvas;
    
signals:
    
public slots:
    
protected:
    
};

#endif // Q_BEAMER_PROXY_H
