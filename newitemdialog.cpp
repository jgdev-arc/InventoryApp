#include "newitemdialog.h"
#include "ui_newitemdialog.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

NewItemDialog::NewItemDialog(Item*& newItem, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewItemDialog)
{
    ui->setupUi(this);

    this->newItem = &newItem;
    imageFilePath = "none.png";

    connect(ui->btn_confirm_new_item, &QPushButton::clicked,
            this, &NewItemDialog::confirmAdd);

    connect(ui->btn_load_image, &QPushButton::clicked,
            this, &NewItemDialog::loadItemImage);

    QDir pathDir("./images");
    if (!pathDir.exists())
    {
        QDir().mkdir("./images");
    }


}

NewItemDialog::~NewItemDialog()
{
    delete ui;
}

void NewItemDialog::confirmAdd()
{
    QString productName = ui->txt_product_name->text();
    int quantity = ui->sb_quantity->value();

    if (productName.trimmed() != "" && quantity >= 1)
    {
        *newItem = new Item(productName, quantity, imageFilePath);
        this->close();
    }
    else
    {
        QMessageBox mb;
        mb.setText("Name must be valid and quantity must be at least 1.");
        mb.exec();
    }
}

void NewItemDialog::loadItemImage()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this, "Open Image", "./",
                                            "Image Files (*.png *.jpg)");

    if (filename != "")
    {
        int lastSlash = filename.lastIndexOf("/");

        QString shortName = filename.right(filename.size() - lastSlash - 1);

        QFile::copy(filename, "./images/" + shortName);

        QPixmap pixmap("./images/" + shortName);

        ui->lbl_image->setPixmap(pixmap);
        ui->lbl_image->setScaledContents(true);

        imageFilePath = "./images/" + shortName;
    }
}
