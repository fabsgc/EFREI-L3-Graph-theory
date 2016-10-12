#include "fonctions.h"

bool existe(list<int> mylist, int x){
    if (find(mylist.begin(), mylist.end(), x) != mylist.end())
        return true;

    return false;
}
