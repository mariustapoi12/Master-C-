//
// Created by Marius on 3/17/2022.
//
#include "Service.h"
#include "UserService.h"
#include "Repository.h"
#include "Tutorial.h"
#include "Tests.h"
#include "GUI.h"
#include "validator.h"
#include "QApplication"
#include <vector>
#include <crtdbg.h>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    std::cout<<"Starting tests..."<<std::endl;
    TestAdminService();
    //TestUserService();
    TestComparator();
    TestTutorial();
    std::cout<<"Finishing tests..."<<std::endl;
    std::string filename = R"(D:\cc++\c++\a14-917tapoimarius\tutorials.txt)";
    std::vector<Tutorial> RepoVector;
    RepoVector.reserve(10);
    std::vector<Tutorial> UserRepoVector;
    UserRepoVector.reserve(10);
    Repository repo{RepoVector, filename};
    repo.loadTutorialsFromFile();
    Service serv = Service(repo);
    UserService user_serv = UserService(repo);
    Validator validator{};
    GUI gui{serv, user_serv, validator, repo};
    gui.show();
    _CrtDumpMemoryLeaks();
    return application.exec();
}