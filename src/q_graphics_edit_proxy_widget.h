#ifndef Q_GRAPHICS_EDIT_PROXY_WIDGET_H
#define Q_GRAPHICS_EDIT_PROXY_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QWebView>
#include <QPushButton>

class QGraphicsEditProxyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QGraphicsEditProxyWidget(QWidget *parent = nullptr);
    
    void setHtmlWidget(QWebView *web);
    void showHandles();
    
    QGridLayout *layout;
signals:
    
public slots:
};

#endif // Q_GRAPHICS_EDIT_PROXY_WIDGET_H
