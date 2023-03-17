//
// Created by Marius on 3/17/2022.
//

#include <QVBoxLayout>
#include <QFormLayout>
#include <QErrorMessage>
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "GUI.h"

TutorialTableModel::TutorialTableModel(UserRepository *newRepository)
{
    this->repository = newRepository;
}

int TutorialTableModel::rowCount(const QModelIndex &parent) const {
    this->repository->getNrElems();
}
int TutorialTableModel::columnCount(const QModelIndex &parent) const {
    return 6;
}

QVariant TutorialTableModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    Tutorial currentTutorial = this->repository->getAllUserRepo()[row];
    int column = index.column();
    if (role == Qt::DisplayRole || role == Qt::EditRole){
        switch (column) {
            case 0:
                return QString::fromStdString(currentTutorial.get_title());
            case 1:
                return QString::fromStdString(currentTutorial.get_presenter());
            case 2:
                return QString::fromStdString(std::to_string(currentTutorial.get_duration().get_minutes()));
            case 3:
                return QString::fromStdString(std::to_string(currentTutorial.get_duration().get_seconds()));
            case 4:
                return QString::fromStdString(std::to_string(currentTutorial.get_nr_of_likes()));
            case 5:
                return QString::fromStdString(currentTutorial.get_link());
            default:
                break;
        }
    }
    return QVariant();
}

QVariant TutorialTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return QString("Title");
                case 1:
                    return QString("Presenter");
                case 2:
                    return QString("Minutes");
                case 3:
                    return QString("Seconds");
                case 4:
                    return QString("Nr. of likes");
                case 5:
                    return QString("Link");
                default:
                    break;
            }
        }
    }
    return QVariant();
}

void TutorialTableModel::update() {
    QModelIndex topLeft = this->index(1,1);
    QModelIndex bottomRight = this->index(this->rowCount(), this->columnCount());
    emit layoutChanged();
    emit dataChanged(topLeft, bottomRight);
}

GUI::GUI(Service &serv, UserService &user_serv, Validator &validator, Repository &repo): serv{serv}, user_serv{user_serv}, validator{validator}, repo{repo}{
    this->titleWidget = new QLabel(this);
    this->adminButton = new QPushButton(this);
    this->userButton = new QPushButton(this);
    this->csvButton = new QRadioButton("CSV");
    this->htmlButton = new QRadioButton("HTML");
    this->repoTypeSelected = false;
    this->startGUI();
    this->connectSignalsAndSlots();
}

void GUI::startGUI() {
    auto* layout = new QVBoxLayout(this);
    QFont titleFont = this->titleWidget->font();
    this->titleWidget->setText("<p style='text-align:center'><font color=#4D2D52>Welcome to the C++ Tutorial App! <br> Select your mode!</font></p>");
    titleFont.setItalic(true);
    titleFont.setPointSize(10);
    titleFont.setStyleHint(QFont::System);
    titleFont.setWeight(QFont::DemiBold);
    this->titleWidget->setFont(titleFont);
    layout->addWidget(this->titleWidget);
    this->adminButton->setText("Admin mode");
    this->adminButton->setStyleSheet(QString("background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 white, stop: 0.4 "
                                    "gray, stop:1 green)"));
    auto* radioButtonsLayout = new QGridLayout(this);
    radioButtonsLayout->addWidget(this->csvButton, 0, 0);
    radioButtonsLayout->addWidget(this->htmlButton, 1, 0);
    layout->addLayout(radioButtonsLayout);
    layout->addWidget(this->adminButton);
    this->userButton->setText("User mode");
    layout->addWidget(this->userButton);
    this->setLayout(layout);
    this->setStyleSheet("background-color:#D9DBF1");
}

GUI::~GUI() = default;

void GUI::connectSignalsAndSlots() {
    QObject::connect(this->csvButton, &QRadioButton::clicked, [this]() {
        this->user_serv.repositoryType("csv");
        this->repoTypeSelected = true;
    });
    QObject::connect(this->htmlButton, &QRadioButton::clicked, [this]() {
        this->user_serv.repositoryType("html");
        this->repoTypeSelected = true;
    });

    QObject::connect(this->adminButton, &QPushButton::clicked, this, &GUI::displayAdmin);
    QObject::connect(this->userButton, &QPushButton::clicked, [this](){
        if (!this->repoTypeSelected) {
            auto* error = new QMessageBox();
            error->setIcon(QMessageBox::Warning);
            error->setText("Please select the type of file you want!");
            error->setWindowTitle("File type warning!");
            error->exec();
        } else {
            displayUser();
        }
    });
}

void GUI::displayAdmin() {
    this->serv.clearUndoRedo();
    auto* admin = new AdminGUI(this, this->serv, this->validator, this->repo);
    admin->show();
}

