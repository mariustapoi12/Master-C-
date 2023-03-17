//
// Created by Marius on 3/17/2022.
//

#include "Duration.h"

Duration::Duration() {
    this->minutes = 0;
    this->seconds = 0;
}

Duration::Duration(int minutes, int seconds) {
    this -> minutes = minutes;
    this ->seconds = seconds;
}

Duration::Duration(const Duration& copy) {
    this -> minutes = copy.minutes;
    this -> seconds = copy.seconds;
}

int Duration::get_minutes() const{
    return minutes;
}

int Duration::get_seconds() const{
    return seconds;
}

/*
void Duration::set_minutes(int minutes) {
    this -> minutes = minutes;
}

void Duration::set_seconds(int seconds) {
    this -> seconds = seconds;
}
*/