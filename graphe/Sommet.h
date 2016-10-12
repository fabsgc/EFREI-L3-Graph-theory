#ifndef SOMMET_H_INCLUDED
#define SOMMET_H_INCLUDED

#include "fonctions.h"

using namespace std;

class Sommet{
public:
    int mValue;
    int mRang;
    int mOrdre;
    int mDatePlusTot;
    int mDatePlusTard;
    int mDuree;

    list<int> mSuccesseurs;
    list<int> mPredecesseurs;

    Sommet();
};

#endif // SOMMET_H_INCLUDED