AdminGUI::AdminGUI(QWidget* parent, Service &serv, Validator& validator, Repository& repo): serv{serv}, validator{validator}, repository{repo}{
    this->titleWidget = new QLabel(this);
    this->tutorialListWidget = new QListWidget{};
    this->titleLineEdit = new QLineEdit{};
    this->presenterLineEdit = new QLineEdit{};
    this->minLineEdit = new QLineEdit{};
    this->secLineEdit = new QLineEdit{};
    this->nrOfLikesEdit = new QLineEdit{};
    this->linkLineEdit = new QLineEdit{};
    this->filterLineEdit = new QLineEdit{};
    this->addButton = new QPushButton("Add");
    this->deleteButton = new QPushButton("Delete");
    this->updateButton = new QPushButton("Update");
    this->filterButton = new QPushButton("Filter database");
    this->chartButton = new QPushButton("Display chart");
    this->undoButton = new QPushButton("Undo");
    this->redoButton = new QPushButton("Redo");
    this->shortcutUndo = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
    this->shortcutRedo = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this);
    setParent(parent);
    setWindowFlag(Qt::Window);
    this->initAdminGUI();
    this->populateList();
    this->connectSignalsAndSlots();
}

void AdminGUI::initAdminGUI() {
    auto* layout = new QVBoxLayout(this);
    QFont titleFont = this->titleWidget->font();
    this->titleWidget->setText("<p style='text-align:center'><font color=#4D2D52>ADMIN MODE</font></p>");
    titleFont.setItalic(true);
    titleFont.setPointSize(10);
    titleFont.setStyleHint(QFont::System);
    titleFont.setWeight(QFont::DemiBold);
    this->titleWidget->setFont(titleFont);
    layout->addWidget(this->titleWidget);
    layout->addWidget(this->tutorialListWidget);

    auto * tutorialDetailsLayout = new QFormLayout{};
    tutorialDetailsLayout->addRow("Title:", this->titleLineEdit);
    tutorialDetailsLayout->addRow("Presenter:", this->presenterLineEdit);
    tutorialDetailsLayout->addRow("Minutes:", this->minLineEdit);
    tutorialDetailsLayout->addRow("Seconds:", this->secLineEdit);
    tutorialDetailsLayout->addRow("Nr. of likes: ", this->nrOfLikesEdit);
    tutorialDetailsLayout->addRow("Link: ", this->linkLineEdit);
    layout->addLayout(tutorialDetailsLayout);

    auto* buttonsLayout = new QGridLayout{};
    buttonsLayout->addWidget(this->addButton, 0, 0);
    buttonsLayout->addWidget(this->deleteButton, 0, 1);
    buttonsLayout->addWidget(this->updateButton, 1, 0);
    buttonsLayout->addWidget(this->chartButton, 1, 1);
    buttonsLayout->addWidget(this->undoButton, 2, 0);
    buttonsLayout->addWidget(this->redoButton, 2, 1);
    layout->addLayout(buttonsLayout);

    auto *filterTitle = new QLabel("<p style='text-align:center'><font color=#4D2D52><br>Filter the tutorials by presenter</font></p>");
    QFont filterFont = filterTitle->font();
    filterFont.setPointSize(10);
    filterFont.setStyleHint(QFont::System);
    filterFont.setWeight(QFont::DemiBold);
    filterTitle->setFont(filterFont);
    layout->addWidget(filterTitle);

    auto *  filterDetailsLayout = new QFormLayout{};
    filterDetailsLayout->addRow("Presenter: ", this->filterLineEdit);
    filterDetailsLayout->addRow(this->filterButton);
    layout->addLayout(filterDetailsLayout);
}

void AdminGUI::connectSignalsAndSlots() {
    QObject::connect(this->filterButton, &QPushButton::clicked, this, &AdminGUI::filterList);
    QObject::connect(this->tutorialListWidget, &QListWidget::itemSelectionChanged, [this]() {
        int selectedIndex = this->getSelectedIndex();
        if (selectedIndex < 0)
            return ;
        Tutorial tutorial = this->serv.getAllService()[selectedIndex];
        int min, sec;
        std::string min_s, sec_s;
        min = tutorial.get_duration().get_minutes();
        sec = tutorial.get_duration().get_seconds();
        min_s = std::to_string(min);
        sec_s = std::to_string(sec);
        this->titleLineEdit->setText(QString::fromStdString(tutorial.get_title()));
        this->presenterLineEdit->setText(QString::fromStdString(tutorial.get_presenter()));
        this->minLineEdit->setText(QString::fromStdString(min_s));
        this->secLineEdit->setText(QString::fromStdString(sec_s));
        this->nrOfLikesEdit->setText(QString::fromStdString(std::to_string(tutorial.get_nr_of_likes())));
        this->linkLineEdit->setText(QString::fromStdString(tutorial.get_link()));
    });
    QObject::connect(this->addButton, &QPushButton::clicked, this, &AdminGUI::addTutorial);
    QObject::connect(this->deleteButton, &QPushButton::clicked, this, &AdminGUI::deleteTutorial);
    QObject::connect(this->updateButton, &QPushButton::clicked, this, &AdminGUI::updateTutorial);
    QObject::connect(this->chartButton, &QPushButton::clicked, this, &AdminGUI::displayChart);
    QObject::connect(this->undoButton, &QPushButton::clicked, this, &AdminGUI::undoGUI);
    QObject::connect(this->redoButton, &QPushButton::clicked, this, &AdminGUI::redoGUI);

    QObject::connect(this->shortcutUndo, &QShortcut::activated, this, &AdminGUI::undoGUI);
    QObject::connect(this->shortcutRedo, &QShortcut::activated, this, &AdminGUI::redoGUI);
}

