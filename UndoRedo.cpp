//
// Created by Marius on 5/30/2022.
//

#include "UndoRedo.h"

UndoRedoAdd::UndoRedoAdd(const Tutorial &tutorial, Repository &newRepo): addedTutorial(tutorial), repo(newRepo){

}

void UndoRedoAdd::undo() {
    int index = this->repo.findByLink(this->addedTutorial.get_link());
    this->repo.deleteRepo(index);
}

void UndoRedoAdd::redo() {
    this->repo.addRepo(this->addedTutorial);
}

UndoRedoRemove::UndoRedoRemove(const Tutorial &tutorial, Repository &newRepo): removedTutorial(tutorial), repo(newRepo) {

}

void UndoRedoRemove::undo() {
    this->repo.addRepo(this->removedTutorial);
}

void UndoRedoRemove::redo() {
    int index = this->repo.findByLink(this->removedTutorial.get_link());
    this->repo.deleteRepo(index);
}

UndoRedoUpdate::UndoRedoUpdate(const Tutorial &oldTutorial, const Tutorial &newTutorial, Repository &newRepo): oldTutorial(oldTutorial), newTutorial(newTutorial), repo(newRepo) {

}

void UndoRedoUpdate::undo() {
    int index = this->repo.findByLink(this->newTutorial.get_link());
    this->repo.updateRepo(index, this->oldTutorial);
}

void UndoRedoUpdate::redo() {
    int index = this->repo.findByLink(this->oldTutorial.get_link());
    this->repo.updateRepo(index, this->newTutorial);
}

UndoRedoUser::UndoRedoUser(const Tutorial &watchTutorial, Repository &newRepo, UserRepository *newUserRepo): watchTutorial(watchTutorial), repo(newRepo){
    this->userRepo = newUserRepo;
}

void UndoRedoUser::undo() {
    this->userRepo->deleteUserRepo(watchTutorial);
    this->repo.addRepo(watchTutorial);
}

void UndoRedoUser::redo() {
    int index = this->repo.findByLink(this->watchTutorial.get_link());
    this->userRepo->addUserRepo(watchTutorial);
    this->repo.deleteRepo(index);
}