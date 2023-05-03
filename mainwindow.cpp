#include "mainwindow.h"
#include "newitemdialog.h"
#include "ui_mainwindow.h"
#include "updateitemdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->menu_new_product, &QAction::triggered,
            this, &MainWindow::handleMenuItemNew);

    connect(ui->btn_remove, &QPushButton::clicked,
            this, &MainWindow::removeSelectedProduct);

    connect(ui->lst_products, &QListWidget::itemClicked,
            this, &MainWindow::handleItemClick);

    connect(ui->menu_edit_selected_product, &QAction::triggered,
            this, &MainWindow::handleMenuItemEdit);

    connect(ui->menu_save_products, &QAction::triggered,
            this, &MainWindow::handleSaveItems);

    connect(ui->menu_load_products, &QAction::triggered,
            this, &MainWindow::handleLoadItems);
}

MainWindow::~MainWindow()
{
    // free up product list memory
    for (Item* product : product_list)
    {
        delete product;
    }

    product_list.clear();

    delete ui;
}


void MainWindow::handleMenuItemNew()
{
    Item* newItem = nullptr;
    NewItemDialog newItemDialog(newItem, nullptr);

    newItemDialog.setModal(true);
    newItemDialog.exec();

    if (newItem != nullptr)
    {
        product_list.push_back(newItem);
        ui->lst_products->addItem(newItem->getName());
    }
}


void MainWindow::removeSelectedProduct()
{
    int index = ui->lst_products->currentRow();

    if (index >= 0)
    {
        // remove from vector
        Item* theItem = product_list.at(index);
        delete theItem;
        product_list.removeAt(index);

        delete ui->lst_products->currentItem();
    }

    // remove image after object/item deletion
    QPixmap pixmap("none.png");
    ui->lbl_image->setPixmap(pixmap);
}

void MainWindow::handleItemClick(QListWidgetItem* item)
{
    int index = item->listWidget()->currentRow();

    if (index != -1)
    {
        Item* currentItem = product_list.at(index);

        if (currentItem != nullptr)
        {
            ui->lbl_product_name->setText(currentItem->getName());
            ui->lbl_quantity->setText(QString::number(currentItem->getQuantity()));

            QPixmap pixmap(currentItem->getImageFilePath());

            ui->lbl_image->setPixmap(pixmap);
            ui->lbl_image->setScaledContents(true);
        }
    }
}

void MainWindow::handleMenuItemEdit()
{
    int index = ui->lst_products->currentRow();

    if (index != -1)
    {
        Item* currentItem = product_list.at(index);

        if (currentItem != nullptr)
        {
            UpdateItemDialog updateItemDialog(currentItem, nullptr);
            updateItemDialog.exec();

            // must update UI
            ui->lbl_product_name->setText(currentItem->getName());
            ui->lbl_quantity->setText(QString::number(currentItem->getQuantity()));

            QPixmap pixmap(currentItem->getImageFilePath());
            ui->lbl_image->setPixmap(pixmap);
            ui->lbl_image->setScaledContents(true);
        }
    }
}

void MainWindow::handleSaveItems()
{
    QFile outputFile("products.txt");

    outputFile.open(QIODevice::WriteOnly |
                    QIODevice::Text);

    QTextStream out(&outputFile);

    for(Item* product : product_list)
    {
        out<<product->getName()<<",";
        out<<product->getQuantity()<<",";
        out<<product->getImageFilePath()<<Qt::endl;
    }

    out.flush();
    outputFile.close();
}

void MainWindow::handleLoadItems()
{
    QFile inputFile("products.txt");

    inputFile.open(QIODevice::ReadOnly |
                   QIODevice::Text);

    QTextStream in(&inputFile);

    // clear current list, and then vector
    for (Item* temp : product_list)
    {
        delete temp;
    }

    product_list.clear(); // data model
    ui->lst_products->clear(); // view

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList info = line.split(",");

        // handle list of products in UI
        ui->lst_products->addItem(info.at(0));

        // handle vector
        Item* product = new Item(info.at(0),
                                 info.at(1).toInt(),
                                 info.at(2));

        product_list.push_back(product);
    }

    in.flush();
    inputFile.close();
}








