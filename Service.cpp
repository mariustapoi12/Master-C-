//
// Created by Marius on 3/17/2022.
//

#include "Service.h"
#include <iterator>

Service::Service(Repository& repo): repo(repo) {
}

std::vector<Tutorial> Service::getAllService() {
    return this->repo.getAllRepo();
}

//unsigned int Service::getNrElemsService() {
//    return this->repo.getNrElems();
//}

unsigned int Service::getCapService() {
    return this->repo.getCap();
}


void Service::addTutorial(std::string title, std::string presenter, Duration duration, int nr_of_likes,
                         std::string link)
{
    Tutorial newTutorial(title, presenter, duration, nr_of_likes, link);
    this->repo.addRepo(newTutorial);
    std::shared_ptr<UndoRedoAction> action = std::make_shared<UndoRedoAdd>(newTutorial, this->repo);
    this->undoAdmin.push_back(action);
    this->redoAdmin.clear();
}

void Service::deleteTutorial(std::string link)
{
    int delete_index = this->repo.findByLink(link);
    Tutorial tutorial = this->repo.getAllRepo()[delete_index];
    this->repo.deleteRepo(delete_index);
    std::shared_ptr<UndoRedoAction> action = std::make_shared<UndoRedoRemove>(tutorial, this->repo);
    this->undoAdmin.push_back(action);
    this->redoAdmin.clear();
}

void Service::updateTutorial(std::string old_link, std::string new_title, std::string new_presenter, Duration new_duration,
                            int new_nr_of_likes, std::string new_link) {
    int update_index = this->repo.findByLink(old_link);
    Tutorial old_tutorial = this->repo.getAllRepo()[update_index];
    Tutorial new_tutorial = Tutorial(new_title, new_presenter, new_duration, new_nr_of_likes, new_link);
    this->repo.updateRepo(update_index, new_tutorial);
    std::shared_ptr<UndoRedoAction> action = std::make_shared<UndoRedoUpdate>(old_tutorial, new_tutorial, this->repo);
    this->undoAdmin.push_back(action);
    this->redoAdmin.clear();
}

int Service::numberOfVidsPerPresenter(const std::string& presenter) {
    int count = 0;
    std::vector<Tutorial> data;
    data = this->repo.getAllRepo();
    for (const Tutorial& tutorial: data) {
        if (tutorial.get_presenter() == presenter) {
            count += 1;
        }
    }
    return count;
}

void Service::undoLastAction() {
    if (this->undoAdmin.empty()) {
        std::string error;
        error += std::string("Cannot undo anymore!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    this->undoAdmin.back()->undo();
    this->redoAdmin.push_back(this->undoAdmin.back());
    this->undoAdmin.pop_back();
}

void Service::redoLastAction() {
    if (this->redoAdmin.empty()){
        std::string error;
        error += std::string("Cannot redo anymore!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    this->redoAdmin.back()->redo();
    this->undoAdmin.push_back(this->redoAdmin.back());
    this->redoAdmin.pop_back();
}

void Service::clearUndoRedo() {
    this->undoAdmin.clear();
    this->redoAdmin.clear();
}

Service::~Service() = default;