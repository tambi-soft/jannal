#ifndef FILL_STORAGE_BOXES_DIALOG_H
#define FILL_STORAGE_BOXES_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>

#include <src/fill_storage_box_widget.h>
#include <src/rest_adapter.h>

class QFillStorageBoxesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QFillStorageBoxesDialog(QString booking_id, int boxes_count, QDialog *parent = nullptr);
    
private:
    QScrollArea *scroll;
    QList<QFillStorageBoxWidget*> widget_list;
    QGridLayout *layout;
    QVBoxLayout *scroll_layout;
    QWidget *scroll_widget;
    
signals:
    
public slots:
    void populate(QVariant var);
    void onSaveButton();
    void onCancelButton();
};

#endif // FILL_STORAGE_BOXES_DIALOG_H