void AdminGUI::populateList(){
    this->tutorialListWidget->clear();
    std::vector<Tutorial> Tutorials = this->serv.getAllService();
    for (Tutorial& tutorial: Tutorials) {
        std::string populate_string = tutorial.get_title() + " | " + tutorial.get_presenter();
        this->tutorialListWidget->addItem(QString::fromStdString(populate_string));
    }
}

void AdminGUI::addTutorial() {
    std::string title = this->titleLineEdit->text().toStdString();
    std::string presenter = this->presenterLineEdit->text().toStdString();
    std::string min_s = this->minLineEdit->text().toStdString();
    std::string sec_s = this->secLineEdit->text().toStdString();
    std::string nr_of_likes_s = this->nrOfLikesEdit->text().toStdString();
    std::string link = this->linkLineEdit->text().toStdString();
    int min, sec, nr_of_likes;
    try {
        this->validator.ValidateInputStrings(title);
        this->validator.ValidateInputStrings(presenter);
        this->validator.ValidateInputNumbers(min_s);
        this->validator.ValidateInputNumbers(sec_s);
        this->validator.ValidateInputNumbers(nr_of_likes_s);
        this->validator.ValidateInputStrings(link);
        this->validator.ValidateLink(link);
        min = stoi(min_s);
        this->validator.ValidateMinutes(min);
        sec = stoi(sec_s);
        this->validator.ValidateSeconds(sec);
        nr_of_likes = stoi(nr_of_likes_s);
        this->validator.ValidateNrOfLikes(nr_of_likes);
        auto duration = Duration(min, sec);
        this->serv.addTutorial(title, presenter, duration, nr_of_likes, link);
        this->populateList();
    } catch (ValidationException& exc) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(exc.what());
        error->setWindowTitle("Invalid input!");
        error->exec();
    } catch (RepositoryException& re) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(re.what());
        error->setWindowTitle("Error at adding a tutorial!");
        error->exec();
    }
}

void AdminGUI::deleteTutorial() {
    try {
        std::string link = this->linkLineEdit->text().toStdString();
        this->validator.ValidateInputStrings(link);
        this->validator.ValidateLink(link);
        this->validator.ValidateLink(link);
        this->serv.deleteTutorial(link);
        this->populateList();
    } catch (ValidationException& exc) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(exc.what());
        error->setWindowTitle("Invalid input!");
        error->exec();
    } catch (RepositoryException& re) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(re.what());
        error->setWindowTitle("Error at deleting a tutorial!");
        error->exec();
    }
}

void AdminGUI::updateTutorial() {
    int index = this->getSelectedIndex();
    try {
        if (index < 0) {
            auto* error = new QMessageBox();
            error->setIcon(QMessageBox::Critical);
            error->setText("No tutorial selected!");
            error->setWindowTitle("Selection error!");
            error->exec();
        } else {
            std::string old_link = this->serv.getAllService()[index].get_link();
            std::string new_title = this->titleLineEdit->text().toStdString();
            std::string new_presenter = this->presenterLineEdit->text().toStdString();
            std::string new_min_s = this->minLineEdit->text().toStdString();
            std::string new_sec_s = this->secLineEdit->text().toStdString();
            std::string new_nr_of_likes_s = this->nrOfLikesEdit->text().toStdString();
            std::string new_link = this->linkLineEdit->text().toStdString();
            int new_min, new_sec, new_nr_of_likes;
            this->validator.ValidateInputStrings(new_title);
            this->validator.ValidateInputStrings(new_presenter);
            this->validator.ValidateInputNumbers(new_min_s);
            this->validator.ValidateInputNumbers(new_sec_s);
            this->validator.ValidateInputNumbers(new_nr_of_likes_s);
            this->validator.ValidateInputStrings(new_link);
            this->validator.ValidateLink(new_link);
            new_min = stoi(new_min_s);
            this->validator.ValidateMinutes(new_min);
            new_sec = stoi(new_sec_s);
            this->validator.ValidateSeconds(new_sec);
            new_nr_of_likes = stoi( new_nr_of_likes_s);
            this->validator.ValidateNrOfLikes(new_nr_of_likes);
            auto new_duration = Duration(new_min, new_sec);
            this->serv.updateTutorial(old_link, new_title, new_presenter, new_duration, new_nr_of_likes, new_link);
            this->populateList();
        }
    } catch (ValidationException& exc) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(exc.what());
        error->setWindowTitle("Invalid input!");
        error->exec();
    } catch (RepositoryException& re) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(re.what());
        error->setWindowTitle("Error at deleting a tutorial!");
        error->exec();
    }
}

