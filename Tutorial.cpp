//
// Created by Marius on 3/17/2022.
//

#include "Tutorial.h"
#include <vector>
#include <sstream>

Tutorial::Tutorial()
:title(" "), presenter(" ") ,duration(), nr_of_likes(), link(" ")
{

}

Tutorial::Tutorial(std::string _title, std::string _presenter, const Duration& _duration, int _nr_of_likes, std::string _link)
:title(_title), presenter(_presenter), duration(_duration), nr_of_likes(_nr_of_likes), link(_link)
{

}

Tutorial::Tutorial(const Tutorial &rhs)
: title(rhs.title), presenter(rhs.presenter), duration(rhs.duration), nr_of_likes(rhs.nr_of_likes), link(rhs.link)
{

}

Duration Tutorial::get_duration() const
{
    return duration;
}

std::string Tutorial::get_presenter() const
{
    return presenter;
}

std::string Tutorial::get_title() const
{
    return title;
}

int Tutorial::get_nr_of_likes() const
{
    return nr_of_likes;
}

void Tutorial::inc_likes()
{
    nr_of_likes++;
}

std::string Tutorial::get_link() const
{
    return link;
}

void Tutorial::play() const
{
    ShellExecuteA(nullptr, nullptr, "chrome.exe", this->get_link().c_str(), nullptr, SW_SHOWMAXIMIZED);
}

std::string Tutorial::toString() const {
    auto str_min = std::to_string(this->duration.get_minutes());
    auto str_sec = std::to_string(this->duration.get_seconds());
    auto str_nr_of_likes = std::to_string(this->nr_of_likes);

    return "Title: " + this->title + " | Presenter: " + this->presenter + " | Duration: " + str_min + ":" + str_sec +
           " | Nr. of likes: " + str_nr_of_likes + " | Link: " + this ->link;
}

std::vector<std::string> tokenize(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    result.reserve(100);
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, delimiter))
        result.push_back(token);

    return result;
}

bool Tutorial::operator==(const Tutorial& tutorialToCheck) const {
    return this->link == tutorialToCheck.link;
}

std::istream& operator>>(std::istream& inputStream, Tutorial& tutorial) {
    std::string line;
    std::getline(inputStream, line);
    std::vector<std::string> tokens;
    if (line.empty())
        return inputStream;
    tokens = tokenize(line, ',');
    tutorial.title = tokens[0];
    tutorial.presenter = tokens[1];
    std::vector<std::string> duration_tokens;
    duration_tokens = tokenize(tokens[2], ':');
    tutorial.duration = Duration(std::stoi(duration_tokens[0]), std::stoi(duration_tokens[1]));
    tutorial.nr_of_likes = std::stoi(tokens[3]);
    tutorial.link = tokens[4];
    return inputStream;
}

std::ostream& operator<<(std::ostream& outputStream, const Tutorial& tutorialOutput) {
    outputStream << tutorialOutput.title << "," << tutorialOutput.presenter << "," << tutorialOutput.duration.get_minutes()<< ":" << tutorialOutput.duration.get_seconds() << "," << std::to_string(tutorialOutput.nr_of_likes) << "," << tutorialOutput.link;
    return outputStream;
}

Tutorial::~Tutorial() = default;