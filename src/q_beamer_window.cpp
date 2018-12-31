#include "q_beamer_window.h"

QBeamerWindow::QBeamerWindow(QDialog *parent)
    : QDialog (parent)
    , layout (new QVBoxLayout)
    , canvas (new QCanvasWidget)
{
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(canvas);
    setLayout(layout);
    
    connect(canvas, &QCanvasWidget::deleteBeamerWindow, this, &QBeamerWindow::deleteLater);
    connect(canvas, &QCanvasWidget::reloadCanvas, this, &QBeamerWindow::reloadCanvas);
}

void QBeamerWindow::reloadCanvas()
{
    this->canvas->deleteLater();
    
    this->canvas = new QCanvasWidget;
    this->layout->addWidget(canvas);
}