int AdminGUI::getSelectedIndex() const {
    QModelIndexList selectedIndexes = this->tutorialListWidget->selectionModel()->selectedIndexes();
    if (selectedIndexes.empty()) {
        this->titleLineEdit->clear();
        this->presenterLineEdit->clear();
        this->minLineEdit->clear();
        this->secLineEdit->clear();
        this->nrOfLikesEdit->clear();
        this->linkLineEdit->clear();
        return -1;
    }
    int selectedIndex = selectedIndexes.at(0).row();
    return selectedIndex;
}

void AdminGUI::filterList() {
    this->tutorialListWidget->clear();
    std::vector<Tutorial> Tutorials = this->serv.getAllService();

    std::string filteredPresenter = this->filterLineEdit->text().toStdString();
    for (Tutorial& tutorial: Tutorials)
        if (filteredPresenter.empty())
            this->populateList();
        else if(tutorial.get_presenter() == filteredPresenter) {
            std::string populate_string = tutorial.get_title() + " | " + tutorial.get_presenter();
            this->tutorialListWidget->addItem(QString::fromStdString(populate_string));
        }
}

void AdminGUI::displayChart() {
//    this->chartWindow = new QWidget{};
//    auto* chartLayout = new QVBoxLayout(this->chartWindow);
//    this->chartWindow->setStyleSheet("background-color:#D9DBF1");
//    std::vector<Tutorial> presenters = this->serv.getAllService();
//    auto* chart = new QChart();
//    auto *axis_x = new QBarCategoryAxis();
//    axis_x->setTitleText("Presenters");
//
//    auto* axis_y = new QValueAxis();
//    chart->addAxis(axis_y, Qt::AlignLeft);
//    axis_y->setRange(0, 8);
//    axis_y->setTitleText("Number of presenters");
//
//    std::vector<std::string> used_presenters;
//    used_presenters.reserve(10);
//
//    for (auto& presenter: presenters) {
//        auto *series = new QBarSeries();
//        std::string presenter_s = presenter.get_presenter();
//        if(std::find(used_presenters.begin(), used_presenters.end(), presenter_s)==used_presenters.end())
//        {
//            used_presenters.push_back(presenter_s);
//            auto* set = new QBarSet(QString::fromStdString(presenter_s));
//            int number_of_videos = this->serv.numberOfVidsPerPresenter(presenter_s);
//            *set << number_of_videos;
//            series->append(set);
//            chart->addSeries(series);
//            series->attachAxis(axis_y);
//        }
//    }
//    QStringList categories;
//    for (int i = 0; i < used_presenters.size(); i++) {
//        categories << QString::fromStdString(std::to_string((i+1)));
//    }
//
//    axis_x->append(categories);
//    chart->addAxis(axis_x, Qt::AlignBottom);
//
//    chart->setTitle("Number of videos per presenters");
//    chart->setAnimationOptions(QChart::QChart::SeriesAnimations);
//
//    chart->legend()->setVisible(true);
//    chart->legend()->setAlignment(Qt::AlignLeft);
//    chart->legend()->setBackgroundVisible(true);
//    chart->legend()->setBorderColor(QColor::fromRgb(171, 147, 225));
//    chart->legend()->setFont(QFont("Cambria Math", 7));
//
//    auto* chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);
//
//    chartLayout->addWidget(chartView);
//    this->chartWindow->resize(840, 720);
//    this->chartWindow->show();
}

void AdminGUI::undoGUI() {
    try {
        this->serv.undoLastAction();
        this->populateList();
    } catch (RepositoryException& re) {
        QMessageBox::critical(this, "Error", re.what());
    }
}

void AdminGUI::redoGUI() {
    try {
        this->serv.redoLastAction();
        this->populateList();
    } catch (RepositoryException& re) {
        QMessageBox::critical(this, "Error", re.what());
    }
}

AdminGUI::~AdminGUI() = default;


void GUI::displayUser() {
    this->user_serv.clearUndoRedo();
    auto* user = new UserGUI(this, this->serv, this->user_serv, this->validator);
    user->show();
}

