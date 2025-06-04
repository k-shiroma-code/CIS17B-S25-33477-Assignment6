#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QListWidget>
#include <QString>
#include <QMessageBox>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "ui_mainwindow.h"

using namespace std;

class DuplicateItemException : public runtime_error
{
public:
    DuplicateItemException(const string& msg) : runtime_error(msg) {}
};

class ItemNotFoundException : public runtime_error
{
public:
    ItemNotFoundException(const string& msg) : runtime_error(msg) {}
};

class StoredItem
{
private:
    string id;
    string description;
    string location;

public:
    StoredItem(string id, string desc, string loc)
        : id(id), description(desc), location(loc) {}

    string getId() const { return id; }
    string getDescription() const { return description; }
    string getLocation() const { return location; }

    string toString() const
    {
        return id + " - " + description + " at " + location;
    }
};

class StorageManager
{
private:
    unordered_map<string, shared_ptr<StoredItem>> itemById;//lookup with id
    map<string, shared_ptr<StoredItem>> itemByDescription; //order lookup with description
public:
    void addItem(const shared_ptr<StoredItem>& item)
    {
        string id = item->getId();
        if (itemById.count(id))
        {
            throw DuplicateItemException("Item with ID " + id + " already exists!");
        }
        itemById[id] = item;
        itemByDescription[item->getDescription()] = item;
    }

    shared_ptr<StoredItem> findById(const string& id) const
    {
        auto it = itemById.find(id);
        if (it == itemById.end())
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        return it->second;
    }

    void removeItem(const string& id)
    {
        auto it = itemById.find(id);
        if (it == itemById.end())
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        auto desc = it->second->getDescription();
        itemById.erase(it);
        itemByDescription.erase(desc);
    }

    vector<shared_ptr<StoredItem>> listItemsByDescription() const //switched to vector instead of void for list items to return it not print
    {
        vector<shared_ptr<StoredItem>> items;
        for (const auto& pair : itemByDescription)
        {
            items.push_back(pair.second);
        }
        return items;
    }//used for ui on showing everything
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddItemClicked();
    void onFindItemClicked();
    void onRemoveItemClicked();

private:
    QLineEdit *idInput, *descInput, *locInput;
    QLineEdit *findIdInput, *removeIdInput;
    QListWidget *itemList;
    StorageManager manager;
    Ui::MainWindow *ui;//needed this for ui

    void refreshList();
};

#endif

