#include "q_canvas_widget.h"

QCanvasWidget::QCanvasWidget(QWidget *parent)
    : QWidget(parent)
    , view (new QGraphicsView)
    , scene (new QGraphicsScene)
    , layout (new QVBoxLayout)
    , step_animator (new QStepAnimator)
{
    connect(step_animator, &QStepAnimator::currentAnimationStepCoordinates, this, qOverload<QPoint>(&QCanvasWidget::scrollToPosition));
    connect(step_animator, &QStepAnimator::currentAnimationStepZoom, this, &QCanvasWidget::scaleView);
    
    //this->view_zoomable = new QZoomableGraphicsView(view);
    //this->view_zoomable->set_modifiers(Qt::NoModifier);
    
    //view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setRenderHint(QPainter::Antialiasing);
    
    view->installEventFilter(this);
    scene->installEventFilter(this);
    //layout->installEventFilter(this);
    
    this->resolution_width = 1920; // 480
    this->resolution_height = 1080; // 270
    
    view->setScene(scene);
    
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(view);
    setLayout(layout);
    
    addJSON(":test_json");
    
    //view->scale(10, 10);
    
    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);
    
    //scene->addRect(100, 0, 80, 100, outlinePen, blueBrush);
    
    drawControlls();
    
    //view->show();
    view->showFullScreen();
    
    //scrollToPosition(1000, 1000);
    //view->scale(0.1, 0.1);
    //view->scale(1.2, 1.2);
    
    
    //view->fitInView(scene->sceneRect());
    
    //qDebug() << scene->items();
    
    //qDebug() << this->nodes_map;
}

void QCanvasWidget::addJSON(QString path)
{
    QString json_string;
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    json_string = file.readAll();
    file.close();
    
    QJsonDocument json_document = QJsonDocument::fromJson(json_string.toUtf8());
    
    // handle the steps
    QJsonValue steps_val = json_document["steps"];
    this->steps_array = steps_val.toArray();
    
    // handle the frames
    QJsonValue graph_val = json_document["graphics"];
    QJsonArray graph_arr = graph_val.toArray();
    
    for (int i=0; i < graph_arr.count(); ++i)
    {
        QJsonObject obj = graph_arr.at(i).toObject();
        QString type = obj.value("type").toString();
        
        if (type.startsWith("frame"))
        {
            // insert id -> position into this->nodes_list
            QPointF pos;
            pos = QPointF(
                obj.value("x").toDouble(),
                obj.value("y").toDouble()
            );
            //this->nodes_map[obj.value("id").toInt()] = pos;
        }
        
        // draw the content
        if (type == "frame-html")
        {
            addHTML(
                obj.value("parent").toInt(),
                obj.value("id").toInt(),
                obj.value("html").toString(),
                obj.value("x").toDouble(),
                obj.value("y").toDouble(),
                obj.value("rotate").toInt(),
                obj.value("scale").toDouble()
            );
        }
    }
    
    
    
}

void QCanvasWidget::addHTML(int parent, int id, QString html, double dx, double dy, int rotate, double scale)
{
    // read the stylesheet
    QString css;
    QFile file;
    file.setFileName(":test_style");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    css = file.readAll();
    file.close();
    // build data and stylesheet to a minimal html-document
    QString html_full = "<!doctype html>"
        "<html lang=\"de\">"
        "<head>"
            "<meta charset=\"utf-8\" />"
            "<style>" + css + "</style>"
        "</head>"
        "<body>" + html + "</body>";
    
    // calculate absolute position
    int pos_x = 0;
    int pos_y = 0;
    if (parent == id)
    // if parent == id, we have no parent. that means: absolute positioning
    {
        pos_x = int(dx);
        pos_y = int(dy);
    }
    else
    {
        // get parent position
        int par_x = int(this->nodes_map[parent]["pos"].toPointF().x());
        int par_y = int(this->nodes_map[parent]["pos"].toPointF().y());
        // calculate new absolute position
        pos_x = int(par_x + dx * this->resolution_width);
        pos_y = int(par_y + dy * this->resolution_height);
    }
    
    QWebView *web_view = new QWebView();
    web_view->setHtml(html_full);
    web_view->setFixedSize(this->resolution_width, this->resolution_height);
    //web_view->setZoomFactor(scale);
    web_view->move(pos_x, pos_y);
    
    web_view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    web_view->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    
    QGraphicsProxyWidget *proxy = scene->addWidget(web_view);
    proxy->setRotation(rotate);
    proxy->setScale(scale);
    
    QMap<QString, QVariant> props;
    props["pos"] = QPointF(pos_x, pos_y);
    props["parent"] = parent;
    props["rotate"] = rotate;
    props["scale"] = scale;
    props["type"] = "frame-html";
    props["widget"] = QVariant::fromValue(proxy);
    this->nodes_map[id] = props;
    
    //qDebug() << qvariant_cast<QGraphicsProxyWidget*>(props["widget"]);
}

