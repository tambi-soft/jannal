#include "q_beamer_proxy.h"

QBeamerProxy::QBeamerProxy(QString filepath, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->filepath = filepath;
    
    setLayout(this->layout);
    layout->setContentsMargins(0, 0, 0, 0);
    
    this->config = new Config();
    this->config->getSettings()->setValue("jannal/last_open_path", filepath);
}

void QBeamerProxy::initPresentation()
{
    this->canvas_edit = new QCanvasWidget(this->filepath, true, 0);
    this->layout->addWidget(this->canvas_edit);
    
    connect(this->canvas_edit, &QCanvasWidget::reloadCanvas, this, &QBeamerProxy::deletePresentation);
}

void QBeamerProxy::deletePresentation()
{
    this->canvas_edit->deleteLater();
    
    this->initPresentation();
}

void QBeamerProxy::runPresentation()
{
    int screen = 0;
    QString screen_str = this->config->getSettings()->value("jannal/beamer_screen").toString();
    if (screen_str == "auto")
    {
        QList<QScreen*> screen_list = QGuiApplication::screens();
        screen = screen_list.length()-1;
    }
    else
    {
        screen = screen_str.toInt();
    }
    
    QCanvasWidget *canvas = new QCanvasWidget(this->filepath, false, screen);
    canvas->showFullScreen();
    
    connect(canvas, &QCanvasWidget::currentAnimationStepCoordinates, this, &QBeamerProxy::moveEditorToPosititon);
}

void QBeamerProxy::moveEditorToPosititon(QPoint position, double zoom)
{
    this->canvas_edit->movePresentationMarker(position, zoom);
    this->canvas_edit->scrollToPosition(position, zoom);
}
