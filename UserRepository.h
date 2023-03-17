//
// Created by Marius on 3/28/2022.
//

#ifndef A5_6_917TAPOIMARIUS_USERREPOSITORY_H
#define A5_6_917TAPOIMARIUS_USERREPOSITORY_H

#endif //A5_6_917TAPOIMARIUS_USERREPOSITORY_H
#pragma once
#include "Tutorial.h"
#include <vector>

class UserRepository{
protected:
    std::vector<Tutorial> watch_list;
    std::string userFilename;
public:
    ///Constructor for the UserRepository class
    ///\param watch_list - the list of tutorials that the user will watch
    explicit UserRepository(std::vector<Tutorial> &watch_list);

    UserRepository();

    ///Method to get all the elements of the UserRepository
    ///\return - the list of elements from the UserRepository
    virtual std::vector<Tutorial>& getAllUserRepo()=0;

    ///Method to get the number of elements from the UserRepository
    ///\return - the number of elements
    virtual unsigned int getNrElems()=0;

    ///Method to get the capacity of the dynamic array used in the UserRepository
    ///\return - the capacity
    virtual unsigned int getCap()=0;

    ///Method to add an element to the UserRepository
    ///\param tutorial - the element to be added
    virtual void addUserRepo(const Tutorial& tutorial)=0;

    ///Method to delete an entity based on its index in the UserRepository (the watch list)
    ///\param delete_index - the index of the tutorial to be deleted
    virtual void deleteUserRepo(const Tutorial& tutorial)=0;

    virtual void writeToFile()=0;

    virtual std::string& getFilename()=0;

    ///Destructor
    ~UserRepository();

    virtual int findByLink(const std::string& link)=0;
};

class UserException: public std::exception {
private:
    std::string message;
public:
    explicit UserException(std::string& _message);

    const char *what() const noexcept override;
};