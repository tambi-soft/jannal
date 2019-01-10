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
    QCanvasWidget *canvas = new QCanvasWidget(filepath, true);
    this->layout->addWidget(canvas);
}

void QBeamerProxy::runPresentation()
{
    QCanvasWidget *canvas = new QCanvasWidget(this->filepath, false);
    canvas->showFullScreen();
}
