#include "q_canvas_scene.h"

QCanvasScene::QCanvasScene(QGraphicsScene *parent)
{
    
}

void QCanvasScene::setSize(QSize size)
{
    this->size = size;
}

void QCanvasScene::addHTML(int parent, int id, QString html, double dx, double dy, int rotate, double scale, QString tree_edge, bool show_scroll_bars)
{
    QFile *css_file = new QFile(this->conf_obj.value("css-file").toString());
    QString css_file_path =  this->dir_path->filePath(css_file->fileName());
    
    // read the stylesheet
    QString css;
    QFile file;
    file.setFileName(css_file_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    css = file.readAll();
    file.close();
    
    html = html.replace(":DOCUMENTROOT:", "file://" + this->dir_path->path());
    
    // build data and stylesheet to a minimal html-document
    QString html_full = "<!doctype html>"
        "<html lang=\"de\">"
        "<head>"
            "<meta charset=\"utf-8\" />"
            "<style>" + css + "</style>"
        "</head>"
        "<body><div class=\"abs\"><div class=\"cell\">" + html + "</div></div></body>"
        "</html>";
    
    // calculate absolute position
    int pos_x = 0;
    int pos_y = 0;
    
    int par_x = 0;
    int par_y = 0;
    // if parent == id, we have no parent. that means: absolute positioning
    if (parent == id)
    {
        pos_x = int(dx);
        pos_y = int(dy);
    }
    else
    {
        // get parent position
        par_x = int(this->nodes_map[parent]["pos"].toPointF().x());
        par_y = int(this->nodes_map[parent]["pos"].toPointF().y());
        // calculate new absolute position
        pos_x = int(par_x + dx * this->resolution_width);
        pos_y = int(par_y + dy * this->resolution_height);
    }
    
    QWebView *web_view = new QWebView();
    web_view->setHtml(html_full);
    web_view->setFixedSize(this->resolution_width, this->resolution_height);
    //web_view->setZoomFactor(scale);
    web_view->move(pos_x, pos_y);
    
    if (!show_scroll_bars)
    {
        web_view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
        web_view->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    }
    
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
    props["tree-edge"] = tree_edge;
    this->nodes_map[id] = props;
    
    //qDebug() << qvariant_cast<QGraphicsProxyWidget*>(props["widget"]);
    
    if (parent != id)
    {
        drawTreeEdge(par_x, par_y, pos_x, pos_y, id);
    }
}

void QCanvasScene::drawTreeEdge(int par_x, int par_y, int pos_x, int pos_y, int id)
{
    // draw a line from parent to child
    if (this->conf_obj.value("tree-edge-style").toString() == "line" &&
        !(this->nodes_map[id]["tree-edge"].toString() == "none"))
    {
        int dx = par_x - pos_x;
        int dy = par_y - pos_y;
        
        QPoint from, to;
        
        // default from-value
        from = QPoint(par_x + this->resolution_width/2, par_y + this->resolution_height);
        
        // is the child above or below the parent?
        if (dy <= 0)
        {
            to = QPoint(pos_x + this->resolution_width/2, pos_y);
        }
        else
        {
            from = QPoint(par_x + this->resolution_width/2, par_y);
            to = QPoint(pos_x + this->resolution_width/2, pos_y + this->resolution_height);
        }
        
        // is the child more left or right of the parent (and not so much above or below)?
        if (par_y + this->resolution_height >= pos_y &&
            par_y - this->resolution_height <= pos_y)
        {
            if (dx <= 0)
            {
                from = QPoint(par_x + this->resolution_width, par_y + this->resolution_height/2);
                to = QPoint(pos_x, pos_y + this->resolution_height/2);
            }
            else
            {
                from = QPoint(par_x, par_y + this->resolution_height/2);
                to = QPoint(pos_x + this->resolution_width, pos_y + this->resolution_height/2);
            }
        }
        
        drawLine(from,
                 to,
                 this->conf_obj.value("tree-edge-width").toInt(),
                 this->conf_obj.value("tree-edge-color").toString()
                 );
    }
}