UserGUI::UserGUI(QWidget* parent, Service& serv, UserService &user_serv, Validator& validator): serv{serv}, user_serv{user_serv}, validator{validator} {
    this->titleWidget = new QLabel(this);
    this->tutorialListWidget = new QListWidget{};
    this->watchListListWidgetr = new QListWidget{};
    this->tutorialListWidget = new QListWidget{};
    this->titleLineEdit = new QLineEdit{};
    this->presenterLineEdit = new QLineEdit{};
    this->minLineEdit = new QLineEdit{};
    this->secLineEdit = new QLineEdit{};
    this->nrOfLikesEdit = new QLineEdit{};
    this->linkLineEdit = new QLineEdit{};
    this->filterLineEdit = new QLineEdit{};
    this->addButton = new QPushButton("Add to watch list");
    this->openListButton = new QPushButton("Open file");
    this->filterButton = new QPushButton("Filter (slideshow)");
    this->deleteButton = new QPushButton("Delete from watch list");
    this->undoButton = new QPushButton("Undo");
    this->redoButton = new QPushButton("Redo");
    this->shortcutUndo = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_U), this);
    this->shortcutRedo = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this);
    setParent(parent);
    setWindowFlag(Qt::Window);
    this->initUserGUI();
    this->populateTutorialList();
    this->connectSignalsAndSlots();
}

void UserGUI::initUserGUI() {
    auto* layout = new QVBoxLayout(this);
    QFont titleFont = this->titleWidget->font();
    this->titleWidget->setText("<p style='text-align:center'><font color=#4D2D52>USER MODE </font></p>");
    titleFont.setItalic(true);
    titleFont.setPointSize(10);
    titleFont.setStyleHint(QFont::System);
    titleFont.setWeight(QFont::DemiBold);
    this->titleWidget->setFont(titleFont);
    layout->addWidget(this->titleWidget);

    auto* listLayout = new QGridLayout(this);
    listLayout->addWidget(this->openListButton);
    listLayout->addWidget(this->tutorialListWidget, 0, 0);
    listLayout->addWidget(this->watchListListWidgetr, 0, 1);
    layout->addLayout(listLayout);

    this->tableWindow = new QWidget{};
    this->listAndTableLayout = new QGridLayout(this->tableWindow);
    //this->listAndTableLayout->addWidget(this->watchListListWidgetr);
    this->tableWindow->show();

    auto * tutorialDetailsLayout = new QFormLayout{};
    tutorialDetailsLayout->addRow("Title:", this->titleLineEdit);
    tutorialDetailsLayout->addRow("Presenter:", this->presenterLineEdit);
    tutorialDetailsLayout->addRow("Minutes:", this->minLineEdit);
    tutorialDetailsLayout->addRow("Seconds:", this->secLineEdit);
    tutorialDetailsLayout->addRow("Nr. of likes: ", this->nrOfLikesEdit);
    tutorialDetailsLayout->addRow("Link: ", this->linkLineEdit);
    tutorialDetailsLayout->addRow(this->addButton);
    tutorialDetailsLayout->addRow(this->deleteButton);
    tutorialDetailsLayout->addRow(this->openListButton);
    layout->addLayout(tutorialDetailsLayout);

    auto* undoRedoLayout = new QGridLayout(this);
    undoRedoLayout->addWidget(this->undoButton, 0, 0);
    undoRedoLayout->addWidget(this->redoButton, 0, 1);
    layout->addLayout(undoRedoLayout);


    auto* filterLayout = new QFormLayout{};
    filterLayout->addRow("Presenter:", this->filterLineEdit);
    filterLayout->addWidget(this->filterButton);
    layout->addLayout(filterLayout);
}

void UserGUI::createTable() {
    this->watchListTableModel = new TutorialTableModel{this->user_serv.getUserRepo()};
    this->watchListTable = new QTableView{};
    this->watchListTable->setModel(this->watchListTableModel);
    //this->watchListTable->setItemDelegate(new PictureDelegate{});
    this->listAndTableLayout->addWidget(this->watchListTable, 0, 1);
    this->watchListTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->resize(900, 500);
}

void UserGUI::populateTutorialList() {
    this->tutorialListWidget->clear();
    std::vector<Tutorial> Tutorials = this->serv.getAllService();
    for (Tutorial &tutorial: Tutorials) {
        std::string populate_string = tutorial.get_title() + " | " + tutorial.get_presenter();
        this->tutorialListWidget->addItem(QString::fromStdString(populate_string));
    }
}

int UserGUI::getSelectedIndex() const {
    QModelIndexList selectedIndexes = this->tutorialListWidget->selectionModel()->selectedIndexes();
    if (selectedIndexes.empty()) {
        this->titleLineEdit->clear();
        this->presenterLineEdit->clear();
        this->minLineEdit->clear();
        this->secLineEdit->clear();
        this->nrOfLikesEdit->clear();
        this->linkLineEdit->clear();
        return -1;
    }
    int selectedIndex = selectedIndexes.at(0).row();
    return selectedIndex;
}

