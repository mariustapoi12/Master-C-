//
// Created by Marius on 3/17/2022.
//

#ifndef A5_6_917TAPOIMARIUS_UI_H
#define A5_6_917TAPOIMARIUS_UI_H

#endif //A5_6_917TAPOIMARIUS_UI_H
#pragma once
#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QShortcut>
#include <QTableView>
#include <QShortcut>
#include <QHeaderView>
#include <QGridLayout>
#include "Service.h"
#include "Duration.h"
#include "UserService.h"
#include "validator.h"

class TutorialTableModel: public QAbstractTableModel {
private:
    UserRepository* repository;
public:
    explicit TutorialTableModel(UserRepository *newRepository);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void update();
};


class GUI: public QWidget{
private:
    Service &serv;
    UserService &user_serv;
    Validator &validator;
    Repository &repo;
    void startGUI();
    QLabel* titleWidget;
    QPushButton* adminButton;
    QPushButton* userButton;
    void displayAdmin();
    void displayUser();
    void connectSignalsAndSlots();

    QRadioButton* csvButton, *htmlButton;
    bool repoTypeSelected;

public:
    explicit GUI(Service &serv, UserService &user_serv, Validator &validator, Repository &repo);
    ~GUI() override;
};

class AdminGUI: public QWidget{
public:
    Service& serv;
    Validator& validator;
    Repository& repository;
    void initAdminGUI();
    QLabel* titleWidget;
    QListWidget* tutorialListWidget;
    QLineEdit* titleLineEdit, *presenterLineEdit, *minLineEdit, *secLineEdit, *nrOfLikesEdit, *linkLineEdit, *filterLineEdit;
    QPushButton* addButton, *deleteButton, *updateButton, *filterButton, *chartButton;
    QPushButton* undoButton, *redoButton;
    QShortcut* shortcutUndo, *shortcutRedo;
    void populateList();
    void filterList();
    void connectSignalsAndSlots();
    int getSelectedIndex() const;
    void addTutorial();
    void deleteTutorial();
    void updateTutorial();
    void displayChart();
    QWidget* chartWindow{};
    void undoGUI();
    void redoGUI();
public:
    explicit AdminGUI(QWidget* parent, Service &serv, Validator &validator, Repository &repo);
    ~AdminGUI() override;
};

class UserGUI: public QWidget {
private:
    Service& serv;
    UserService& user_serv;
    Validator& validator;
    void initUserGUI();
    QLabel* titleWidget;
    QListWidget* tutorialListWidget, *watchListListWidgetr;
    QLineEdit* titleLineEdit, *presenterLineEdit, *minLineEdit, *secLineEdit, *nrOfLikesEdit, *linkLineEdit, *filterLineEdit;
    QPushButton* addButton, *openListButton, *filterButton, *deleteButton;
    QPushButton* undoButton, *redoButton;
    QShortcut* shortcutUndo, *shortcutRedo;

    QTableView* watchListTable;
    TutorialTableModel* watchListTableModel;
    QGridLayout* listAndTableLayout;
    QWidget* tableWindow{};

    void createTable();

    void populateTutorialList();
    void populateWatchList();
    void connectSignalsAndSlots();
    int getSelectedIndex() const;
    int getSelectedIndexWatchList() const;
    void addTutorial();
    void deleteTutorial();
    void filterTutorial();
    void undoGUI();
    void redoGUI();
public:
    explicit UserGUI(QWidget* parent, Service &serv, UserService &user_serv, Validator &validator);
    ~UserGUI() override;
};
