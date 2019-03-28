#ifndef Q_CANVAS_WIDGET_H
#define Q_CANVAS_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QWindow>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVBoxLayout>

#include <QFile>
#include <QDir>

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
#include <QPushButton>
#include <QPixmap>

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

#include "q_graphics_edit_proxy_widget.h"
#include "q_zoomable_graphics_view.h"
#include "q_step_animator.h"

class QCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCanvasWidget(QString filepath, bool edit_mode, int screen_number, QWidget *parent = nullptr);
    
    bool editMode = true;
    
    void addJSON(QString path);
    void addFrameHTML(QJsonObject object, int parent, int id, double dx, double dy, int rotate, double scale, QString tree_edge);
    void addFrameUrl(int parent, int id, QString html, double dx, double dy, int rotate, double scale, QString tree_edge, bool show_scroll_bars);
    void addFrameImage(int parent, int id, QString image_path, double dx, double dy, int rotate, double scale, QString tree_edge, bool show_scroll_bars);
    QMap<QString, QPoint> calculateFramePos(int parent, int id, double dx, double dy);
    void drawTreeEdge(int par_x, int par_y, int pos_x, int pos_y, int id);
    void drawLine(QPoint from, QPoint to, int width, QString color_str);
    void scrollToPosition(int x, int y, double zoom);
    void scrollToPosition(QPointF pos, double zoom);
    void scrollToPosition(QPoint pos, double zoom);
    void movePresentationMarker(QPoint position, double scale);
    QGraphicsItem *stepMarker = nullptr;
    
    void drawControlls();
    void stepToStart();
    void stepForward();
    void stepBackward();
    
    void scaleView(double factor);
    double scale_factor = 1;
    double scale_offset = 1; // for smaller or larger displays than the specified resolution
    double animation_speed = 50;
    double animation_speed_from_config = 50;
    
    int screen_number = 0;
    
    QDir *dir_path;
    
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
    void currentAnimationStepCoordinates(QPoint position, double zoom);
    void currentAnimationStepZoom(double factor);
    void stepForwardSignal();
    void stepBackwardSignal();
    
public slots:
    
protected:
    bool eventFilter(QObject */*target*/, QEvent *event);
    void showEvent(QShowEvent *);
};

#endif // Q_CANVAS_WIDGET_H
