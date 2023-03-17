//
// Created by Marius on 5/2/2022.
//

#ifndef A8_9_917TAPOIMARIUS_VALIDATOR_H
#define A8_9_917TAPOIMARIUS_VALIDATOR_H

#include "Tutorial.h"
#include <vector>

class ValidationException: public std::exception{
private:
    std::string message;
public:
    explicit ValidationException(std::string& _message);

    const char *what() const noexcept override;
};

class Validator{
public:
    Validator();

    ///Method to check if a given string does not contain digits
    ///\param input - the string to be verified
    ///\return - true if it is valid, false otherwise
    bool ValidateString(const std::string& input);

    void ValidateInputStrings(const std::string& input);

    void ValidateInputNumbers(const std::string& input);

    void ValidateMinutes(const int& minutes);

    void ValidateSeconds(const int& seconds);

    void ValidateNrOfLikes(const int& nr_of_likes);

    void ValidateLink(const std::string& link);

    void ValidateValidTutorialsEmpty(std::vector<Tutorial> &validTutorials);

    void ValidateValidTutorialsRemaining(std::vector<Tutorial> &validTutorials);

    void ValidateWatchList(unsigned int nr_elems);

    void ValidateId(int delete_id, unsigned int nr_elems);
};

#endif //A8_9_917TAPOIMARIUS_VALIDATOR_H
