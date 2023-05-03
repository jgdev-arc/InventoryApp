#ifndef NEWITEMDIALOG_H
#define NEWITEMDIALOG_H

#include <QDialog>
#include "Item.h"

namespace Ui {
class NewItemDialog;
}

class NewItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewItemDialog(Item*& newItem, QWidget* parent = nullptr);
    ~NewItemDialog();

    void confirmAdd();
    void loadItemImage();

private:
    Ui::NewItemDialog *ui;
    Item** newItem;
    QString imageFilePath;
};

#endif // NEWITEMDIALOG_H
