#include "updateitemdialog.h"
#include "ui_updateitemdialog.h"

#include <QFileDialog>
#include <QMessageBox>

UpdateItemDialog::UpdateItemDialog(Item* currentItem, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateItemDialog)
{
    ui->setupUi(this);

    this->currentItem = currentItem;

    if (currentItem != nullptr)
    {
        ui->lbl_item_name->setText(currentItem->getName());
        QPixmap pixmap(currentItem->getImageFilePath());
        ui->lbl_image->setPixmap(pixmap);
        ui->lbl_image->setScaledContents(true);

        ui->sb_quantity->setValue(currentItem->getQuantity());

        imageFilePath = currentItem->getImageFilePath();

    }

    // connections
    connect(ui->btn_confirm_update, &QPushButton::clicked,
            this, &UpdateItemDialog::confirmUpdate);

    connect(ui->btn_load_item_image, &QPushButton::clicked,
            this, &UpdateItemDialog::loadItemImage);

}

UpdateItemDialog::~UpdateItemDialog()
{
    delete ui;
}

void UpdateItemDialog::confirmUpdate()
{
    int quantity = ui->sb_quantity->value();

    if (quantity >= 1)
    {
        currentItem->setQuantity(quantity);
        currentItem->setImageFilePath(imageFilePath);
        this->close();
    }
    else
    {
        QMessageBox mb;
        mb.setText("Quantity must be at least 1.");
        mb.exec();
    }
}

void UpdateItemDialog::loadItemImage()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this,
                        "Open Image", "./",
                        "Image Files (*.png *.jpg)");

    if (filename != "")
    {
        int lastSlash = filename.lastIndexOf("/");
        QString shortName = filename.right(filename.size() - lastSlash - 1);

        QFile::copy(filename, "./images/" + shortName);
        QPixmap pixmap("./images/" + shortName);

        ui->lbl_image->setPixmap(pixmap);
        ui->lbl_image->setScaledContents(true);

        // update internal data
        imageFilePath = "./images/" + shortName;
    }
}



