//
// Created by Marius on 5/2/2022.
//

#include "HTMLRepository.h"
#include "Repository.h"
#include <fstream>
#include <algorithm>

HTMLRepository::HTMLRepository(const std::vector<Tutorial> &watch_list, const std::string &userFilename) {
    this->watch_list = watch_list;
    this->userFilename = userFilename;
}

std::vector<Tutorial>& HTMLRepository::getAllUserRepo() {
    return this->watch_list;
}

unsigned int HTMLRepository::getNrElems() {
    return this->watch_list.size();
}

unsigned int HTMLRepository::getCap() {
    return this->watch_list.capacity();
}

std::string& HTMLRepository::getFilename() {
    return this->userFilename;
}

void HTMLRepository::addUserRepo(const Tutorial &tutorial) {
    this->watch_list.push_back(tutorial);
    this->writeToFile();
}

void HTMLRepository::deleteUserRepo(const Tutorial &tutorial)
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

void HTMLRepository::writeToFile() {
    std::ofstream fout(this->userFilename);
    fout << "<!DOCTYPE html>\n<html><head><title>Watch List</title></head><body>\n";
    fout << "<table border=\"1\">\n";
    fout << "<tr><td>Title</td><td>Presenter</td><td>Duration</td><td>Nr. of likes</td><td>Link</td></tr>\n";
    for (const Tutorial& tutorial: this->watch_list) {
        auto duration = tutorial.get_duration();
        int minutes = duration.get_minutes();
        int seconds = duration.get_seconds();
        fout << "<tr><td>" << tutorial.get_title() << "</td>"
             << "<td>" << tutorial.get_presenter() << "</td>"
             << "<td>" << std::to_string(minutes) << ":" << std::to_string(seconds) << "</td>"
             << "<td>" << std::to_string(tutorial.get_nr_of_likes()) << "</td>"
             << "<td><a href=\"" << tutorial.get_link() << "\">"<<"Link"<<"</a></td>" << '\n';
    }
    fout << "</table></body></html>";
    fout.close();
}

int HTMLRepository::findByLink(const std::string &link) {
    int searched_index = -1;
    std::vector<Tutorial>::iterator it;
    it = std::find_if(this->watch_list.begin(), this->watch_list.end(), [&link](Tutorial& tutorial) {return tutorial.get_link() == link;});
    if (it != this->watch_list.end())
    {
        searched_index = it - this->watch_list.begin();
    }
    return searched_index;
}

HTMLRepository::~HTMLRepository()=default;