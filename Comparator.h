//
// Created by Marius on 5/6/2022.
//

#ifndef A8_9_917TAPOIMARIUS_COMPARATOR_H
#define A8_9_917TAPOIMARIUS_COMPARATOR_H
#include "Tutorial.h"

template <typename Type> class Comparator{
public:
    virtual bool compare(Type first, Type second) = 0;
};

class ComparatorAscendingByLikes : public Comparator<Tutorial>{
    bool compare(Tutorial first, Tutorial second) override;
};


class ComparatorAscendingByTitle : public Comparator<Tutorial>{
public:
    bool compare(Tutorial first, Tutorial second) override;

};

#endif //A8_9_917TAPOIMARIUS_COMPARATOR_H
