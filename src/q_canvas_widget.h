#ifndef Q_CANVAS_WIDGET_H
#define Q_CANVAS_WIDGET_H

#include <QObject>
#include <QWidget>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVBoxLayout>

#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStack>
#include <QTransform>

#include <QWebView>
#include <QUrl>

#include <QGraphicsLineItem>

#include <QDebug>

class QCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCanvasWidget(QWidget *parent = nullptr);
    
    void addJSON(QString path);
    void addHTML(QString html, int pos_x, int pos_y, int rotate, int scale);
    
private:
    int resolution_width;
    int resolution_height;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QVBoxLayout* layout;
    
signals:
    
public slots:
};

#endif // Q_CANVAS_WIDGET_H
