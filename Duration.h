//
// Created by Marius on 3/17/2022.
//

#ifndef A5_6_917TAPOIMARIUS_DURATION_H
#define A5_6_917TAPOIMARIUS_DURATION_H

#endif //A5_6_917TAPOIMARIUS_DURATION_H

#pragma once

class Duration{
private:
    int minutes;
    int seconds;
public:
    ///Default constructor
    Duration();
    /// Constructor
    /// \param minutes of the duration
    /// \param seconds of the duration
    Duration(int minutes, int seconds);
    /// Copy constructor
    /// \param copy pointer to the copies
    Duration(const Duration& copy);
    /// Minutes getter
    /// \return the minutes of the duration
    int get_minutes()const;
    /// Seconds getter
    /// \return the seconds of the duration
    int get_seconds()const;

/*
void set_minutes (int minutes);
void set_seconds (int seconds);
 */
};