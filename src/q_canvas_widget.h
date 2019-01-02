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

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#include <QTimer>
#include <QTimeLine>

#include <QDebug>

#include "q_zoomable_graphics_view.h"
#include "q_step_animator.h"

class QCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCanvasWidget(QWidget *parent = nullptr);
    
    bool editMode = false;
    
    void addJSON(QString path);
    void addHTML(int parent, int id, QString html, double dx, double dy, int rotate, double scale);
    void scrollToPosition(int x, int y);
    void scrollToPosition(QPointF pos);
    void scrollToPosition(QPoint pos);
    
    void drawControlls();
    void stepToStart();
    void stepForward();
    void stepBackward();
    
    void scaleView(double factor);
    double scale_factor = 1;
    double scale_offset = 1; // e.g. for smaller or larger displays than the specified resolution
    double animation_speed;
    
private:
    int resolution_width;
    int resolution_height;
    
    //QMap<int, QPointF> nodes_map;
    QMap<int, QMap<QString, QVariant>> nodes_map;
    //QList<QPointF> steps;
    QJsonArray steps_array;
    int step_active = 0;
    
    void stepHelper();
    
    QZoomableGraphicsView *view_zoomable;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QVBoxLayout* layout;
    
    QStepAnimator* step_animator;
    
signals:
    void reloadCanvas();
    void deleteBeamerWindow();
    
public slots:
    
protected:
    bool eventFilter(QObject */*target*/, QEvent *event);
};

#endif // Q_CANVAS_WIDGET_H
