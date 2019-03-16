#ifndef Q_CANVAS_SCENE_H
#define Q_CANVAS_SCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>

#include <QFile>
#include <QWebView>

class QCanvasScene : public QGraphicsScene
{
public:
    QCanvasScene(QGraphicsScene *parent = nullptr);
    
    QSize size;
    QString dir_path;
    
    void setSize(QSize size);
    void addHTML(int parent, int id, QString html, double dx, double dy, int rotate, double scale, QString tree_edge, bool show_scroll_bars);
    void drawTreeEdge(int par_x, int par_y, int pos_x, int pos_y, int id);
    
};

#endif // Q_CANVAS_SCENE_H
