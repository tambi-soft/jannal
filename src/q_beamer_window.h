#ifndef Q_BEAMER_WINDOW_H
#define Q_BEAMER_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>

#include "q_canvas_widget.h"

class QBeamerWindow : public QDialog
{
public:
    QBeamerWindow(QDialog *parent = nullptr);
    
private:
    QVBoxLayout* layout;
    QCanvasWidget *canvas;
    
protected:
    
};

#endif // Q_BEAMER_WINDOW_H
