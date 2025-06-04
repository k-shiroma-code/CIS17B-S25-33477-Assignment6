#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect to buttons in window ui
    connect(ui->addItemButton, &QPushButton::clicked, this, &MainWindow::onAddItemClicked);//add item
    connect(ui->findItemButton, &QPushButton::clicked, this, &MainWindow::onFindItemClicked);//find the item
    connect(ui->removeItemButton, &QPushButton::clicked, this, &MainWindow::onRemoveItemClicked);//remove item
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);//menu exit file<exit
    connect(ui->actionAbout, &QAction::triggered, this, [=]()
    {
        QMessageBox::about(this, "About", "Kyle Shiroma\nCIS17B-S25-33477-Assignment6");//about<assignment info
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddItemClicked()//slot(empty space for additem func)
{
    QString id = ui->idInput->text().trimmed();
    QString desc = ui->descInput->text().trimmed();
    QString loc = ui->locInput->text().trimmed();

    if (id.isEmpty() || desc.isEmpty() || loc.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "All fields must be filled.");
        return;
    }

    auto item = make_shared<StoredItem>(id.toStdString(), desc.toStdString(), loc.toStdString());

    try {
        manager.addItem(item);
        refreshList();
        ui->idInput->clear();
        ui->descInput->clear();
        ui->locInput->clear();
    } catch (const DuplicateItemException &e) {
        QMessageBox::warning(this, "Duplicate ID", e.what());
    }
}

void MainWindow::onFindItemClicked()//slot for finditem
{
    QString id = ui->findIdInput->text().trimmed();
    if (id.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Please enter an ID to find.");
        return;
    }

    try {
        auto item = manager.findById(id.toStdString());
        QMessageBox::information(this, "Item Found", QString::fromStdString(item->toString()));
    } catch (const ItemNotFoundException &e) {
        QMessageBox::warning(this, "Not Found", e.what());
    }
}

void MainWindow::onRemoveItemClicked()//slot for remove item
{
    QString id = ui->removeIdInput->text().trimmed();
    if (id.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Please enter an ID to remove.");
        return;
    }

    try {
        manager.removeItem(id.toStdString());
        refreshList();
    } catch (const ItemNotFoundException &e) {
        QMessageBox::warning(this, "Not Found", e.what());
    }
}

void MainWindow::refreshList()
{
    ui->itemList->clear();
    auto items = manager.listItemsByDescription();
    for (const auto& item : items)
    {
        ui->itemList->addItem(QString::fromStdString(item->toString()));
    }
}
