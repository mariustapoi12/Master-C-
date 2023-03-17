//
// Created by Marius on 3/28/2022.
//

#include "UserService.h"
#include <algorithm>
#include <vector>
#include "CSVRepository.h"
#include "HTMLRepository.h"

UserService::UserService(Repository &repo, UserRepository *user_repo): repo(repo){
    this->user_repo = user_repo;
}

UserService::UserService(Repository &repo): repo(repo) {}

std::vector<Tutorial> UserService::getAllUserService() {
    return this->user_repo->getAllUserRepo();
}

unsigned int UserService::getNrElemsUserService() {
    return this->user_repo->getNrElems();
}

unsigned int UserService::getCapUserService() {
    return this->user_repo->getCap();
}

void UserService::addUserService(const Tutorial& tutorial) {
    this->user_repo->addUserRepo(tutorial);
    std::string presenter = tutorial.get_presenter();
    int delete_index = this->repo.findByLink(tutorial.get_link());
    this->repo.deleteRepo(delete_index);
    std::shared_ptr<UndoRedoAction> action = std::make_shared<UndoRedoUser>(tutorial, this->repo, this->user_repo);
    this->undoUser.push_back(action);
    this->redoUser.clear();
}

UserRepository* UserService::getUserRepo() {
    return this->user_repo;
}

void UserService::deleteUserService(const Tutorial& tutorial, int delete_index){
    this->repo.addRepo(tutorial);
    this->user_repo->deleteUserRepo(tutorial);
    std::shared_ptr<UndoRedoAction> action = std::make_shared<UndoRedoUser>(tutorial, this->repo, this->user_repo);
    this->undoUser.push_back(action);
    this->redoUser.clear();
}

std::vector<Tutorial> UserService::getFiltered(std::string filter_presenter) {
        std::vector<Tutorial> tutorials = repo.getAllRepo();
        if(filter_presenter[0] == '\0')
            return tutorials;
        std::vector<Tutorial> valid_tutorials(tutorials.size());
        auto it = std::copy_if (tutorials.begin(), tutorials.end(), valid_tutorials.begin(),[&filter_presenter](Tutorial &tutorial){return tutorial.get_presenter() == filter_presenter;} );
        valid_tutorials.resize(std::distance(valid_tutorials.begin(), it));
        return valid_tutorials;
}

std::string& UserService::getFileService() {
    this->user_repo->writeToFile();
    return this->user_repo->getFilename();
}

void UserService::repositoryType(const std::string& fileType) {
    if (fileType == "csv") {
        std::vector<Tutorial> userVector;
        std::string userFile = R"(D:\cc++\c++\a8-9-917tapoimarius\WatchList.csv)";
        auto * csv_repo = new CSVRepository{ userVector, userFile};
        this->user_repo = csv_repo;
    }else if (fileType == "html") {
        std::vector<Tutorial> userVector ;
        std::string userFile =  R"(D:\cc++\c++\a8-9-917tapoimarius\WatchList.html)";
        auto * html_repo = new HTMLRepository{userVector, userFile};
        this->user_repo = html_repo;
    } else {
        std::string error;
        error += std::string("The filename is invalid!");
        if(!error.empty())
            throw UserException(error);
    }
}

void UserService::undoLastAction() {
    if (this->undoUser.empty()) {
        std::string error;
        error += std::string("Cannot undo anymore!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    this->undoUser.back()->undo();
    this->redoUser.push_back(this->undoUser.back());
    this->undoUser.pop_back();
}

void UserService::redoLastAction() {
    if (this->redoUser.empty()) {
        std::string error;
        error += std::string("Cannot redo anymore!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    this->redoUser.back()->redo();
    this->undoUser.push_back(this->redoUser.back());
    this->redoUser.pop_back();
}

void UserService::clearUndoRedo() {
    this->undoUser.clear();
    this->redoUser.clear();
}

UserService::~UserService() = default;