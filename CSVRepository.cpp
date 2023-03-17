//
// Created by Marius on 5/2/2022.
//

#include "CSVRepository.h"
#include "Repository.h"
#include <fstream>
#include <algorithm>

CSVRepository::CSVRepository(const std::vector<Tutorial> &watch_list, const std::string &userFilename) {
    this->watch_list = watch_list;
    this->userFilename = userFilename;
}

std::vector<Tutorial>& CSVRepository::getAllUserRepo() {
    return this->watch_list;
}

unsigned int CSVRepository::getNrElems() {
    return this->watch_list.size();
}

unsigned int CSVRepository::getCap() {
    return this->watch_list.capacity();
}

std::string& CSVRepository::getFilename() {
    return this->userFilename;
}

void CSVRepository::addUserRepo(const Tutorial &tutorial) {
    this->watch_list.push_back(tutorial);
    this->writeToFile();
}

void CSVRepository::deleteUserRepo(const Tutorial &tutorial)
{
    int index = this->findByLink(tutorial.get_link());
    if (index == -1) {
        std::string error;
        error += std::string("The tutorial does not exist!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    this->watch_list.erase(this->watch_list.begin() + index);
    this->writeToFile();
}

void CSVRepository::writeToFile() {
    std::ofstream fout(this->userFilename);
    if (!this->watch_list.empty()) {
        for (const Tutorial& tutorial: this->watch_list) {
            fout<<tutorial<<"\n";
        }
    }
    fout.close();
}

int CSVRepository::findByLink(const std::string &link) {
    int searched_index = -1;
    std::vector<Tutorial>::iterator it;
    it = std::find_if(this->watch_list.begin(), this->watch_list.end(), [&link](Tutorial& tutorial) {return tutorial.get_link() == link;});
    if (it != this->watch_list.end())
    {
        searched_index = it - this->watch_list.begin();
    }
    return searched_index;
}

CSVRepository::~CSVRepository()=default;