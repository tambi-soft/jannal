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
    
    removeEventFilter(this);
}
