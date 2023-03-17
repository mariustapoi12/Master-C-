//
// Created by Marius on 5/6/2022.
//

#include "Comparator.h"

bool ComparatorAscendingByLikes::compare(Tutorial first, Tutorial second) {
    if (first.get_nr_of_likes()>second.get_nr_of_likes())
        return false;
    return true;
}

bool ComparatorAscendingByTitle::compare(Tutorial first, Tutorial second) {
    if (first.get_title()>second.get_title())
        return false;
    return true;
}
