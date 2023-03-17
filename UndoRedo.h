//
// Created by Marius on 5/30/2022.
//

#ifndef A11_12_917TAPOIMARIUS_UNDOREDO_H
#define A11_12_917TAPOIMARIUS_UNDOREDO_H

#include "Repository.h"
#include "UserRepository.h"

class UndoRedoAction {
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual ~UndoRedoAction() = default;
};

class UndoRedoAdd: public UndoRedoAction {
private:
    Tutorial addedTutorial;
    Repository& repo;
public:
    UndoRedoAdd(const Tutorial& tutorial, Repository& newRepo);
    void undo() override;
    void redo() override;
    ~UndoRedoAdd() override = default;
};

class UndoRedoRemove: public UndoRedoAction {
private:
    Tutorial removedTutorial;
    Repository& repo;
public:
    UndoRedoRemove(const Tutorial& tutorial, Repository& newRepo);
    void undo() override;
    void redo() override;
    ~UndoRedoRemove() override = default;
};

class UndoRedoUpdate: public UndoRedoAction {
private:
    Tutorial oldTutorial;
    Tutorial newTutorial;
    Repository& repo;
public:
    UndoRedoUpdate(const Tutorial& oldTutorial, const Tutorial& newTutorial, Repository& newRepo);
    void undo () override;
    void redo() override;
    ~UndoRedoUpdate() override = default;
};

class UndoRedoUser: public UndoRedoAction {
private:
    Tutorial watchTutorial;
    Repository& repo;
    UserRepository* userRepo;
public:
    UndoRedoUser(const Tutorial& watchTutorial, Repository& newRepo, UserRepository *newUserRepo);
    void undo() override;
    void redo() override;
    ~UndoRedoUser() override = default;
};

#endif //A11_12_917TAPOIMARIUS_UNDOREDO_H
