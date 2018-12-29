#include "q_canvas_widget.h"

QCanvasWidget::QCanvasWidget(QWidget *parent)
    : QWidget(parent)
    , view (new QGraphicsView)
    , scene (new QGraphicsScene)
    , layout (new QVBoxLayout)
{
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
    //view->scale(0.2, 0.2);
    
    
    //view->fitInView(scene->sceneRect());
}

void QCanvasWidget::addJSON(QString path)
{
    this->current_path = path;
    
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
    /*
    for (int i=0; i < steps_arr.count(); ++i)
    {
        QJsonObject obj = steps_arr.at(i).toObject();
        QString type = obj.value("type").toString();
        
        //steps.append()
    }
    */
    
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
            this->nodes_map[obj.value("id").toInt()] = pos;
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
                obj.value("scale").toInt()
            );
        }
    }
    qDebug() << nodes_map;
    
    
    
}

void QCanvasWidget::addHTML(int parent, int id, QString html, double dx, double dy, int rotate, int scale)
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
        int par_x = int(this->nodes_map[parent].x());
        int par_y = int(this->nodes_map[parent].y());
        // calculate new absolute position
        pos_x = int(par_x + dx * this->resolution_width);
        pos_y = int(par_y + dy * this->resolution_height);
    }
    
    QWebView *web_view = new QWebView();
    web_view->setHtml(html_full);
    web_view->setFixedSize(this->resolution_width, this->resolution_height);
    web_view->setZoomFactor(scale);
    web_view->move(pos_x, pos_y);
    
    web_view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    web_view->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    
    QGraphicsProxyWidget *proxy = scene->addWidget(web_view);
}

void QCanvasWidget::drawControlls()
{
    
}

void QCanvasWidget::stepForward()
{
    this->step_active++;
    
    // detect overflow to start over
    if (this->step_active >= this->steps_array.count() - 1)
    {
        this->step_active = 0;
    }
    
    QJsonObject obj = this->steps_array.at(this->step_active).toObject();
    QString type = obj.value("type").toString();
    if (type == "frame")
    {
        int frame_id = obj.value("frame-id").toInt();
        
    }
}

void QCanvasWidget::stepBackward()
{
    this->step_active--;
    
    // detect underflow to start over
    if (this->step_active <= 0)
    {
        this->step_active = this->steps_array.count() - 1;
    }
    
    scrollToPosition(1920, 1080);
}

void QCanvasWidget::scrollToPosition(int x, int y)
{
    view->horizontalScrollBar()->setValue(x);
    view->verticalScrollBar()->setValue(y);
}

void QCanvasWidget::reloadAll()
{
    scene->clear();
    addJSON(this->current_path);
}

void QCanvasWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "canvas";
    switch (event->key())
    {
        case Qt::Key_1:
        {
            stepBackward();
            break;
        }
        case Qt::Key_2:
        {
            stepForward();
            break;
        }
        case Qt::Key_PageUp:
        {
            stepBackward();
            break;
        }
        case Qt::Key_PageDown:
        {
            stepForward();
            break;
        }
        case Qt::Key_F5:
        {
            reloadAll();
            break;
        }
        case Qt::Key_Escape:
        {
            if (!this->editMode)
            {
                deleteLater();
                emit deleteBeamerWindow();
            }
            break;
        }
    }
}