int UserGUI::getSelectedIndexWatchList() const {
    QModelIndexList selectedIndexes = this->watchListListWidgetr->selectionModel()->selectedIndexes();
    if (selectedIndexes.empty()) {
        this->titleLineEdit->clear();
        this->presenterLineEdit->clear();
        this->minLineEdit->clear();
        this->secLineEdit->clear();
        this->nrOfLikesEdit->clear();
        this->linkLineEdit->clear();
        return -1;
    }
    int selectedIndex = selectedIndexes.at(0).row();
    return selectedIndex;
}

void UserGUI::populateWatchList() {
    this->watchListListWidgetr->clear();
    std::vector<Tutorial> Tutorials = this->user_serv.getAllUserService();
    for (Tutorial &tutorial: Tutorials) {
        std::string populate_string = tutorial.get_title() + " | " + tutorial.get_presenter();
        this->watchListListWidgetr->addItem(QString::fromStdString(populate_string));
    }
    this->watchListTableModel->update();
    this->watchListTable->resizeRowsToContents();
    this->watchListTable->resizeColumnsToContents();
}

void UserGUI::connectSignalsAndSlots() {
    QObject::connect(this->tutorialListWidget, &QListWidget::itemClicked, [this]() {
        int selectedIndex = getSelectedIndex();
        Tutorial tutorial = this->serv.getAllService()[selectedIndex];
        int min, sec;
        std::string min_s, sec_s;
        min = tutorial.get_duration().get_minutes();
        sec = tutorial.get_duration().get_seconds();
        min_s = std::to_string(min);
        sec_s = std::to_string(sec);
        this->titleLineEdit->setText(QString::fromStdString(tutorial.get_title()));
        this->presenterLineEdit->setText(QString::fromStdString(tutorial.get_presenter()));
        this->minLineEdit->setText(QString::fromStdString(min_s));
        this->secLineEdit->setText(QString::fromStdString(sec_s));
        this->nrOfLikesEdit->setText(QString::fromStdString(std::to_string(tutorial.get_nr_of_likes())));
        this->linkLineEdit->setText(QString::fromStdString(tutorial.get_link()));
        tutorial.play();
    });

    QObject::connect(this->openListButton, &QPushButton::clicked, [this]() {
            std::string link = std::string("start ").append(this->user_serv.getFileService());
            system(link.c_str());
    });

    QObject::connect(this->addButton, &QPushButton::clicked, this, &UserGUI::addTutorial);
    QObject::connect(this->watchListListWidgetr, &QListWidget::itemSelectionChanged, [this]() {
        int selectedIndex = this->getSelectedIndexWatchList();
        if (selectedIndex < 0)
            return ;
        Tutorial tutorial = this->user_serv.getAllUserService()[selectedIndex];
        int min, sec;
        std::string min_s, sec_s;
        min = tutorial.get_duration().get_minutes();
        sec = tutorial.get_duration().get_seconds();
        min_s = std::to_string(min);
        sec_s = std::to_string(sec);
        this->titleLineEdit->setText(QString::fromStdString(tutorial.get_title()));
        this->presenterLineEdit->setText(QString::fromStdString(tutorial.get_presenter()));
        this->minLineEdit->setText(QString::fromStdString(min_s));
        this->secLineEdit->setText(QString::fromStdString(sec_s));
        this->nrOfLikesEdit->setText(QString::fromStdString(std::to_string(tutorial.get_nr_of_likes())));
        this->linkLineEdit->setText(QString::fromStdString(tutorial.get_link()));
    });
    QObject::connect(this->deleteButton, &QPushButton::clicked, this, &UserGUI::deleteTutorial);
    QObject::connect(this->filterButton, &QPushButton::clicked, this, &UserGUI::filterTutorial);
    QObject::connect(this->undoButton, &QPushButton::clicked, this, &UserGUI::undoGUI);
    QObject::connect(this->redoButton, &QPushButton::clicked, this, &UserGUI::redoGUI);


    QObject::connect(this->shortcutUndo, &QShortcut::activated, this, &UserGUI::undoGUI);
    QObject::connect(this->shortcutRedo, &QShortcut::activated, this, &UserGUI::redoGUI);

    this->createTable();
}

