//
// Created by Marius on 3/17/2022.
//

#include "Repository.h"
#include <algorithm>
#include <fstream>

RepositoryException::RepositoryException(std::string &_message) : message(_message){}

const char* RepositoryException::what() const noexcept {
    return message.c_str();
}

Repository::Repository(std::vector<Tutorial> &repo_array, std::string &tutorial_filename)
{
    this->repo = repo_array;
    this->tutorial_filename = tutorial_filename;
}

void Repository::loadTutorialsFromFile()
{
    if (!this->tutorial_filename.empty())
    {
        Tutorial tutorial_from_file;
        std::ifstream fin(this->tutorial_filename);
        while(fin >> tutorial_from_file)
        {
            if (std::find(this->repo.begin(), this->repo.end(), tutorial_from_file) ==this->repo.end())
                this->repo.push_back(tutorial_from_file);
        }
        fin.close();
    }
}

void Repository::writeTutorialsToFile()
{
    if (!this->tutorial_filename.empty()) {
        std::ofstream fout(this->tutorial_filename);
        for (const Tutorial &tutorial: this->repo) {
            fout << tutorial << "\n";
        }
        fout.close();
    }
}

std::vector<Tutorial>& Repository::getAllRepo() {
    if (this->repo.empty()) {
        std::string error;
        error += std::string("The database is empty!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    return this->repo;
}

//unsigned int Repository::getNrElems() {
//    return this->repo.size();
//}

unsigned int Repository::getCap() {
    return this->repo.capacity();
}

void Repository::addRepo(const Tutorial& tutorial) {
    int existing = this->findByLink(tutorial.get_link());
    if (existing != -1) {
        std::string error;
        error += std::string("The tutorial already exists!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    this->repo.push_back(tutorial);
    this->writeTutorialsToFile();
}

void Repository::deleteRepo(int delete_index) {
    if (delete_index == -1) {
        std::string error;
        error += std::string("The tutorial does not exist!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    this->repo.erase(this->repo.begin() + delete_index);
    this->writeTutorialsToFile();
}

int Repository::findByLink(const std::string& link) {
    int searched_index = -1;
    std::vector<Tutorial>::iterator it;
    it = std::find_if(this->repo.begin(), this->repo.end(), [&link](Tutorial &tutorial) {return tutorial.get_link() == link; });
    if(it != this->repo.end())
    {
        searched_index = it - this->repo.begin();
    }
    return searched_index;
}

void Repository::updateRepo(int update_index, const Tutorial& new_tutorial) {
    if (update_index == -1) {
        std::string error;
        error += std::string("The tutorial does not exist!");
        if(!error.empty())
            throw RepositoryException(error);
    }
    this->repo[update_index] = new_tutorial;
    this->writeTutorialsToFile();
}

Repository::~Repository()=default;
