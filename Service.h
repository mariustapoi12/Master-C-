//
// Created by Marius on 3/17/2022.
//

#ifndef A5_6_917TAPOIMARIUS_SERVICE_H
#define A5_6_917TAPOIMARIUS_SERVICE_H

#endif //A5_6_917TAPOIMARIUS_SERVICE_H
#pragma once
#include "Repository.h"
#include "Comparator.h"
#include <memory>
#include "UndoRedo.h"

class Service
{
private:
    Repository& repo;
    std::vector<std::shared_ptr<UndoRedoAction>> undoAdmin;
    std::vector<std::shared_ptr<UndoRedoAction>> redoAdmin;

public:
    ///Constructor for the Service class
    ///\param repo - the admin repository
    explicit Service(Repository& repo);

    ///Method to get all the elements of the repository
    ///\return - the elements from the repository
    std::vector<Tutorial> getAllService();

    ///Method to get the number of elements from the repository
    ///\return - the number of elements from the repository
    //unsigned int getNrElemsService();

    ///Method to get the capacity of the repository
    ///\return - the capacity
    unsigned int getCapService();

    /// Checks if the tutorial can be added and if it can it builds the object, adds it to the repo
    /// \param title title of the tutorial
    /// \param presenter name of the presenter
    /// \param duration duration of the tutorial
    /// \param nr_of_likes number of likes of the tutorial
    /// \param link link of the tutorial
    void addTutorial(std::string title, std::string presenter, Duration duration, int nr_of_likes, std::string link);

    /// Looks for the tutorial by the link. If it finds it, it deletes it
    /// \param link the link of the tutorial to be deleted
    void deleteTutorial(std::string link);

    /// Looks for the tutorial by the link. If it finds it, it updates it
    /// \param old_link the link by which the tutorial is found
    /// \param new_title the new title of the tutorial
    /// \param new_presenter the new presenter of the tutorial
    /// \param new_duration the new duration of the tutorial
    /// \param new_nr_of_likes the new number of likes of the tutorial
    /// \param new_link the new link of the tutorial
    void updateTutorial(std::string old_link, std::string new_title, std::string new_presenter, Duration new_duration,
                       int new_nr_of_likes, std::string new_link);

    int numberOfVidsPerPresenter(const std::string& presenter);

    void undoLastAction();
    void redoLastAction();
    void clearUndoRedo();

    /// Destructor
    ~Service();
};

template <typename TElem>
void sortElements(std::vector<TElem>& v, Comparator<TElem>* c) {

    bool sorted;

    do
    {
        sorted = true;

        for (int i = 0; i < v.size() - 1; i++)
        {
            if (!c->compare(v[i], v[i + 1]))
            {
                std::swap(v[i], v[i + 1]);
                sorted = false;
            }
        }
    } while (!sorted);

}