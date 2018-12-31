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
#include <QWebFrame>
#include <QUrl>
#include <QScrollBar>

#include <QPointF>
#include <QMap>
#include <QList>
#include <QVariant>

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>

#include <QPushButton>

#include <QKeyEvent>
#include <QMouseEvent>

#include <QDebug>

#include "q_step_animator.h"

class QCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCanvasWidget(QWidget *parent = nullptr);
    
    bool editMode = false;
    
    QString current_path;
    void addJSON(QString path);
    void addHTML(int parent, int id, QString html, double dx, double dy, int rotate, int scale);
    void scrollToPosition(int x, int y);
    void scrollToPosition(QPointF pos);
    void scrollToPosition(QPoint pos);
    
    void drawControlls();
    void stepForward();
    void stepBackward();
    
    void reloadAll();
    
private:
    int resolution_width;
    int resolution_height;
    
    //QMap<int, QPointF> nodes_map;
    QMap<int, QMap<QString, QVariant>> nodes_map;
    //QList<QPointF> steps;
    QJsonArray steps_array;
    int step_active = 0;
    
    void stepHelper();
    
    QGraphicsView* view;
    QGraphicsScene* scene;
    QVBoxLayout* layout;
    
    QStepAnimator* step_animator;
    
signals:
    void deleteBeamerWindow();
    
public slots:
    
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // Q_CANVAS_WIDGET_H
