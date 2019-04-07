#include "q_graphics_edit_proxy_widget.h"

QGraphicsEditProxyWidget::QGraphicsEditProxyWidget(QWidget *parent)
    : QWidget(parent)
    , layout (new QGridLayout)
{
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);
    
    
}

QGraphicsEditProxyWidget::~QGraphicsEditProxyWidget()
{
    this->layout->deleteLater();
}

void QGraphicsEditProxyWidget::setHtmlWidget(QWebView *web)
{
    this->layout->addWidget(web, 0, 0);
}

void QGraphicsEditProxyWidget::showHandles()
{
    QPushButton *button_edit = new QPushButton("edit");
    button_edit->setIcon(QIcon::fromTheme("document-page-setup"));
    button_edit->setMinimumSize(150, 150);
    
    this->layout->addWidget(button_edit, 1, 0);
}