void UserGUI::addTutorial()
{
    std::string title = this->titleLineEdit->text().toStdString();
    std::string presenter = this->presenterLineEdit->text().toStdString();
    std::string min_s = this->minLineEdit->text().toStdString();
    std::string sec_s = this->secLineEdit->text().toStdString();
    std::string nr_of_likes_s = this->nrOfLikesEdit->text().toStdString();
    std::string link = this->linkLineEdit->text().toStdString();
    int min, sec, nr_of_likes;
    try {
        this->validator.ValidateInputStrings(title);
        this->validator.ValidateInputStrings(presenter);
        this->validator.ValidateInputNumbers(min_s);
        this->validator.ValidateInputNumbers(sec_s);
        this->validator.ValidateInputNumbers(nr_of_likes_s);
        this->validator.ValidateInputStrings(link);
        this->validator.ValidateLink(link);
        min = stoi(min_s);
        this->validator.ValidateMinutes(min);
        sec = stoi(sec_s);
        this->validator.ValidateSeconds(sec);
        nr_of_likes = stoi(nr_of_likes_s);
        this->validator.ValidateNrOfLikes(nr_of_likes);
        auto duration = Duration(min, sec);
        Tutorial tutorial = Tutorial(title, presenter, duration, nr_of_likes, link);
        this->user_serv.addUserService(tutorial);
        //this->watchListListWidgetr->addItem(this->tutorialListWidget->takeItem(this->getSelectedIndex()));
        this->populateWatchList();
        this->populateTutorialList();
    } catch (ValidationException& exc) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(exc.what());
        error->setWindowTitle("Invalid input!");
        error->exec();
    } catch (RepositoryException& re) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(re.what());
        error->setWindowTitle("Error at adding a tutorial!");
        error->exec();
    }
}

void UserGUI::deleteTutorial()
{
    std::string title = this->titleLineEdit->text().toStdString();
    std::string presenter = this->presenterLineEdit->text().toStdString();
    std::string min_s = this->minLineEdit->text().toStdString();
    std::string sec_s = this->secLineEdit->text().toStdString();
    std::string nr_of_likes_s = this->nrOfLikesEdit->text().toStdString();
    std::string link = this->linkLineEdit->text().toStdString();
    int min, sec, nr_of_likes;
    try {
        this->validator.ValidateInputStrings(title);
        this->validator.ValidateInputStrings(presenter);
        this->validator.ValidateInputNumbers(min_s);
        this->validator.ValidateInputNumbers(sec_s);
        this->validator.ValidateInputNumbers(nr_of_likes_s);
        this->validator.ValidateInputStrings(link);
        this->validator.ValidateLink(link);
        min = stoi(min_s);
        this->validator.ValidateMinutes(min);
        sec = stoi(sec_s);
        this->validator.ValidateSeconds(sec);
        nr_of_likes = stoi(nr_of_likes_s);
        this->validator.ValidateNrOfLikes(nr_of_likes);
        auto duration = Duration(min, sec);
        auto *question = new QMessageBox;
        question->setWindowTitle("Like?");
        question->setText("Did you like the tutorial?");
        QAbstractButton *myYesButton = question->addButton(QMessageBox::Yes);
        QAbstractButton *myNoButton = question->addButton(QMessageBox::No);
        question->setIcon(QMessageBox::Question);
        question->exec();
        if (question->clickedButton() == myYesButton) {
            nr_of_likes++;
        }
        question->close();
        Tutorial tutorial = Tutorial(title, presenter, duration, nr_of_likes, link);
        int selectedIndex = this->getSelectedIndexWatchList();
        this->user_serv.deleteUserService(tutorial, selectedIndex);
        this->populateWatchList();
        this->populateTutorialList();
    } catch (ValidationException& exc) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(exc.what());
        error->setWindowTitle("Invalid input!");
        error->exec();
    } catch (RepositoryException& re) {
        auto* error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(re.what());
        error->setWindowTitle("Error at adding a tutorial!");
        error->exec();
    }
}

