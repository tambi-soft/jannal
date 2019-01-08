#ifndef Q_CANVAS_WIDGET_H
#define Q_CANVAS_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QWindow>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

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

#include <QtMath>

#include <QDebug>

#include "q_zoomable_graphics_view.h"
#include "q_step_animator.h"

class QCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCanvasWidget(bool edit_mode, QWidget *parent = nullptr);
    
    bool editMode = true;
    
    void addJSON(QString path);
    void addHTML(int parent, int id, QString html, double dx, double dy, int rotate, double scale, QString tree_edge, bool show_scroll_bars);
    void drawTreeEdge(int par_x, int par_y, int pos_x, int pos_y, int id);
    void drawLine(QPoint from, QPoint to, int width, QString color_str);
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
    QJsonObject conf_obj;
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
    void showEvent(QShowEvent *);
};

#endif // Q_CANVAS_WIDGET_H