void QCanvasWidget::drawControlls()
{
    
}

void QCanvasWidget::stepForward()
{
    this->step_active++;
    
    // detect overflow to start over
    if (this->step_active > this->steps_array.count()-1)
    {
        this->step_active = 0;
    }
    
    stepHelper();
}

void QCanvasWidget::stepBackward()
{
    this->step_active--;
    
    // detect underflow to start over
    if (this->step_active < 0)
    {
        this->step_active = this->steps_array.count()-1;
    }
    
    stepHelper();
}

void QCanvasWidget::stepHelper()
{
    QJsonObject obj = this->steps_array.at(this->step_active).toObject();
    QString type = obj.value("type").toString();
    QPointF pos_to;
    if (type == "frame")
    {
        int frame_id = obj.value("frame-id").toInt();
        pos_to = this->nodes_map[frame_id]["pos"].toPointF();
    }
    else if (type == "position")
    {
        pos_to.setX(obj.value("pos-x").toInt());
        pos_to.setY(obj.value("pos-y").toInt());
    }
    
    QPoint pos_from;
    pos_from.setX(view->horizontalScrollBar()->value());
    pos_from.setY(view->verticalScrollBar()->value());
    
    step_animator->quit();
    step_animator->halt();
    step_animator->setCoordinates(pos_from, QPoint(int(pos_to.x()), int(pos_to.y())));
    step_animator->setZoom(this->scale_factor, obj.value("zoom").toDouble());
    step_animator->start();
}

void QCanvasWidget::scaleView(double factor)
{
    this->scale_factor = factor;
    
    this->view->resetMatrix();
    view->scale(factor, factor);
}

void QCanvasWidget::scrollToPosition(int x, int y)
{
    /*
    QPointF mapped = view->mapFromScene(QPointF(x, y));
    x = int(mapped.x());
    y = int(mapped.y());
    */
    
    //this->view_zoomable->gentle_zoom(2);
    
    view->horizontalScrollBar()->setValue(x);
    view->verticalScrollBar()->setValue(y);
}
void QCanvasWidget::scrollToPosition(QPointF pos)
{
    scrollToPosition(int(pos.x()), int(pos.y()));
}
void QCanvasWidget::scrollToPosition(QPoint pos)
{
    scrollToPosition(pos.x(), pos.y());
}

bool QCanvasWidget::eventFilter(QObject */*target*/, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
        switch (key_event->key())
        {
            case Qt::Key_Left:
            {
                stepBackward();
                return true;
            }
            case Qt::Key_Right:
            {
                stepForward();
                return true;
            }
            case Qt::Key_PageUp:
            {
                stepBackward();
                return true;
            }
            case Qt::Key_PageDown:
            {
                stepForward();
                return true;
            }
            case Qt::Key_F5:
            {
                emit reloadCanvas();
                return true;
            }
            case Qt::Key_Escape:
            {
                if (!this->editMode)
                {
                    deleteLater();
                    emit deleteBeamerWindow();
                }
                return true;
            }
        }
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
        qDebug() << view->mapToScene(mouse_event->pos());
    }
    
    return false;
}
