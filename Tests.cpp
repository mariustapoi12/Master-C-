//
// Created by Marius on 3/29/2022.
//

#include "Tests.h"
#include "Tutorial.h"
#include "validator.h"
#include <cassert>
#include <iostream>

void TestAdminService() {
    std::string filename = R"(D:\cc++\c++\a8-9-917tapoimarius\tutorials_tests.txt)";
    std::vector<Tutorial> RepoVector;
    RepoVector.reserve(10);
    std::vector<Tutorial> UserRepoVector;
    UserRepoVector.reserve(10);
    Repository repo{RepoVector, filename};
    repo.loadTutorialsFromFile();
    Service serv = Service(repo);
    UserService user_serv = UserService(repo);
    assert(serv.getAllService()[0].get_presenter() == "freeCodeCamp.org");
    assert(serv.getCapService() == 16);
    serv.addTutorial("Rick Astley - Never Gonna Give You Up (Official Music Video)",
                                 "Rick Astley",Duration(3, 32), 13000000,
                                 "https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    assert(serv.getAllService()[10].get_presenter() == "Rick Astley");
    serv.updateTutorial("https://www.youtube.com/watch?v=dQw4w9WgXcQ",
                                      "Rick Astley - Never Gonna Give You Up (Official Music Video)",
                                      "Rick Astley",Duration(3, 32),
                                      13000000,
                                      "https://www.youtube.com/watch?v=dQw4w9WgXcZ");
    assert(serv.getAllService()[10].get_link() == "https://www.youtube.com/watch?v=dQw4w9WgXcZ");
    serv.deleteTutorial("https://www.youtube.com/watch?v=dQw4w9WgXcZ");
}

void TestUserService()
{
    std::string filename = R"(D:\cc++\c++\a8-9-917tapoimarius\tutorials_tests.txt)";
    std::vector<Tutorial> RepoVector;
    RepoVector.reserve(10);
    std::vector<Tutorial> UserRepoVector;
    UserRepoVector.reserve(10);
    Repository repo{RepoVector, filename};
    repo.loadTutorialsFromFile();
    Service serv = Service(repo);
    UserService user_serv = UserService(repo);

    user_serv.repositoryType("csv");

    assert(user_serv.getCapUserService() == 0);
    Tutorial tutorial = repo.getAllRepo()[9];
    user_serv.addUserService(tutorial);
    assert(user_serv.getNrElemsUserService() == 1);
    assert(user_serv.getAllUserService()[0].get_presenter() == "Simplilearn");
    assert(user_serv.getAllUserService()[0].get_link() == "https://www.youtube.com/watch?v=McojvctVsUs");
    std::vector<Tutorial> validTutorials1;
    validTutorials1.reserve(10);
    std::string empty;
    empty[0] = '\0';
    validTutorials1 = user_serv.getFiltered("Simplilearn");
    assert(validTutorials1[0].get_presenter() =="Simplilearn");
    std::vector<Tutorial> validTutorials3;
    validTutorials3.reserve(10);
    validTutorials3 = user_serv.getFiltered(empty);
    assert(validTutorials3[0].get_presenter() =="Videotutorial.ro");
    auto tut = Tutorial("Introducere în programare - tutorial C++ - cursul 1", "Videotutorial.ro",
                            Duration(15, 52), 6600,
    "https://www.youtube.com/watch?v=JjY9U9lu37I");
    user_serv.deleteUserService(tut, 1);
}


void TestTutorial() {
    auto tutorial = Tutorial("C++ Tutorial for Beginners - Full Course", "freeCodeCamp.org",
                             Duration(241, 18), 240000,
                             "https://www.youtube.com/watch?v=vLnPwxZdW4Y");
    tutorial.inc_likes();
    std::string str = tutorial.toString();
    assert(str == "Title: C++ Tutorial for Beginners - Full Course | Presenter: freeCodeCamp.org | Duration: 241:18 | Nr. of likes: 240001 | Link: https://www.youtube.com/watch?v=vLnPwxZdW4Y");
    Duration duration_t = tutorial.get_duration();
    assert(duration_t.get_seconds() == 18);
    assert(duration_t.get_minutes() == 241);
    assert(tutorial.get_title() == "C++ Tutorial for Beginners - Full Course");
    assert(tutorial.get_nr_of_likes() == 240001);
    //tutorial.play();
}

void TestComparator(){
    std::vector<Tutorial> test_vector;

    Tutorial t1 = Tutorial("C++ Tutorial for Beginners - Full Course", "freeCodeCamp.org",
                           Duration(241, 18), 240000,
                           "https://www.youtube.com/watch?v=vLnPwxZdW4Y");
    Tutorial t2 = Tutorial("Introducere în programare - tutorial C++ - cursul 1", "Videotutorial.ro",
                        Duration(15, 52), 6600,
                        "https://www.youtube.com/watch?v=JjY9U9lu37I");
    Tutorial t3 = Tutorial("C++ Programming Course - Beginner to Advanced", "freeCodeCamp.org",
                           Duration(1867, 30), 29000,
                           "https://www.youtube.com/watch?v=8jLOx1hD3_o");
    test_vector.push_back(t1);
    test_vector.push_back(t2);
    test_vector.push_back(t3);

    Comparator<Tutorial>* c1 = new ComparatorAscendingByLikes{};
    Comparator<Tutorial>* c2 = new ComparatorAscendingByTitle{};
    sortElements(test_vector, c1);

    assert(test_vector[0].get_nr_of_likes() == 6600);
    assert(test_vector[1].get_nr_of_likes() == 29000);
    assert(test_vector[2].get_nr_of_likes() == 240000);

    sortElements(test_vector, c2);
    assert(test_vector[0].get_title() == "C++ Programming Course - Beginner to Advanced");
    assert(test_vector[1].get_title() == "C++ Tutorial for Beginners - Full Course");
    assert(test_vector[2].get_title() == "Introducere în programare - tutorial C++ - cursul 1");

}