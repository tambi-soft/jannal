#include "q_canvas_widget.h"

QCanvasWidget::QCanvasWidget(QString filepath, bool edit_mode, int screen_number, QWidget *parent)
    : QWidget(parent)
    , view (new QGraphicsView)
    , scene (new QGraphicsScene)
    , layout (new QVBoxLayout)
    , step_animator (new QStepAnimator)
{
    this->editMode = edit_mode;
    this->screen_number = screen_number;
    
    connect(step_animator, &QStepAnimator::currentAnimationStepCoordinates, this, qOverload<QPoint, double>(&QCanvasWidget::scrollToPosition));
    connect(step_animator, &QStepAnimator::currentAnimationStepZoom, this, &QCanvasWidget::scaleView);
    
    view->setRenderHint(QPainter::Antialiasing);
    
    view->installEventFilter(this);
    scene->installEventFilter(this);
    
    this->resolution_width = 1920;
    this->resolution_height = 1080;
    
    QList<QScreen*> screen_list = QGuiApplication::screens();
    QScreen *screen = screen_list.at(this->screen_number);
    QRect geometry = screen->availableGeometry();
    
    if (!this->editMode)
    {
        move(geometry.left(), geometry.top());
    }
    
    double scale_offset_width = geometry.width() / static_cast<double>(this->resolution_width);
    double scale_offset_height = geometry.height() / static_cast<double>(this->resolution_height);
    if (scale_offset_width > scale_offset_height)
    {
        this->scale_offset = scale_offset_height;
    }
    else
    {
        this->scale_offset = scale_offset_width;
    }
    
    view->setScene(scene);
    
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(view);
    setLayout(layout);
    
    addJSON(filepath);
    
    drawControlls();
    
    if (!this->editMode)
    {
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        
        view->showFullScreen();
    }
    else
    {
        this->view_zoomable = new QZoomableGraphicsView(view);
        this->view_zoomable->set_modifiers(Qt::NoModifier);
        
        view->setDragMode(QGraphicsView::ScrollHandDrag);
        view->scale(0.3, 0.3);
    }
    
    //scrollToPosition(1000, 1000);
    //view->scale(0.1, 0.1);
    //view->scale(1.2, 1.2);
    
    
    //view->fitInView(scene->sceneRect());
    
    //qDebug() << scene->items();
}

void QCanvasWidget::addJSON(QString path)
{
    this->dir_path = new QDir(path);
    this->dir_path->cdUp();
    
    QString json_string;
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    json_string = file.readAll();
    file.close();
    
    QJsonDocument json_document = QJsonDocument::fromJson(json_string.toUtf8());
    
    // get the global json-file configs
    QJsonValue conf_val = json_document["config"];
    this->conf_obj = conf_val.toObject();
    
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
                obj.value("scale").toDouble(),
                obj.value("tree-edge").toString(),
                obj.value("scroll-bars").toBool()
            );
        }
        else if (type == "line")
        {
            drawLine(
                QPoint(obj.value("start-x").toInt(), obj.value("start-y").toInt()),
                QPoint(obj.value("stop-x").toInt(), obj.value("stop-y").toInt()),
                obj.value("width").toInt(),
                obj.value("color").toString()
            );
        }
    }
    
    QColor color;
    color.setNamedColor(this->conf_obj.value("background-color").toString());
    view->setBackgroundBrush(QBrush(color, Qt::SolidPattern));
    
    if (!this->editMode)
    {
        stepToStart();
    }
}

void QCanvasWidget::addHTML(int parent, int id, QString html, double dx, double dy, int rotate, double scale, QString tree_edge, bool show_scroll_bars)
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

void QCanvasWidget::drawTreeEdge(int par_x, int par_y, int pos_x, int pos_y, int id)
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

void QCanvasWidget::drawLine(QPoint from, QPoint to, int width, QString color_str)
{
    QColor color;
    color.setNamedColor(color_str);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(width);
    
    scene->addLine(QLineF(from, to), pen);
}

void QCanvasWidget::drawControlls()
{
    
}

void QCanvasWidget::stepToStart()
{
    // fast-forward to the first step
    this->animation_speed = 1000;
    this->step_active = -1;
    stepForward();
    this->animation_speed = 50;
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
        
        // maybe there is an offset defined
        double offset_x = obj.value("pos-x").toDouble();
        double offset_y = obj.value("pos-y").toDouble();
        
        pos_to.setX(pos_to.x() + offset_x * this->resolution_width);
        pos_to.setY(pos_to.y() + offset_y * this->resolution_height);
    }
    else if (type == "position")
    {
        pos_to.setX(obj.value("pos-x").toInt());
        pos_to.setY(obj.value("pos-y").toInt());
    }
    
    QPoint pos_from;
    pos_from.setX(int(view->sceneRect().x()));
    pos_from.setY(int(view->sceneRect().y()));
    
    step_animator->quit();
    step_animator->halt();
    step_animator->setSpeed(this->animation_speed);
    step_animator->setCoordinates(pos_from, QPoint(int(pos_to.x()), int(pos_to.y())));
    step_animator->setZoom(this->scale_factor, obj.value("zoom").toDouble());
    step_animator->start();
}

void QCanvasWidget::scaleView(double factor)
{
    this->scale_factor = factor;
    
    this->view->resetMatrix();
    view->scale(this->scale_offset * factor, this->scale_offset * factor);
}

void QCanvasWidget::scrollToPosition(int x, int y, double zoom)
{
    view->setSceneRect(x, y, this->resolution_width, this->resolution_height);
    emit currentAnimationStepCoordinates(QPoint(x, y), zoom);
}
void QCanvasWidget::scrollToPosition(QPointF pos, double zoom)
{
    scrollToPosition(int(pos.x()), int(pos.y()), zoom);
}
void QCanvasWidget::scrollToPosition(QPoint pos, double zoom)
{
    scrollToPosition(pos.x(), pos.y(), zoom);
}

void QCanvasWidget::movePresentationMarker(QPoint position, double scale)
{
    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QBrush transparentBrush(Qt::transparent);
    QPen outlinePen(Qt::red);
    outlinePen.setWidth(50);
    
    scene->removeItem(this->stepMarker);
    delete this->stepMarker;
    
    double marker_width = 1 / scale * this->resolution_width;
    double marker_height = 1 / scale * this->resolution_height;
    this->stepMarker = scene->addRect(
                position.x() - ((1-scale) * (marker_width / 2)),
                position.y() - ((1-scale) * (marker_height / 2)),
                marker_width,
                marker_height,
                outlinePen,
                transparentBrush
                );
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
            case Qt::Key_Up:
            {
                stepBackward();
                return true;
            }
            case Qt::Key_Down:
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

void QCanvasWidget::showEvent(QShowEvent *)
{
    if (this->editMode)
    {
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        //scene->setSceneRect(scene->sceneRect());
    }
}
