#include "q_beamer_proxy.h"

QBeamerProxy::QBeamerProxy(QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    setLayout(this->layout);
    layout->setContentsMargins(0, 0, 0, 0);
}

void QBeamerProxy::initPresentation(QString filepath)
{
    this->filepath = filepath;
    this->canvas_edit = new QCanvasWidget(filepath, true, 0);
    this->layout->addWidget(this->canvas_edit);
}

void QBeamerProxy::runPresentation()
{
    QList<QScreen*> screen_list = QGuiApplication::screens();
    
    QCanvasWidget *canvas = new QCanvasWidget(this->filepath, false, screen_list.length()-1);
    canvas->showFullScreen();
    
    connect(canvas, &QCanvasWidget::currentAnimationStepCoordinates, this, &QBeamerProxy::moveEditorToPosititon);
}

void QBeamerProxy::moveEditorToPosititon(QPoint position, double zoom)
{
    this->canvas_edit->movePresentationMarker(position, zoom);
    this->canvas_edit->scrollToPosition(position, zoom);
}
