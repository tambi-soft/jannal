#include "q_canvas_widget.h"

QCanvasWidget::QCanvasWidget(QWidget *parent)
    : QWidget(parent)
    , view (new QGraphicsView)
    , scene (new QGraphicsScene)
    , layout (new QVBoxLayout)
{
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
    
    this->resolution_width = 1920;
    this->resolution_height = 1080;
    
    //view->show();
    view->showFullScreen();
    
    //view->fitInView(scene->sceneRect());
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
    
    QJsonValue graph_val = json_document["graphics"];
    QJsonArray graph_arr = graph_val.toArray();
    
    for (int i=0; i < graph_arr.count(); ++i)
    {
        QJsonObject obj = graph_arr.at(i).toObject();
        
        QString type = obj.value("type").toString();
        if (type == "frame-html")
        {
            addHTML(
                obj.value("html").toString(),
                obj.value("pos-x").toInt(),
                obj.value("pos-y").toInt(),
                obj.value("rotate").toInt(),
                obj.value("scale").toInt()
            );
        }
    }
}

void QCanvasWidget::addHTML(QString html, int pos_x, int pos_y, int rotate, int scale)
{
    QString css;
    QFile file;
    file.setFileName(":test_style");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    css = file.readAll();
    file.close();
    
    QString html_full = "<!doctype html>"
        "<html lang=\"de\">"
        "<head>"
            "<meta charset=\"utf-8\" />"
            "<style>" + css + "</style>"
        "</head>"
        "<body>" + html + "</body>";
    
    QWebView *web_view = new QWebView();
    web_view->setHtml(html_full);
    scene->addWidget(web_view);
    web_view->setFixedSize(this->resolution_width, this->resolution_height);
    web_view->move(pos_x, pos_y);
}
