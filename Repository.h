//
// Created by Marius on 3/17/2022.
//

#ifndef A5_6_917TAPOIMARIUS_REPOSITORY_H
#define A5_6_917TAPOIMARIUS_REPOSITORY_H

#endif //A5_6_917TAPOIMARIUS_REPOSITORY_H

#pragma once
#include "Tutorial.h"
#include <vector>

class Repository{
private:
    std::vector<Tutorial> repo;
    std::string tutorial_filename;

public:
    void loadTutorialsFromFile();

    void writeTutorialsToFile();

    ///Constructor for the Repository class
    ///\param repo_array - the dynamic array in which the tutorials will be stored
    explicit Repository(std::vector<Tutorial> &repo_array, std::string& tutorial_filename);

    ///Method to get all the elements of the repository
    ///\return - the array of elements
    std::vector<Tutorial>& getAllRepo();

    ///Method to get the number of elements in the repository
    ///\return - the number of elements
    //unsigned int getNrElems();

    ///Method to get the capacity of the repository
    ///\return - the capacity
    unsigned int getCap();

    ///Method to add an element to the repository
    ///\param tutorial - the entity to be added
    void addRepo(const Tutorial& tutorial);

    ///Method to find an entity by link
    ///\param link - the link of the tutorial that we are searching for
    int findByLink(const std::string& link);

    ///Method to delete an entity based on its index
    ///\param delete_index - the index of the tutorial to be deleted
    void deleteRepo(int delete_index);

    ///Method to update an entity based on its index with a new entity
    ///\param update_index - the index of the tutorial to be updated
    ///\param new_tutorial - the new tutorial with which the update is done
    void updateRepo(int update_index, const Tutorial& new_tutorial);

    ///Destructor
    ~Repository();
};

class RepositoryException: public std::exception {
private:
    std::string message;
public:
    explicit RepositoryException(std::string& _message);

    const char *what() const noexcept override;
};