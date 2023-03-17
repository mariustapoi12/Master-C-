//
// Created by Marius on 5/2/2022.
//

#ifndef A8_9_917TAPOIMARIUS_HTMLREPOSITORY_H
#define A8_9_917TAPOIMARIUS_HTMLREPOSITORY_H

#include "UserRepository.h"

class HTMLRepository: public UserRepository{
public:
    HTMLRepository(const std::vector<Tutorial>& watch_list, const std::string& userFilename);
    std::vector<Tutorial>& getAllUserRepo() override;
    int findByLink(const std::string& link) override;
    unsigned int getNrElems() override;
    unsigned int getCap() override;
    void addUserRepo(const Tutorial &tutorial) override;
    void deleteUserRepo(const Tutorial &tutorial) override;
    void writeToFile() override;
    std::string& getFilename() override;
    ~HTMLRepository();
};

#endif //A8_9_917TAPOIMARIUS_HTMLREPOSITORY_H
