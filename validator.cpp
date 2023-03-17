//
// Created by Marius on 5/2/2022.
//

#include "validator.h"
#include <algorithm>

ValidationException::ValidationException(std::string &_message): message(_message){}

const char* ValidationException::what() const noexcept {
    return message.c_str();
}
Validator::Validator() = default;

bool Validator::ValidateString(const std::string &input) {
    if (std::all_of(input.begin(), input.end(), ::isdigit)) {
        return false;
    }
    return true;
}

void Validator::ValidateInputStrings(const std::string &input){
    std::string errors;
    if(input.length()==0)
        errors+=std::string("Input cannot be empty!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateInputNumbers(const std::string &input){
    ValidateInputStrings(input);
    std::string errors;
    if(ValidateString(input))
        errors+=std::string("Input number expected!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateMinutes(const int &minutes) {
    std::string errors;
    if (minutes<0)
        errors+=std::string("Minutes cannot be less than 0!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateSeconds(const int &seconds) {
    std::string errors;
    if (seconds<0)
        errors+=std::string("Seconds cannot be less than 0!");
    else if (seconds>=60)
        errors+=std::string("Seconds cannot be greater than 59!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateNrOfLikes(const int& nr_of_likes){
    std::string errors;
    if (nr_of_likes<0)
        errors+=std::string("The number of likes cannot be less than 0!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateLink(const std::string& link)
{
    std::string errors;
    if(link.find("www") == std::string::npos)
        errors+=std::string("The link is not valid!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateValidTutorialsEmpty(std::vector<Tutorial> &validTutorials)
{
    std::string errors;
    if (validTutorials.empty())
        errors+=std::string("No tutorials corresponding!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateValidTutorialsRemaining(std::vector<Tutorial> &validTutorials)
{
    std::string errors;
    if (validTutorials.empty())
        errors+=std::string("No tutorials remaining!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateWatchList(unsigned int nr_elems)
{
    std::string errors;
    if (nr_elems==0)
        errors+=std::string("The watch list is empty!");
    if(!errors.empty())
        throw ValidationException(errors);
}

void Validator::ValidateId(int delete_id, unsigned int nr_elems)
{
    std::string errors;
    if (delete_id<0)
        errors+=std::string("The id cannot be less than 0!");
    if (delete_id>nr_elems)
        errors+=std::string("The id cannot be higher than the number of elements in the watch list!");
    if(!errors.empty())
        throw ValidationException(errors);
}