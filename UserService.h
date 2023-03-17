//
// Created by Marius on 3/28/2022.
//

#ifndef A5_6_917TAPOIMARIUS_USERSERVICE_H
#define A5_6_917TAPOIMARIUS_USERSERVICE_H

#endif //A5_6_917TAPOIMARIUS_USERSERVICE_H

#pragma once
#include "UserRepository.h"
#include "Repository.h"
#include <memory>
#include "UndoRedo.h"

class UserService{
private:
    Repository& repo;
    UserRepository* user_repo;
    std::vector<std::shared_ptr<UndoRedoAction>> undoUser;
    std::vector<std::shared_ptr<UndoRedoAction>> redoUser;
public:
    ///Constructor for the UserService class
    ///\param repo - the admin repository
    ///\param user_repo - the user repository
    UserService(Repository& repo, UserRepository * user_repo);

    explicit UserService(Repository& repo);

    ///Method to get all the elements from the user repository
    ///\return - the elements from the user repository
    std::vector<Tutorial> getAllUserService();

    ///Method to get the number of elements from the user repository
    ///\return - the number of elements
    unsigned  int getNrElemsUserService();

    ///Method to get the capacity of the user repository
    ///\return - the capacity
    unsigned int getCapUserService();

    ///Method to add a new tutorial to the user repository
    ///\param tutorial - the tutorial to be added
    void addUserService(const Tutorial& tutorial);

    /// Method to delete a tutorial from the watch list
    /// \param tutorial - the tutorial which will be deleted
    void deleteUserService(const Tutorial& tutorial, int delete_index);

    ///Method to get the filtered list of tutorials based on their breed and age
    ///\param filter_presenter - the presenter filter
    ///\return - the number of elements from the valid_tutorials array
    std::vector<Tutorial> getFiltered(std::string filter_presenter);

    void repositoryType(const std::string& fileType);

    std::string& getFileService();

    void undoLastAction();
    void redoLastAction();
    void clearUndoRedo();

    UserRepository* getUserRepo();

    ///Destructor
    ~UserService();
};
