//
// Created by Marius on 3/28/2022.
//

#include "UserRepository.h"
#include <fstream>

UserException::UserException(std::string &_message): message(_message) {}

const char* UserException::what() const noexcept {
    return message.c_str();
}

UserRepository::UserRepository() = default;

UserRepository::UserRepository(std::vector<Tutorial>& watch_list) {
    this->watch_list = watch_list;
}

UserRepository::~UserRepository() = default;