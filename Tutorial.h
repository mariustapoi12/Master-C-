//
// Created by Marius on 3/17/2022.
//

#ifndef A5_6_917TAPOIMARIUS_TUTORIAL_H
#define A5_6_917TAPOIMARIUS_TUTORIAL_H

#endif //A5_6_917TAPOIMARIUS_TUTORIAL_H

#pragma once
#include "Duration.h"
#include <string>
#include <Windows.h>

class Tutorial{
private:
    std::string title;
    std::string presenter;
    Duration duration;
    int nr_of_likes;
    std::string link;
public:
    ///Constructor for the Tutorial class
    Tutorial();
    ///Constructor for the Tutorial class
    Tutorial(std::string, std::string, const Duration&, int, std::string);

    /// Copy constructor for a Tutorial
    /// \param rhs copy pointer
    Tutorial(const Tutorial &rhs);

    Duration get_duration() const;

    std::string get_title() const;

    int get_nr_of_likes() const;

    /// Presenter getter
    /// \return the presenter of the turorial
    std::string get_presenter() const;

    /// Link getter
    /// \return the link of a tutorial
    std::string get_link() const;

    ///Function use for playing the video in chrome.exe
    void play() const;

    ///Function that increments the number of likes
    void inc_likes();

    ///Method to format an entity into a string
    ///\return - the string format of a tutorial
    std::string toString() const;

    bool operator==(const Tutorial& tutorialToCheck) const;

    friend std::istream& operator>>(std::istream& inputStream, Tutorial& tutorial);

    friend std::ostream& operator<<(std::ostream& outputStream, const Tutorial& tutorialOutput);

    ~Tutorial();
};
