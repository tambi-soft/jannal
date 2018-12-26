#ifndef Q_CANVAS_WIDGET_H
#define Q_CANVAS_WIDGET_H

#include <QObject>
#include <QWidget>

class QCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCanvasWidget(QWidget *parent = nullptr);
    
signals:
    
public slots:
};

#endif // Q_CANVAS_WIDGET_H