void UserGUI::filterTutorial()
{
    bool done = false;
    std::vector<int> positions;
    int cnt = -1, cnt2=0;
    try {
        populateTutorialList();
        std::string presenter_filter = this->filterLineEdit->text().toStdString();

          if (presenter_filter.empty()) {
            std::vector<Tutorial> Tutorials = this->serv.getAllService();
            while (!Tutorials.empty() && !done) {
                for (Tutorial &tutorial: Tutorials) {
                    cnt++;
                    std::string populate_string = tutorial.get_title() + " | " + tutorial.get_presenter();
                    tutorial.play();
                    int min, sec;
                    std::string min_s, sec_s;
                    min = tutorial.get_duration().get_minutes();
                    sec = tutorial.get_duration().get_seconds();
                    min_s = std::to_string(min);
                    sec_s = std::to_string(sec);
                    this->titleLineEdit->setText(QString::fromStdString(tutorial.get_title()));
                    this->presenterLineEdit->setText(QString::fromStdString(tutorial.get_presenter()));
                    this->minLineEdit->setText(QString::fromStdString(min_s));
                    this->secLineEdit->setText(QString::fromStdString(sec_s));
                    this->nrOfLikesEdit->setText(QString::fromStdString(std::to_string(tutorial.get_nr_of_likes())));
                    this->linkLineEdit->setText(QString::fromStdString(tutorial.get_link()));
                    auto *question = new QMessageBox;
                    question->setWindowTitle("Add to watch list");
                    question->setText("Do you want to add the tutorial to the watch list?");
                    QAbstractButton *myYesButton = question->addButton(QMessageBox::Yes);
                    QAbstractButton *myNoButton = question->addButton(QMessageBox::No);
                    question->addButton(QMessageBox::Cancel);
                    question->button(QMessageBox::Cancel)->hide();
                    question->setIcon(QMessageBox::Question);
                    question->exec();

                    if (question->clickedButton() == myYesButton) {
                        this->user_serv.addUserService(tutorial);
                        positions.push_back(cnt);
                        populateWatchList();
                        populateTutorialList();
                    } else if (question->clickedButton() == myNoButton)
                        ;
                    else
                        done = true;
                    question->close();
                    if (done)
                        break;
                }
                for(int pos: positions)
                {
                    Tutorials.erase(Tutorials.begin() + pos);
                }
                if(Tutorials.empty())
                {
                    done = true;
                }
                cnt = -1;
                positions.clear();
                this->tutorialListWidget->clear();
            }
        } else {
            this->validator.ValidateInputStrings(presenter_filter);
            std::vector<Tutorial> validTutorials = this->user_serv.getFiltered(presenter_filter);
            if (validTutorials.empty()) {
                std::string error;
                error += std::string("The list of valid tutorials is empty!");
                if(!error.empty())
                    throw UserException(error);
            } else {
                this->tutorialListWidget->clear();
                while (!validTutorials.empty() && !done) {
                    for (Tutorial &tutorial: validTutorials) {
                        cnt++;
                        std::string populate_string = tutorial.get_title() + " | " + tutorial.get_presenter();
                        if(cnt2 != validTutorials.size()) {
                            cnt2++;
                            this->tutorialListWidget->addItem(QString::fromStdString(populate_string));
                        }
                        tutorial.play();
                        int min, sec;
                        std::string min_s, sec_s;
                        min = tutorial.get_duration().get_minutes();
                        sec = tutorial.get_duration().get_seconds();
                        min_s = std::to_string(min);
                        sec_s = std::to_string(sec);
                        this->titleLineEdit->setText(QString::fromStdString(tutorial.get_title()));
                        this->presenterLineEdit->setText(QString::fromStdString(tutorial.get_presenter()));
                        this->minLineEdit->setText(QString::fromStdString(min_s));
                        this->secLineEdit->setText(QString::fromStdString(sec_s));
                        this->nrOfLikesEdit->setText(QString::fromStdString(std::to_string(tutorial.get_nr_of_likes())));
                        this->linkLineEdit->setText(QString::fromStdString(tutorial.get_link()));
                        auto *question = new QMessageBox;
                        question->setWindowTitle("Add to watch list");
                        question->setText("Do you want to add the tutorial to the watch list?");
                        QAbstractButton *myYesButton = question->addButton(QMessageBox::Yes);
                        QAbstractButton *myNoButton = question->addButton(QMessageBox::No);
                        question->addButton(QMessageBox::Cancel);
                        question->button(QMessageBox::Cancel)->hide();
                        question->setIcon(QMessageBox::Question);
                        question->exec();

                        if (question->clickedButton() == myYesButton) {
                            this->user_serv.addUserService(tutorial);
                            positions.push_back(cnt);
                            populateWatchList();
                        } else if (question->clickedButton() == myNoButton)
                            ;
                        else
                            done = true;
                        question->close();
                        if (done)
                            break;
                    }
                    for(int pos: positions)
                    {
                        validTutorials.erase(validTutorials.begin() + pos);
                        cnt2--;
                    }
                    if(validTutorials.empty())
                    {
                        done = true;
                    }
                    cnt = -1; cnt2=0;
                    positions.clear();
                    this->tutorialListWidget->clear();
                }
            }
        }
    } catch (ValidationException& ve) {
        auto *error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(ve.what());
        error->setWindowTitle("Validation error!");
        error->exec();
    } catch (UserException& ue) {
        auto *error = new QMessageBox();
        error->setIcon(QMessageBox::Critical);
        error->setText(ue.what());
        error->setWindowTitle("Filter error!");
        error->exec();
    }
    populateTutorialList();
}

void UserGUI::undoGUI() {
    try {
        this->user_serv.undoLastAction();
        this->populateTutorialList();
        this->populateWatchList();
    } catch (RepositoryException& re) {
        QMessageBox::critical(this, "Error", re.what());
    }
}

void UserGUI::redoGUI() {
    try {
        this->user_serv.redoLastAction();
        this->populateTutorialList();
        this->populateWatchList();
    } catch (RepositoryException& re) {
        QMessageBox::critical(this, "Error", re.what());
    }
}

UserGUI::~UserGUI() = default;