#ifndef Q_BEAMER_PROXY_H
#define Q_BEAMER_PROXY_H

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
    
    void initPresentation();
    void deletePresentation();
    void runPresentation();
    void moveEditorToPosititon(QPoint position, double zoom);
    
    //QCanvasWidget *canvas;
    QString filepath;
    
    QCanvasWidget *canvas_edit;
    
private:
    QVBoxLayout *layout;
    Config *config;
    
signals:
    
public slots:
};

#endif // Q_BEAMER_PROXY_H
